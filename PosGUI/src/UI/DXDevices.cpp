module;
#include <d2d1_3.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <Windows.h>

module PGUI.UI.DXDevices;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Factories;

namespace PGUI::UI::DXDevices
{
	// ReSharper disable CppInconsistentNaming

	ComPtr<ID3D11Device2> d3d11Device;
	ComPtr<IDXGIDevice4> dxgiDevice;
	ComPtr<ID2D1Device7> d2d1Device;
	std::atomic_uint64_t deviceCreationID{ 0 };

	// ReSharper restore CppInconsistentNaming

	auto InitD3D11Device() -> void
	{
		if (d3d11Device)
		{
			Logger::Info(L"DXDevices::InitD3D11Device called, but D3D11 device already initialized");
			return;
		}

		const auto dxgiFactory = Factories::DXGIFactory::GetFactory();
		SYSTEM_POWER_STATUS powerStatus{ };
		if (const auto ret = GetSystemPowerStatus(&powerStatus);
			ret == 0)
		{
			Logger::Error(
				L"Cannot get system power status {}",
				Error{ GetLastError() }
			);
		}

		auto gpuPreference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
		const auto batteryFlags = FromUnderlying<BatteryFlag>(powerStatus.BatteryFlag);
		const auto powerSaverOn = powerStatus.SystemStatusFlag == 1;
		const auto lowBattery = IsAnyFlagSet(batteryFlags, BatteryFlag::Low, BatteryFlag::Critical);
		const auto charging = powerStatus.ACLineStatus != 0 || IsFlagSet(batteryFlags, BatteryFlag::Charging);

		if (const auto hasBattery = !IsFlagSet(batteryFlags, BatteryFlag::NoBattery);
			(powerSaverOn || lowBattery) && (!charging) && hasBattery)
		{
			gpuPreference = DXGI_GPU_PREFERENCE_MINIMUM_POWER;
		}

		ComPtr<IDXGIAdapter1> adapter;
		if (const auto error = Error{
				dxgiFactory->EnumAdapterByGpuPreference(
					0, gpuPreference,
					GetIID(adapter),
					adapter.put_void())
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot enumerate DXGI adapters" };
		}

		constexpr auto createDeviceFlags =
				#ifdef _DEBUG
				static_cast<D3D11_CREATE_DEVICE_FLAG>(D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG);
		#else
		D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		#endif

		constexpr std::array featureLevels =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		ComPtr<ID3D11Device> device;
		if (const auto error = Error{
				D3D11CreateDevice(
					adapter.get(),
					D3D_DRIVER_TYPE_UNKNOWN, nullptr,
					createDeviceFlags,
					featureLevels.data(),
					static_cast<UINT>(featureLevels.size()),
					D3D11_SDK_VERSION,
					device.put(), nullptr, nullptr)
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot create D3D11 device" };
		}

		d3d11Device = device.try_query<ID3D11Device2>();
		if (d3d11Device.get() == nullptr)
		{
			throw Exception{ Error{ SystemErrorCode::InterfaceNotSupported }, L"Cannot query D3D11Device2 interface" };
		}

		dxgiDevice = d3d11Device.try_query<IDXGIDevice4>();
		if (dxgiDevice.get() == nullptr)
		{
			throw Exception{ Error{ SystemErrorCode::InterfaceNotSupported }, L"Cannot query IDXGIDevice4 interface" };
		}
	}

	auto InitD2D1Device() -> void
	{
		if (d2d1Device)
		{
			Logger::Info(L"DXDevices::InitD2D1Device called, but D2D1 device already initialized");
			return;
		}

		const auto d2Factory = Factories::D2DFactory::GetFactory();

		if (const auto error = Error{
				d2Factory->CreateDevice(
					dxgiDevice.get(),
					d2d1Device.put())
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot create D2D1 device" };
		}
	}

	auto InitDevices() -> void
	{
		InitD3D11Device();
		InitD2D1Device();
		deviceCreationID.fetch_add(1, std::memory_order_relaxed);
	}

	auto ResetDevices() noexcept -> void
	{
		d3d11Device.reset();
		dxgiDevice.reset();
		d2d1Device.reset();
	}

	auto GetDeviceCreationID() noexcept -> std::uint64_t
	{
		return deviceCreationID.load(std::memory_order_relaxed);
	}

	auto D3D11Device() noexcept -> ComPtr<ID3D11Device2>& { return d3d11Device; }
	auto DXGIDevice() noexcept -> ComPtr<IDXGIDevice4>& { return dxgiDevice; }
	auto D2D1Device() noexcept -> ComPtr<ID2D1Device7>& { return d2d1Device; }
}

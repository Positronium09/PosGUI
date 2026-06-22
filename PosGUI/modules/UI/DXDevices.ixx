module;
#include <d2d1_3.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <Windows.h>

export module PGUI.UI.DXDevices;

import std;

import PGUI.ComPtr;

namespace PGUI::UI::DXDevices
{
	enum class BatteryFlag : BYTE
	{
		High = BATTERY_FLAG_HIGH,
		Low = BATTERY_FLAG_LOW,
		Critical = BATTERY_FLAG_CRITICAL,
		Charging = BATTERY_FLAG_CHARGING,
		NoBattery = BATTERY_FLAG_NO_BATTERY,
		Unknown = BATTERY_FLAG_UNKNOWN
	};

	consteval auto MakeEnumFlag(BatteryFlag) noexcept -> void { }

	// ReSharper disable CppInconsistentNaming
	
	inline ComPtr<ID3D11Device2> d3d11Device;
	inline ComPtr<IDXGIDevice4> dxgiDevice;
	inline ComPtr<ID2D1Device7> d2d1Device;
	inline std::atomic_uint64_t deviceCreationID{ 0 };

	// ReSharper restore CppInconsistentNaming
}

export namespace PGUI::UI::DXDevices
{
	auto InitD3D11Device() -> void;
	auto InitD2D1Device() -> void;

	inline auto InitDevices() -> void
	{
		InitD3D11Device();
		InitD2D1Device();
		deviceCreationID.fetch_add(1, std::memory_order_relaxed);
	}

	inline auto ResetDevices() noexcept -> void
	{
		d3d11Device.reset();
		dxgiDevice.reset();
		d2d1Device.reset();
	}

	[[nodiscard]] inline auto GetDeviceCreationID() noexcept
	{
		return deviceCreationID.load(std::memory_order_relaxed);
	}

	[[nodiscard]] inline auto& D3D11Device() noexcept { return d3d11Device; }
	[[nodiscard]] inline auto& DXGIDevice() noexcept { return dxgiDevice; }
	[[nodiscard]] inline auto& D2D1Device() noexcept { return d2d1Device; }
}

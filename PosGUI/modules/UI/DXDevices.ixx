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
}

export namespace PGUI::UI::DXDevices
{
	auto InitD3D11Device() -> void;
	auto InitD2D1Device() -> void;
	auto InitDevices() -> void;
	auto ResetDevices() noexcept -> void;

	[[nodiscard]] auto GetDeviceCreationID() noexcept -> std::uint64_t;

	[[nodiscard]] auto D3D11Device() noexcept -> ComPtr<ID3D11Device2>&;
	[[nodiscard]] auto DXGIDevice() noexcept -> ComPtr<IDXGIDevice4>&;
	[[nodiscard]] auto D2D1Device() noexcept -> ComPtr<ID2D1Device7>&;
}

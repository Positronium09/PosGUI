module;
#include <d2d1.h>
#include <d2d1_3.h>
#include <d3d11_4.h>
#include <dcomp.h>
#include <dxgi1_2.h>
#include <Windows.h>

export module PGUI.UI.DirectXCompositionWindow;

import std;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ComPtr;
import PGUI.UI.Clip;
import PGUI.UI.Graphics;
import PGUI.ErrorHandling;

export namespace PGUI::UI
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
	DEFINE_ENUM_FLAG_OPERATORS(BatteryFlag);

	class DirectXCompositionWindow :
		public Window,
		protected ComPtrHolder<IDXGISwapChain1, IDCompositionTarget, ID2D1DeviceContext7, IDCompositionVisual3>
	{
		public:
		~DirectXCompositionWindow() override = default;

		static auto InitD3D11Device() -> void;

		static auto InitDCompDevice() -> void;

		static auto InitD2D1Device() -> void;

		static auto InitDevices() -> void
		{
			InitD3D11Device();
			InitDCompDevice();
			InitD2D1Device();
			deviceCreationID.fetch_add(1, std::memory_order_relaxed);
		}

		[[nodiscard]] const auto& GetD2D1DeviceContext() const noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] const auto& GetDCompositionTarget() const noexcept { return Get<IDCompositionTarget>(); }
		[[nodiscard]] const auto& GetSwapChain() const noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] const auto& GetDCompositionVisual() const noexcept { return Get<IDCompositionVisual3>(); }
		[[nodiscard]] auto& GetD2D1DeviceContext() noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] auto& GetDCompositionTarget() noexcept { return Get<IDCompositionTarget>(); }
		[[nodiscard]] auto& GetSwapChain() noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] auto& GetDCompositionVisual() noexcept { return Get<IDCompositionVisual3>(); }

		[[nodiscard]] static auto& D3D11Device() noexcept { return d3d11Device; }
		[[nodiscard]] static auto& DXGIDevice() noexcept { return dxgiDevice; }
		[[nodiscard]] static auto& DCompositionDevice() noexcept { return dCompositionDevice; }
		[[nodiscard]] static auto& DCompositionSurfaceFactory() noexcept { return dCompositionSurfaceFactory; }
		[[nodiscard]] static auto& D2D1Device() noexcept { return d2d1Device; }

		[[nodiscard]] auto GetGraphics() const noexcept
		{
			Graphics graphics{ GetD2D1DeviceContext() };
			const auto dpi = GetDpi();
			graphics.SetDpi(dpi);
			return graphics;
		}

		[[nodiscard]] static auto GetDeviceCreationID() noexcept
		{
			return deviceCreationID.load(std::memory_order_relaxed);
		}

		virtual auto BeginDraw() -> void;

		virtual auto EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG>;

		virtual auto CreateDeviceResources() -> void
		{
			/* */
		}

		virtual auto DiscardDeviceResources() -> void
		{
			/* */
		}

		virtual auto Draw(const Graphics&) -> void
		{
			/* E_NOTIMPL */
		}

		protected:
		explicit DirectXCompositionWindow(const WindowClassPtr& wndClass) noexcept;

		auto OnSizeChanged(SizeL newSize) -> void override;

		private:
		inline static ComPtr<ID3D11Device2> d3d11Device;
		inline static ComPtr<IDXGIDevice4> dxgiDevice;
		inline static ComPtr<IDCompositionDevice5> dCompositionDevice;
		inline static ComPtr<IDCompositionSurfaceFactory> dCompositionSurfaceFactory;
		inline static ComPtr<ID2D1Device7> d2d1Device;

		HMONITOR currentMonitor = nullptr;
		PAINTSTRUCT paintStruct{ };
		static inline std::atomic_uint64_t deviceCreationID{ 0 };

		auto InitSwapChain() -> void;

		auto InitD2D1DeviceContext() -> void;

		auto InitDirectComposition() -> void;

		auto InitDeviceDependent()
		{
			InitSwapChain();
			InitD2D1DeviceContext();
			InitDirectComposition();
		}

		static auto HandleDeviceLoss()
		{
			d3d11Device.reset();
			dxgiDevice.reset();
			dCompositionDevice.reset();
			d2d1Device.reset();
			dCompositionSurfaceFactory.reset();
			InitDevices();
		}

		auto OnNCCreate(MessageID msg, Argument1 arg1, Argument2 arg2) -> MessageHandlerResult;

		auto OnWindowPosChanged(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;

		auto OnPaint(MessageID msg, Argument1 arg1, Argument2 arg2) -> MessageHandlerResult;
	};
}

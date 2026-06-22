module;
#include <d2d1.h>
#include <d2d1_3.h>
#include <dcomp.h>
#include <dxgi1_2.h>
#include <Windows.h>

export module PGUI.UI.DCompWindow;

import std;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.UI.Clip;
import PGUI.UI.Graphics;
import PGUI.UI.DComp;
import PGUI.UI.DXDevices;
import PGUI.ErrorHandling;

export namespace PGUI::UI
{
	class DCompWindow :
		public Window,
		protected ComPtrHolder<IDXGISwapChain1, ID2D1DeviceContext7>
	{
		public:
		~DCompWindow() override = default;

		static auto InitDCompDevice() -> void;

		static auto InitDevices() -> void
		{
			DXDevices::InitDevices();
			InitDCompDevice();
		}

		[[nodiscard]] static auto GetDeviceCreationID() noexcept
		{
			return DXDevices::GetDeviceCreationID();
		}

		[[nodiscard]] static auto& D3D11Device() noexcept { return DXDevices::D3D11Device(); }
		[[nodiscard]] static auto& DXGIDevice() noexcept { return DXDevices::DXGIDevice(); }
		[[nodiscard]] static auto& D2D1Device() noexcept { return DXDevices::D2D1Device(); }
		[[nodiscard]] static auto& DCompositionDevice() noexcept { return dCompositionDevice; }
		[[nodiscard]] static auto& DCompositionSurfaceFactory() noexcept { return dCompositionSurfaceFactory; }

		[[nodiscard]] const auto& GetD2D1DeviceContext() const noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] const auto& GetDCompositionTarget() const noexcept { return target; }
		[[nodiscard]] const auto& GetSwapChain() const noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] const auto& GetDCompositionVisual() const noexcept { return visual; }
		[[nodiscard]] auto& GetD2D1DeviceContext() noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] auto& GetDCompositionTarget() noexcept { return target; }
		[[nodiscard]] auto& GetSwapChain() noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] auto& GetDCompositionVisual() noexcept { return visual; }

		[[nodiscard]] auto GetGraphics() const noexcept
		{
			Graphics graphics{ GetD2D1DeviceContext() };
			const auto dpi = GetDpi();
			graphics.SetDpi(dpi);
			return graphics;
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
		explicit DCompWindow(const WindowClassPtr& wndClass) noexcept;

		auto OnSizeChanged(SizeL newSize) -> void override;

		private:
		static inline DComp::Device dCompositionDevice;
		static inline DComp::SurfaceFactory dCompositionSurfaceFactory;
		DComp::Target target;
		DComp::Visual visual;

		HMONITOR currentMonitor = nullptr;
		PAINTSTRUCT paintStruct{ };

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
			dCompositionDevice.ResetAll();
			dCompositionSurfaceFactory.Reset();
			DXDevices::ResetDevices();
			InitDevices();
		}

		auto OnNCCreate(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;

		auto OnWindowPosChanged(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;

		auto OnPaint(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
	};
}

module;
#include <d2d1.h>
#include <d2d1_3.h>
#include <d3d11_4.h>
#include <dcomp.h>
#include <dxgi1_2.h>
#include <Windows.h>
#include <wrl.h>

export module PGUI.UI.DirectXCompositionWindow;

import std;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ComPtr;
import PGUI.UI.Clip;
import PGUI.UI.Graphics;

export namespace PGUI::UI
{
	class DirectXCompositionWindow :
		public Window,
		protected ComPtrHolder<IDXGISwapChain1, IDCompositionTarget, ID2D1DeviceContext7, IDCompositionVisual>
	{
		public:
		~DirectXCompositionWindow() override = default;

		static auto InitD3D11Device() -> void;

		static auto InitDCompDevice() -> void;

		static auto InitD2D1Device() -> void;

		[[nodiscard]] const auto& GetD2D1DeviceContext() const noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] const auto& GetDCompositionTarget() const noexcept { return Get<IDCompositionTarget>(); }
		[[nodiscard]] const auto& GetSwapChain() const noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] const auto& GetDCompositionVisual() const noexcept { return Get<IDCompositionVisual>(); }
		[[nodiscard]] auto& GetD2D1DeviceContext() noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] auto& GetDCompositionTarget() noexcept { return Get<IDCompositionTarget>(); }
		[[nodiscard]] auto& GetSwapChain() noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] auto& GetDCompositionVisual() noexcept { return Get<IDCompositionVisual>(); }

		[[nodiscard]] static auto& D3D11Device() noexcept { return d3d11Device; }
		[[nodiscard]] static auto& DXGIDevice() noexcept { return dxgiDevice; }
		[[nodiscard]] static auto& DCompositionDevice() noexcept { return dcompDevice; }
		[[nodiscard]] static auto& D2D1Device() noexcept { return d2d1Device; }

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

		virtual auto Draw(Graphics) -> void
		{
			/* E_NOTIMPL */
		}

		protected:
		explicit DirectXCompositionWindow(const WindowClassPtr& wndClass) noexcept;

		auto OnSizeChanged(SizeL newSize) -> void override;

		private:
		inline static ComPtr<ID3D11Device2> d3d11Device;
		inline static ComPtr<IDXGIDevice4> dxgiDevice;
		inline static ComPtr<IDCompositionDevice> dcompDevice;
		inline static ComPtr<ID2D1Device7> d2d1Device;

		HMONITOR currentMonitor;
		PAINTSTRUCT paintStruct{ };

		auto InitSwapChain() -> void;

		auto InitD2D1DeviceContext() -> void;

		auto InitDirectComposition() -> void;

		auto OnNCCreate(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;

		auto OnWindowPosChanged(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;

		auto OnPaint(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;
	};
}

module;
#include <Windows.h>
#include <wrl.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_6.h>
#include <d2d1.h>
#include <d2d1_3.h>
#include <d3d11.h>
#include <d3d11_4.h>
#include <dcomp.h>
#include <utility>

export module PGUI.UI.DirectXCompositionWindow;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ComPtr;
import PGUI.UI.Clip;
import PGUI.UI.Graphics;

namespace PGUI
{
	extern void Init();
}

export namespace PGUI::UI
{
	class DirectXCompositionWindow :
		public Window,
		protected ComPtrHolder<IDXGISwapChain1, IDCompositionTarget, ID2D1DeviceContext7>
	{
		friend void PGUI::Init();

		public:
		virtual ~DirectXCompositionWindow() = default;

		[[nodiscard]] const auto& GetD2D1DeviceContext() const noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] const auto& GetDCompositionTarget() const noexcept { return Get<IDCompositionTarget>(); }
		[[nodiscard]] const auto& GetSwapChain() const noexcept { return Get<IDXGISwapChain1>(); }
		[[nodiscard]] auto& GetD2D1DeviceContext() noexcept { return Get<ID2D1DeviceContext7>(); }
		[[nodiscard]] auto& GetDCompositionTarget() noexcept { return Get<IDCompositionTarget>(); }
		[[nodiscard]] auto& GetSwapChain() noexcept { return Get<IDXGISwapChain1>(); }

		[[nodiscard]] static auto D3D11Device() noexcept { return d3d11Device; }
		[[nodiscard]] static auto DXGIDevice() noexcept { return dxgiDevice; }
		[[nodiscard]] static auto DCompositionDevice() noexcept { return dcompDevice; }
		[[nodiscard]] static auto D2D1Device() noexcept { return d2d1Device; }

		[[nodiscard]] auto GetGraphics() const noexcept { return Graphics{ GetD2D1DeviceContext() }; }

		virtual void BeginDraw();
		virtual auto EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG>;

		virtual void CreateDeviceResources() { /* */ }
		virtual void DiscardDeviceResources() { /* */ }

		protected:
		DirectXCompositionWindow(const WindowClassPtr& wndClass) noexcept;

		private:
		inline static ComPtr<ID3D11Device2> d3d11Device;
		inline static ComPtr<IDXGIDevice4> dxgiDevice;
		inline static ComPtr<IDCompositionDevice> dcompDevice;
		inline static ComPtr<ID2D1Device7> d2d1Device;

		HMONITOR currentMonitor;
		PAINTSTRUCT paintStruct{ };

		void InitSwapChain();
		void InitD2D1DeviceContext();
		void InitDirectComposition();
		static void InitD3D11Device();
		static void InitDCompDevice();
		static void InitD2D1Device();

		auto OnNCCreate(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;
		auto OnWindowPosChanged(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;
		auto OnSize(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;
	};
}

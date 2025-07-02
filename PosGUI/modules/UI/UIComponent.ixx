module;
#include <d2d1_3.h>
#include <Windows.h>

export module PGUI.UI.UIComponent;

import std;

import PGUI.WindowClass;
import PGUI.UI.Clip;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	class UIComponent : public DirectXCompositionWindow
	{
		public:
		UIComponent();
		explicit UIComponent(const WindowClassPtr& wndClass);

		void SetClip(const ClipParameters& params) noexcept;
		void ClearClip() noexcept;
		[[nodiscard]] const auto& GetClip() const noexcept { return clip; }

		void HitTestClipGeometry(bool enable) noexcept { hitTestClip = enable; }
		[[nodiscard]] auto IsHitTestClipGeometryEnabled() const noexcept { return hitTestClip; }

		void AdjustClipOnSize(bool enable) noexcept { adjustClipOnSize = enable; }
		[[nodiscard]] auto IsAdjustClipOnSizeEnabled() const noexcept { return adjustClipOnSize; }

		protected:
		void BeginDraw() override;
		auto EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG> override;

		virtual void OnClipChanged() { /* */ }

		void AutoDpiScaleDrawing(bool enable) noexcept { autoDpiScaledDrawing = enable; }
		[[nodiscard]] auto IsAutoDpiScaleDrawingEnabled() const noexcept { return autoDpiScaledDrawing; }

		void AdjustClip() noexcept;

		private:
		Clip clip;
		bool autoDpiScaledDrawing = true;
		bool adjustClipOnSize = true;
		bool hitTestClip = true;
		
		[[nodiscard]] auto OnCreate(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
		[[nodiscard]] auto OnNcHitTest(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult;
		auto OnSize(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;
	};
}

module;
#include <d2d1_3.h>
#include <Windows.h>

export module PGUI.UI.UIComponent;

import std;

import PGUI.Window;
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

		auto SetClip(const ClipParameters& params) noexcept -> void;

		auto ClearClip() noexcept -> void;

		[[nodiscard]] const auto& GetClip() const noexcept { return clip; }

		auto HitTestClipGeometry(const bool enable) noexcept -> void { hitTestClip = enable; }
		[[nodiscard]] auto IsHitTestClipGeometryEnabled() const noexcept { return hitTestClip; }

		auto AdjustClipOnSize(const bool enable) noexcept -> void { adjustClipOnSize = enable; }
		[[nodiscard]] auto IsAdjustClipOnSizeEnabled() const noexcept { return adjustClipOnSize; }

		protected:
		auto BeginDraw() -> void override;

		auto EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG> override;

		virtual auto OnClipChanged() -> void
		{
			/* */
		}

		auto AutoDpiScaleDrawing(const bool enable) noexcept -> void { autoDpiScaledDrawing = enable; }
		[[nodiscard]] auto IsAutoDpiScaleDrawingEnabled() const noexcept { return autoDpiScaledDrawing; }

		auto AdjustClip() noexcept -> void;

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

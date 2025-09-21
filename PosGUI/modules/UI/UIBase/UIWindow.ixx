module;
#include <Windows.h>

export module PGUI.UI.UIBase:UIWindow;

import std;

import :Interface;
import PGUI.Event;
import PGUI.Shape2D;
import PGUI.Window;
import PGUI.UI.Graphics;
import PGUI.ErrorHandling;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	class UIWindow : public DirectXCompositionWindow
	{
		public:
		UIWindow() noexcept;
		explicit UIWindow(const WindowClassPtr& wndClass) noexcept;

		private:
		RawUIComponentPtr<> hovered;
		RawUIComponentPtr<> pressed;
		RawUIComponentPtr<> focused;

		auto Draw(Graphics graphics) -> void override;

		auto OnMouseMove(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnLButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnLButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnRButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnRButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
	};
}

module;
#include <Windows.h>

module PGUI.UI.UIBase:UIWindow;

import std;

import :UIComponent;
import PGUI.Window;
import PGUI.Event;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIWindow::UIWindow() noexcept :
		UIWindow{ WindowClass::Create(L"PGUI_UIWindow") }
	{ }

	UIWindow::UIWindow(const WindowClassPtr& wndClass) noexcept :
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_MOUSEMOVE, &UIWindow::OnMouseMove);
		RegisterHandler(WM_LBUTTONDOWN, &UIWindow::OnLButtonDown);
		RegisterHandler(WM_LBUTTONUP, &UIWindow::OnLButtonUp);
		RegisterHandler(WM_RBUTTONDOWN, &UIWindow::OnRButtonDown);
		RegisterHandler(WM_RBUTTONUP, &UIWindow::OnRButtonUp);
	}

	auto UIWindow::Draw(const Graphics graphics) -> void
	{
		DirectXCompositionWindow::Draw(graphics);
	}

	auto UIWindow::OnMouseMove(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnLButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnLButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnRButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnRButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}
}

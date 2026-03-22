module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

module PGUI.UI.UICore:UIHost;

import :Interface;
import :UIElement;
import :UIContainer;

import std;

import PGUI.Utils;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIHost::UIHost() :
		UIHost{ WindowClass::Create(L"PGUI_UIHost") }
	{ }

	UIHost::UIHost(const WindowClassPtr& windowClass) :
		DirectXCompositionWindow{ windowClass }
	{
		redrawRequestedEvent.AddCallback(
			std::bind_front(&UIHost::RedrawRequested, this)
		);

		/*
		RegisterHandler(WM_NCCREATE, &UIHost::OnNCCreate);
		RegisterHandler(WM_SIZE, &UIHost::OnSize);

		RegisterHandler(WM_SETFOCUS, &UIHost::OnFocusChanged);
		RegisterHandler(WM_KILLFOCUS, &UIHost::OnFocusChanged);

		RegisterHandler(WM_MOUSEMOVE, &UIHost::OnMouseMove);
		RegisterHandler(WM_MOUSEHOVER, &UIHost::OnMouseHover);
		RegisterHandler(WM_MOUSELEAVE, &UIHost::OnMouseLeave);
		RegisterHandler(WM_MOUSEWHEEL, &UIHost::OnMouseWheel);

		RegisterHandler(WM_LBUTTONDOWN, &UIHost::OnMouseButtonDown);
		RegisterHandler(WM_RBUTTONDOWN, &UIHost::OnMouseButtonDown);
		RegisterHandler(WM_MBUTTONDOWN, &UIHost::OnMouseButtonDown);
		RegisterHandler(WM_XBUTTONDOWN, &UIHost::OnMouseButtonDown);

		RegisterHandler(WM_LBUTTONUP, &UIHost::OnMouseButtonUp);
		RegisterHandler(WM_RBUTTONUP, &UIHost::OnMouseButtonUp);
		RegisterHandler(WM_MBUTTONUP, &UIHost::OnMouseButtonUp);
		RegisterHandler(WM_XBUTTONUP, &UIHost::OnMouseButtonUp);

		RegisterHandler(WM_LBUTTONDBLCLK, &UIHost::OnMouseDoubleClick);
		RegisterHandler(WM_RBUTTONDBLCLK, &UIHost::OnMouseDoubleClick);
		RegisterHandler(WM_MBUTTONDBLCLK, &UIHost::OnMouseDoubleClick);
		RegisterHandler(WM_XBUTTONDBLCLK, &UIHost::OnMouseDoubleClick);

		RegisterHandler(WM_CHAR, &UIHost::OnChar);
		RegisterHandler(WM_UNICHAR, &UIHost::OnChar);
		RegisterHandler(WM_DEADCHAR, &UIHost::OnChar);
		RegisterHandler(WM_KEYDOWN, &UIHost::OnChar);
		RegisterHandler(WM_KEYUP, &UIHost::OnChar);
		*/
	}

	auto UIHost::CreateDeviceResources() -> void
	{
		DirectXCompositionWindow::CreateDeviceResources();
		rootContainer->CreateDeviceResources();
	}

	auto UIHost::DiscardDeviceResources() -> void
	{
		DirectXCompositionWindow::DiscardDeviceResources();
		if (rootContainer)
		{
			rootContainer->DiscardDeviceResources();
		}
	}

	auto UIHost::OnNCCreate(MessageID, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		rootContainer = UIElement::Create<UIContainer>(GetClientRect());
		rootContainer->RedrawRequestedEvent().AddCallback(
			std::bind_front(&UIHost::RedrawRequested, this)
		);
		rootContainer->host = this;

		const auto style = GetClassStyle() | CS_DBLCLKS;

		if (const auto result = SetClassLongPtrW(Hwnd(), GCL_STYLE, style);
			result != ERROR_SUCCESS)
		{
			if (const Error error{ GetLastError() };
				error.IsFailure())
			{
				Logger::Error(error, L"SetClassLongPtrW gave an error at UIHost::OnNCCreate");
			}
		}

		return TRUE;
	}

	auto UIHost::OnSize(MessageID, const Argument1 arg1, const Argument2 arg2) noexcept -> MessageHandlerResult
	{
		auto [sizeFlag, newSize] = ParseSizeMessage(arg1, arg2);
		rootContainer->Resize(newSize);

		return 0;
	}

	/*
	auto UIHost::OnFocusChanged(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnChar(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseMove(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseHover(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseLeave(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseWheel(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseButtonDown(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseButtonUp(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	auto UIHost::OnMouseDoubleClick(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult { }
	*/

	auto UIHost::RedrawRequested(const RawUIElementPtr<>) noexcept -> void
	{
		Invalidate(false);
	}

	auto UIHost::Draw(const Graphics& graphics) noexcept -> void
	{
		BeginDraw();

		Render(graphics);
		rootContainer->Render(graphics);

		EndDraw();
	}
}

module;
#include <Windows.h>

module PGUI.UI.UIBase:UIWindow;

import std;

import :UIElement;
import :UIContainer;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ErrorHandling;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIWindow::UIWindow() :
		UIWindow{ WindowClass::Create(L"PGUI_UIWindow") }
	{ }

	UIWindow::UIWindow(const WindowClassPtr& wndClass) :
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_NCCREATE, &UIWindow::OnNCCreate);

		RegisterHandler(WM_SETFOCUS, &UIWindow::OnFocusChanged);
		RegisterHandler(WM_KILLFOCUS, &UIWindow::OnFocusChanged);

		RegisterHandler(WM_CHAR, &UIWindow::OnChar);
		RegisterHandler(WM_UNICHAR, &UIWindow::OnChar);
		RegisterHandler(WM_DEADCHAR, &UIWindow::OnChar);
		RegisterHandler(WM_KEYDOWN, &UIWindow::OnKey);
		RegisterHandler(WM_KEYUP, &UIWindow::OnKey);

		RegisterHandler(WM_MOUSEMOVE, &UIWindow::OnMouseMove);
		RegisterHandler(WM_MOUSELEAVE, &UIWindow::OnMouseLeave);
		RegisterHandler(WM_MOUSEHOVER, &UIWindow::OnMouseHover);
		RegisterHandler(WM_MOUSEWHEEL, &UIWindow::OnMouseWheel);

		RegisterHandler(WM_LBUTTONDOWN, &UIWindow::OnMouseButtonDown);
		RegisterHandler(WM_RBUTTONDOWN, &UIWindow::OnMouseButtonDown);
		RegisterHandler(WM_MBUTTONDOWN, &UIWindow::OnMouseButtonDown);
		RegisterHandler(WM_XBUTTONDOWN, &UIWindow::OnMouseButtonDown);

		RegisterHandler(WM_LBUTTONUP, &UIWindow::OnMouseButtonUp);
		RegisterHandler(WM_RBUTTONUP, &UIWindow::OnMouseButtonUp);
		RegisterHandler(WM_MBUTTONUP, &UIWindow::OnMouseButtonUp);
		RegisterHandler(WM_XBUTTONUP, &UIWindow::OnMouseButtonUp);

		RegisterHandler(WM_LBUTTONDBLCLK, &UIWindow::OnMouseDoubleClick);
		RegisterHandler(WM_RBUTTONDBLCLK, &UIWindow::OnMouseDoubleClick);
		RegisterHandler(WM_MBUTTONDBLCLK, &UIWindow::OnMouseDoubleClick);
		RegisterHandler(WM_XBUTTONDBLCLK, &UIWindow::OnMouseDoubleClick);
	}

	auto UIWindow::Draw(const Graphics graphics) -> void
	{
		BeginDraw();

		Render(graphics);
		childrenContainer.Render(graphics);

		EndDraw();
	}

	auto UIWindow::OnSizeChanged(const SizeL size) -> void
	{
		DirectXCompositionWindow::OnSizeChanged(size);
		childrenContainer.Resize(GetClientSize());
	}

	auto UIWindow::OnNCCreate(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		auto style = GetClassStyle();
		style |= CS_DBLCLKS;

		if (const auto result = SetClassLongPtrW(Hwnd(), GCL_STYLE, style);
			result != ERROR_SUCCESS)
		{
			if (const Error error{ GetLastError() };
				error.IsFailure())
			{
				Logger::Error(error, L"SetClassLongPtrW gave an error at UIWindow::OnNCCreate");
			}
		}

		childrenContainer.InvalidateRequest().AddCallback(
			[this](RawCUIElementPtr)
			{
				Invalidate();
			}
		);

		return 0;
	}

	auto UIWindow::OnFocusChanged(const UINT msg, WPARAM, LPARAM) const noexcept -> MessageHandlerResult
	{
		if (!focusedElement || (msg != WM_SETFOCUS && msg != WM_KILLFOCUS))
		{
			return 0;
		}

		UIEvent focusEvent;

		if (msg == WM_SETFOCUS)
		{
			focusEvent.type = EventType::GotFocus;
		}
		else if (msg == WM_KILLFOCUS)
		{
			focusEvent.type = EventType::LostFocus;
		}

		focusedElement->HandleEvent(focusEvent);

		return 0;
	}

	auto UIWindow::OnChar(const UINT msg, const WPARAM wParam,
	                      const LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		if (msg == WM_UNICHAR)
		{
			return wParam == UNICODE_NOCHAR;
		}

		if (!focusedElement)
		{
			return 0;
		}

		KeyboardEvent keyEvent;
		keyEvent.key = static_cast<wchar_t>(wParam);
		keyEvent.keyInfo = GetKeyInfoFromLparam(lParam);
		keyEvent.scanCode = LOBYTE(HIWORD(lParam));
		if (keyEvent.keyInfo.isExtended)
		{
			keyEvent.scanCode = static_cast<ScanCode>(MAKEWORD(keyEvent.scanCode, 0xE0));
		}
		if (keyEvent.keyInfo.isDown)
		{
			keyEvent.type = EventType::KeyDown;
		}
		else
		{
			keyEvent.type = EventType::KeyUp;
		}
		focusedElement->HandleEvent(keyEvent);

		return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnKey(const UINT msg, const WPARAM wParam,
	                     const LPARAM lParam) noexcept -> MessageHandlerResult
	{
		if (msg == WM_KEYDOWN && tabStopEnabled && wParam == VK_TAB)
		{
			std::size_t focusIndex = 0;

			if (focusedElement != nullptr)
			{
				if (const auto result = childrenContainer.GetElementLinearIndex(focusedElement);
					result.has_value())
				{
					focusIndex = result.value() + 1;
				}
			}

			focusIndex %= childrenContainer.GetLinearElementCount();

			const auto element = childrenContainer.GetElementAtLinearIndex(focusIndex);
			if (!element.has_value())
			{
				const auto firstElement = childrenContainer.GetElementAtLinearIndex(0);

				if (!firstElement.has_value())
				{
					return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
				}

				ChangeFocusedElement(firstElement.value());
				return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
			}

			ChangeFocusedElement(element.value());

			return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
		}

		if (!focusedElement || (msg != WM_KEYDOWN && msg != WM_KEYUP))
		{
			return 0;
		}

		KeyboardEvent keyEvent;
		keyEvent.type = EventType::KeyDown;
		keyEvent.key = static_cast<VirtualKeyCode>(wParam);
		keyEvent.keyInfo = GetKeyInfoFromLparam(lParam);
		keyEvent.scanCode = LOBYTE(HIWORD(lParam));
		if (keyEvent.keyInfo.isExtended)
		{
			keyEvent.scanCode = static_cast<ScanCode>(MAKEWORD(keyEvent.scanCode, 0xE0));
		}
		if (msg == WM_KEYUP)
		{
			keyEvent.type = EventType::KeyUp;
		}
		focusedElement->HandleEvent(keyEvent);

		return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnMouseMove(UINT, const WPARAM wParam, const LPARAM lParam) -> MessageHandlerResult
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = Hwnd();
		tme.dwHoverTime = HOVER_DEFAULT;

		if (const auto res = TrackMouseEvent(&tme);
			res == 0)
		{
			Logger::Error(
				Error{ GetLastError() },
				L"TrackMouseEvent gave an error at UIWindow::OnMouseMove");
		}

		const auto modifierKeys = GetModifierKeysFromWparam(wParam);
		const auto mouseButton = GetMouseButtonsFromWparam(wParam);
		const auto mousePos = GetMousePosFromLparam(lParam);

		if (!hoveredElement)
		{
			if (const auto result = GetChildren().GetElementAtPosition(mousePos);
				result.has_value())
			{
				MouseEvent mouseEnterEvent;
				hoveredElement = result.value();
				mouseEnterEvent.type = EventType::MouseEnter;
				mouseEnterEvent.position = mousePos;
				mouseEnterEvent.modifierKeys = modifierKeys;
				mouseEnterEvent.mouseButton = mouseButton;
				hoveredElement->HandleEvent(mouseEnterEvent);

				return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
			}
			return 0;
		}

		if (!hoveredElement->HitTest(mousePos))
		{
			MouseEvent mouseLeaveEvent;
			mouseLeaveEvent.type = EventType::MouseLeave;
			mouseLeaveEvent.position = mousePos;
			mouseLeaveEvent.modifierKeys = modifierKeys;
			mouseLeaveEvent.mouseButton = mouseButton;
			hoveredElement->HandleEvent(mouseLeaveEvent);
			hoveredElement = nullptr;
			return 0;
		}

		MouseEvent mouseMoveEvent;
		mouseMoveEvent.type = EventType::MouseMove;
		mouseMoveEvent.position = mousePos;
		mouseMoveEvent.modifierKeys = modifierKeys;
		mouseMoveEvent.mouseButton = mouseButton;
		hoveredElement->HandleEvent(mouseMoveEvent);

		return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnMouseLeave(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		if (!hoveredElement)
		{
			return 0;
		}

		MouseEvent mouseLeaveEvent;
		mouseLeaveEvent.type = EventType::MouseLeave;
		hoveredElement->HandleEvent(mouseLeaveEvent);

		hoveredElement = nullptr;

		return 0;
	}

	auto UIWindow::OnMouseHover(UINT, const WPARAM wParam, const LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		if (!hoveredElement)
		{
			return 0;
		}

		const auto modifierKeys = GetModifierKeysFromWparam(wParam);
		const auto mouseButton = GetMouseButtonsFromWparam(wParam);
		const auto mousePos = GetMousePosFromLparam(lParam);

		MouseEvent mouseHoverEvent;
		mouseHoverEvent.type = EventType::MouseHover;
		mouseHoverEvent.position = mousePos;
		mouseHoverEvent.modifierKeys = modifierKeys;
		mouseHoverEvent.mouseButton = mouseButton;
		hoveredElement->HandleEvent(mouseHoverEvent);

		return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnMouseWheel(UINT, const WPARAM wParam, const LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		if (!hoveredElement)
		{
			return 0;
		}

		const auto modifierKeys = GetModifierKeysFromWparam(LOWORD(wParam));
		const auto mouseButton = GetMouseButtonsFromWparam(LOWORD(wParam));
		const auto wheelDelta = GetMouseWheelDeltaFromWparam(wParam);
		const auto mousePos = GetMousePosFromLparam(lParam);

		MouseEvent mouseWheelEvent;
		mouseWheelEvent.type = EventType::MouseWheel;
		mouseWheelEvent.position = mousePos;
		mouseWheelEvent.modifierKeys = modifierKeys;
		mouseWheelEvent.mouseButton = mouseButton;
		mouseWheelEvent.wheelDelta = wheelDelta;
		hoveredElement->HandleEvent(mouseWheelEvent);

		return { 0, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnMouseButtonDown(const UINT msg, const WPARAM wParam,
	                                 const LPARAM lParam) noexcept -> MessageHandlerResult
	{
		const auto retVal = msg == WM_XBUTTONDOWN ? 1 : 0;

		const auto modifierKeys = GetModifierKeysFromWparam(wParam);
		const auto mouseButton = GetMouseButtonsFromWparam(wParam) | GetMouseButtonForMessage(msg, wParam);
		const auto mousePos = GetMousePosFromLparam(lParam);

		if (focusedElement && !focusedElement->HitTest(mousePos))
		{
			ChangeFocusedElement(nullptr);
		}

		if (hoveredElement != nullptr)
		{
			ChangeFocusedElement(hoveredElement);
		}
		if (!focusedElement)
		{
			return retVal;
		}

		MouseEvent mouseEvent;
		mouseEvent.type = EventType::MouseButtonDown;
		mouseEvent.position = mousePos;
		mouseEvent.modifierKeys = modifierKeys;
		mouseEvent.mouseButton = mouseButton;
		focusedElement->HandleEvent(mouseEvent);

		return { retVal, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnMouseButtonUp(const UINT msg, const WPARAM wParam,
	                               const LPARAM lParam) noexcept -> MessageHandlerResult
	{
		const auto retVal = msg == WM_XBUTTONUP ? 1 : 0;

		const auto modifierKeys = GetModifierKeysFromWparam(wParam);
		const auto mouseButton = GetMouseButtonsFromWparam(wParam) | GetMouseButtonForMessage(msg, wParam);
		const auto mousePos = GetMousePosFromLparam(lParam);

		if (hoveredElement != nullptr)
		{
			ChangeFocusedElement(hoveredElement);
		}
		if (!focusedElement)
		{
			return retVal;
		}

		MouseEvent mouseEvent;
		mouseEvent.type = EventType::MouseButtonUp;
		mouseEvent.position = mousePos;
		mouseEvent.modifierKeys = modifierKeys;
		mouseEvent.mouseButton = mouseButton;
		focusedElement->HandleEvent(mouseEvent);

		return { retVal, MessageHandlerReturnFlags::NoFurtherHandling };
	}

	auto UIWindow::OnMouseDoubleClick(const UINT msg, const WPARAM wParam,
	                                  const LPARAM lParam) noexcept -> MessageHandlerResult
	{
		const auto retVal = msg == WM_XBUTTONDBLCLK ? 1 : 0;

		const auto modifierKeys = GetModifierKeysFromWparam(wParam);
		const auto mouseButton = GetMouseButtonsFromWparam(wParam) | GetMouseButtonForMessage(msg, wParam);
		const auto mousePos = GetMousePosFromLparam(lParam);

		if (focusedElement && !focusedElement->HitTest(mousePos))
		{
			ChangeFocusedElement(nullptr);
		}

		if (hoveredElement != nullptr)
		{
			ChangeFocusedElement(hoveredElement);
		}
		if (!focusedElement)
		{
			return retVal;
		}

		MouseEvent mouseEvent;
		mouseEvent.type = EventType::MouseDoubleClick;
		mouseEvent.position = mousePos;
		mouseEvent.modifierKeys = modifierKeys;
		mouseEvent.mouseButton = mouseButton;
		focusedElement->HandleEvent(mouseEvent);

		return { retVal, MessageHandlerReturnFlags::NoFurtherHandling };
	}
}

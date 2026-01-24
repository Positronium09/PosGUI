module;
#include <Windows.h>

export module PGUI.UI.UIBase:UIWindow;

import std;

import :Interface;
import :UIElement;
import :UIContainer;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	class UIWindow : public DirectXCompositionWindow
	{
		public:
		UIWindow();
		explicit UIWindow(const WindowClassPtr&);

		[[nodiscard]] auto& GetChildren() noexcept
		{
			return childrenContainer;
		}
		[[nodiscard]] const auto& GetChildren() const noexcept
		{
			return childrenContainer;
		}

		auto EnableTabStop() noexcept
		{
			tabStopEnabled = true;
		}
		auto DisableTabStop() noexcept
		{
			tabStopEnabled = false;
		}
		[[nodiscard]] auto IsTabStopEnabled() const noexcept
		{
			return tabStopEnabled;
		}

		[[nodiscard]] auto GetFocusedElement() const noexcept
		{
			return focusedElement;
		}
		[[nodiscard]] auto GetHoveredElement() const noexcept
		{
			return hoveredElement;
		}

		[[nodiscard]] auto LockFocusedElement() const noexcept
		{
			return focusedElement.lock();
		}
		[[nodiscard]] auto LockHoveredElement() const noexcept
		{
			return hoveredElement.lock();
		}

		protected:
		virtual auto Render(const Graphics&) -> void
		{
			/*  */
		}

		private:
		bool tabStopEnabled = false;
		UIContainer childrenContainer;
		//If focused or hovered elements are destroyed crash due to dangling pointer
		//TODO Fix it when it happens prolly use weak_ptr but too lazy rn

		WeakUIElementPtr focusedElement;
		WeakUIElementPtr hoveredElement;

		[[nodiscard]] static auto GetMousePosFromLparam(const LPARAM lParam) noexcept -> PointF
		{
			return MAKEPOINTS(lParam);
		}
		[[nodiscard]] static auto GetMouseButtonsFromWparam(const WPARAM wParam) noexcept
		{
			auto mouseButton = MouseButton::None;
			mouseButton |= (wParam & MK_LBUTTON) != 0 ? MouseButton::Left : MouseButton::None;
			mouseButton |= (wParam & MK_MBUTTON) != 0 ? MouseButton::Middle : MouseButton::None;
			mouseButton |= (wParam & MK_RBUTTON) != 0 ? MouseButton::Right : MouseButton::None;
			mouseButton |= (wParam & MK_XBUTTON1) != 0 ? MouseButton::XButton1 : MouseButton::None;
			mouseButton |= (wParam & MK_XBUTTON2) != 0 ? MouseButton::XButton2 : MouseButton::None;

			return mouseButton;
		}
		[[nodiscard]] static auto GetModifierKeysFromWparam(const WPARAM wParam) noexcept -> ModifierKeys
		{
			ModifierKeys modifierKeys;
			modifierKeys.shift = (wParam & MK_SHIFT) != 0;
			modifierKeys.control = (wParam & MK_CONTROL) != 0;
			modifierKeys.alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
			modifierKeys.super = (GetKeyState(VK_LWIN) & 0x8000) != 0 || (GetKeyState(VK_RWIN) & 0x8000) != 0;
			return modifierKeys;
		}
		[[nodiscard]] static auto GetMouseWheelDeltaFromWparam(const WPARAM wParam) noexcept
		{
			return static_cast<int>(HIWORD(wParam));
		}
		[[nodiscard]] static auto GetKeyInfoFromLparam(const LPARAM lParam) noexcept
		{
			const auto keyFlags = HIWORD(lParam);

			KeyInfo keyInfo;
			keyInfo.repeatCount = LOWORD(lParam);
			keyInfo.isAltPressed = (keyFlags & KF_ALTDOWN) != 0;
			keyInfo.isExtended = (keyFlags & KF_EXTENDED) != 0;
			keyInfo.wasDown = (keyFlags & KF_REPEAT) == KF_REPEAT;
			keyInfo.isDown = (keyFlags & KF_UP) == KF_UP;

			return keyInfo;
		}
		[[nodiscard]] static auto GetMouseButtonForMessage(const UINT msg, const WPARAM wParam) noexcept
		{
			auto mouseButton = MouseButton::None;

			switch (msg)
			{
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_LBUTTONDBLCLK:
				{
					mouseButton |= MouseButton::Left;
					break;
				}
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_RBUTTONDBLCLK:
				{
					mouseButton |= MouseButton::Right;
					break;
				}
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
				case WM_MBUTTONDBLCLK:
				{
					mouseButton |= MouseButton::Middle;
					break;
				}
				case WM_XBUTTONDOWN:
				case WM_XBUTTONUP:
				case WM_XBUTTONDBLCLK:
				{
					if (HIWORD(wParam) == XBUTTON1)
					{
						mouseButton |= MouseButton::XButton1;
					}
					else if (HIWORD(wParam) == XBUTTON2)
					{
						mouseButton |= MouseButton::XButton2;
					}
					break;
				}
				default: ;
			}
			return mouseButton;
		}

		auto ChangeFocusedElement(const RawUIElementPtr newFocused) noexcept -> void
		{
			if (!newFocused)
			{
				if (const auto focused = LockFocusedElement();
					focused != nullptr)
				{
					focused->RemoveFocus();
					focusedElement.reset();
				}
				return;
			}

			if (!newFocused->IsFocusable() || !*newFocused->IsEnabled())
			{
				return;
			}

			const auto focused = LockFocusedElement();
			if (focused.get() == newFocused)
			{
				return;
			}
			if (!focusedElement.expired())
			{
				focused->RemoveFocus();
			}
			if (newFocused)
			{
				newFocused->SetFocus();
			}

			focusedElement = newFocused->weak_from_this();
		}

		auto Draw(Graphics graphics) -> void override;
		auto OnSizeChanged(SizeL) -> void override;

		auto OnNCCreate(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;

		auto OnFocusChanged(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;

		auto OnChar(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;
		auto OnKey(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;

		// ReSharper disable CommentTypo
		// TODO Maybe implement WM_SYSKEYDOWN, WM_SYSKEYUP, WM_SYSDEADCHAR and WM_HOTKEY
		// ReSharper restore CommentTypo

		auto OnMouseMove(UINT, WPARAM, LPARAM) -> MessageHandlerResult;
		auto OnMouseLeave(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnMouseHover(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;
		auto OnMouseWheel(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;
		auto OnMouseButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnMouseButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnMouseDoubleClick(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
	};
}

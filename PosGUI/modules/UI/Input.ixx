module;
#include <Windows.h>

export module PGUI.UI.Input;

import PGUI.Shape2D;

export namespace PGUI::UI
{
	enum class MouseButton
	{
		None,
		Left = 1,
		Middle = 2,
		Right = 4,
		XButton1 = 8,
		XButton2 = 16
	};
	DEFINE_ENUM_FLAG_OPERATORS(MouseButton);

	constexpr auto MOUSEWHEEL_DELTA = WHEEL_DELTA;

	struct ModifierKeys
	{
		bool shift : 1 = false;
		bool control : 1 = false;
		bool alt : 1 = false;
		bool super : 1 = false;
	};

	constexpr auto Releasing = false;
	constexpr auto Pressing = true;

	struct KeyInfo
	{
		bool isDown : 1 = false;
		bool wasDown : 1 = false;
		bool isExtended : 1 = false;
		bool isAltPressed : 1 = false;
		bool transitionState : 1 = Releasing;
		short repeatCount = 0;
	};

	[[nodiscard]] auto GetMousePosFromLparam(const LPARAM lParam) noexcept -> PointF
	{
		return MAKEPOINTS(lParam);
	}
	[[nodiscard]] auto GetMouseButtonsFromWparam(const WPARAM wParam) noexcept
	{
		auto mouseButton = MouseButton::None;
		mouseButton |= (wParam & MK_LBUTTON) != 0 ? MouseButton::Left : MouseButton::None;
		mouseButton |= (wParam & MK_MBUTTON) != 0 ? MouseButton::Middle : MouseButton::None;
		mouseButton |= (wParam & MK_RBUTTON) != 0 ? MouseButton::Right : MouseButton::None;
		mouseButton |= (wParam & MK_XBUTTON1) != 0 ? MouseButton::XButton1 : MouseButton::None;
		mouseButton |= (wParam & MK_XBUTTON2) != 0 ? MouseButton::XButton2 : MouseButton::None;

		return mouseButton;
	}
	[[nodiscard]] auto GetModifierKeysFromWparam(const WPARAM wParam) noexcept -> ModifierKeys
	{
		ModifierKeys modifierKeys;
		modifierKeys.shift = (wParam & MK_SHIFT) != 0;
		modifierKeys.control = (wParam & MK_CONTROL) != 0;
		modifierKeys.alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
		modifierKeys.super = (GetKeyState(VK_LWIN) & 0x8000) != 0 || (GetKeyState(VK_RWIN) & 0x8000) != 0;
		return modifierKeys;
	}
	[[nodiscard]] auto GetMouseWheelDeltaFromWparam(const WPARAM wParam) noexcept
	{
		return static_cast<int>(HIWORD(wParam));
	}
	[[nodiscard]] auto GetKeyInfoFromLparam(const LPARAM lParam) noexcept
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
	[[nodiscard]] auto GetMouseButtonForMessage(const UINT msg, const WPARAM wParam) noexcept
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
			default:;
		}
		return mouseButton;
	}
}
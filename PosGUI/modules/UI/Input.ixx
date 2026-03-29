module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module PGUI.UI.Input;

import PGUI.Shape2D;
import PGUI.Utils;

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

	enum class KeyFlags
	{
		Extended = KF_EXTENDED,
		DlgMode = KF_DLGMODE,
		MenuMode = KF_MENUMODE,
		AltDown = KF_ALTDOWN,
		Repeat = KF_REPEAT,
		Up = KF_UP
	};
	DEFINE_ENUM_FLAG_OPERATORS(KeyFlags);

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

	[[nodiscard]] auto GetMousePosFromLparam(const Argument2 arg2) noexcept -> PointF
	{
		return MAKEPOINTS(arg2);
	}
	[[nodiscard]] auto GetMouseButtonsFromWparam(const Argument1 arg1) noexcept
	{
		auto mouseButton = MouseButton::None;
		mouseButton |= (arg1 & MK_LBUTTON) != 0 ? MouseButton::Left : MouseButton::None;
		mouseButton |= (arg1 & MK_MBUTTON) != 0 ? MouseButton::Middle : MouseButton::None;
		mouseButton |= (arg1 & MK_RBUTTON) != 0 ? MouseButton::Right : MouseButton::None;
		mouseButton |= (arg1 & MK_XBUTTON1) != 0 ? MouseButton::XButton1 : MouseButton::None;
		mouseButton |= (arg1 & MK_XBUTTON2) != 0 ? MouseButton::XButton2 : MouseButton::None;

		return mouseButton;
	}
	[[nodiscard]] auto GetModifierKeysFromWparam(const Argument1 arg1) noexcept -> ModifierKeys
	{
		ModifierKeys modifierKeys;
		modifierKeys.shift = (arg1 & MK_SHIFT) != 0;
		modifierKeys.control = (arg1 & MK_CONTROL) != 0;
		modifierKeys.alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
		modifierKeys.super = (GetKeyState(VK_LWIN) & 0x8000) != 0 || (GetKeyState(VK_RWIN) & 0x8000) != 0;
		return modifierKeys;
	}
	[[nodiscard]] auto GetMouseWheelDeltaFromWparam(const Argument1 arg1) noexcept
	{
		return static_cast<int>(static_cast<short>(HIWORD(arg1)));
	}
	[[nodiscard]] auto GetKeyInfoFromLparam(const Argument2 arg2) noexcept
	{
		const auto keyFlags = FromUnderlying<KeyFlags>(HIWORD(arg2));

		KeyInfo keyInfo;
		keyInfo.repeatCount = LOWORD(arg2);
		keyInfo.isAltPressed = IsFlagSet(keyFlags, KeyFlags::AltDown);
		keyInfo.isExtended = IsFlagSet(keyFlags, KeyFlags::Extended);
		keyInfo.wasDown = IsFlagSet(keyFlags, KeyFlags::Repeat);
		keyInfo.isDown = !IsFlagSet(keyFlags, KeyFlags::Up);

		return keyInfo;
	}
	[[nodiscard]] auto GetMouseButtonForMessage(const MessageID msg, const Argument1 arg1) noexcept
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
				if (HIWORD(arg1) == XBUTTON1)
				{
					mouseButton |= MouseButton::XButton1;
				}
				else if (HIWORD(arg1) == XBUTTON2)
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
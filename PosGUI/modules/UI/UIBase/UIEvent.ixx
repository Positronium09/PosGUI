module;
#include <Windows.h>

export module PGUI.UI.UIBase:UIEvent;

import std;
import PGUI.Shape2D;

export namespace PGUI::UI
{
	using ScanCode = char;
	using VirtualKeyCode = DWORD;

	enum class EventType
	{
		None,
		MouseEnter,
		MouseLeave,
		MouseHover,
		MouseMove,
		MouseButtonDown,
		MouseButtonUp,
		MouseDoubleClick,
		MouseWheel,
		KeyDown,
		KeyUp,
		GotFocus,
		LostFocus,
	};

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

	struct UIEvent
	{
		EventType type;
		bool handled = false;
		std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
	};

	struct MouseEvent : UIEvent
	{
		PointF position;
		int wheelDelta = 0;
		MouseButton mouseButton = MouseButton::None;
		ModifierKeys modifierKeys;
		bool doubleClick = false;
	};

	struct KeyboardEvent : UIEvent
	{
		std::variant<VirtualKeyCode, wchar_t> key = L'\0';
		KeyInfo keyInfo;
		ScanCode scanCode = 0;
	};
}

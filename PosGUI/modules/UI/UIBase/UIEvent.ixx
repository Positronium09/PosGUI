export module PGUI.UI.UIBase:UIEvent;

import std;
import PGUI.Shape2D;

export namespace PGUI::UI
{
	enum class EventType
	{
		None,
		MouseEnter,
		MouseLeave,
		MouseMove,
		MouseDown,
		MouseUp,
		MouseCancel,
		MouseWheel,
		KeyDown,
		KeyUp,
		GotFocus,
		LostFocus,
	};

	enum class MouseButton
	{
		None,
		Left,
		Middle,
		Right,
		XButton1,
		XButton2
	};

	constexpr auto WHEEL_DELTA = 120;

	struct ModifierKeys
	{
		bool shift : 1 = false;
		bool control : 1 = false;
		bool alt : 1 = false;
		bool super : 1 = false;
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
}

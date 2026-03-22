export module PGUI.UI.UICore:UIEvent;

import std;

import PGUI.Shape2D;
import PGUI.Utils;
import PGUI.UI.Input;
import PGUI.ErrorHandling;

export namespace PGUI::UI
{
	using ScanCode = std::uint32_t;
	using CharacterCode = wchar_t;

	enum class EventType
	{
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
		FocusGained,
		FocusLost,
		Moved,
		Resized
	};

	struct MouseEventData
	{
		PointF position;
		MouseButton button;
		ModifierKeys modifierKeys;
		int wheelDelta = 0;
	};

	struct KeyEventData
	{
		std::variant<CharacterCode, ScanCode> key = L'\0';
		ScanCode scanCode = 0;
		ModifierKeys modifierKeys;
		KeyInfo keyInfo;
	};

	struct MovedEventData
	{
		PointF oldPosition;
		PointF newPosition;
	};

	struct ResizedEventData
	{
		SizeF oldSize;
		SizeF newSize;
	};

	struct NoData
	{
	};

	using EventDataTypes = TypeList<
		MouseEventData, 
		KeyEventData,
		MovedEventData,
		ResizedEventData,
		NoData>;

	using EventData = EventDataTypes::Rebind<std::variant>;

	struct UIEvent
	{
		EventType type;
		EventData data;

		auto MarkAsHandled() noexcept -> void
		{
			handled = true;
		}

		[[nodiscard]] auto IsHandled() const noexcept
		{
			return handled;
		}

		[[nodiscard]] auto TimeStamp() const noexcept
		{
			return timestamp;
		}

		[[nodiscard]] auto ElapsedTime() const noexcept
		{
			return std::chrono::steady_clock::now() - timestamp;
		}

		template <typename T> requires EventDataTypes::Contains<T>
		[[nodiscard]] auto GetDataAs() -> Result<T>
		{
			if (std::holds_alternative<T>(data))
			{
				return std::get<T>(data);
			}

			return Unexpected{ ErrorCode::InvalidCast };
		}

		private:
		bool handled = false;
		std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now();
	};
}

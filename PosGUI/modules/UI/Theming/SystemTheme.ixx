module;
#include <Windows.h>
#include <winrt/windows.ui.viewmanagement.h>

export module PGUI.UI.Theming.SystemTheme;

import PGUI.UI.Color;
import PGUI.Event;

namespace PGUI
{
	void Init();
}

export namespace PGUI::UI::Theming
{
	enum class ColorType
	{
		Background = winrt::Windows::UI::ViewManagement::UIColorType::Background,
		Foreground = winrt::Windows::UI::ViewManagement::UIColorType::Foreground,
		AccentDark3 = winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3,
		AccentDark2 = winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2,
		AccentDark1 = winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1,
		Accent = winrt::Windows::UI::ViewManagement::UIColorType::Accent,
		AccentLight1 = winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1,
		AccentLight2 = winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2,
		AccentLight3 = winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3
	};

	class SystemTheme
	{
		friend void PGUI::Init();
		
		public:
		SystemTheme() = delete;

		[[nodiscard]] static auto& ColorValuesChanged() noexcept { return colorValuesChangedEvent; }

		[[nodiscard]] static auto IsDarkMode() -> bool;
		[[nodiscard]] static auto IsLightMode() { return !IsDarkMode(); }

		[[nodiscard]] static auto GetColor(ColorType colorType) -> RGBA;
		[[nodiscard]] static auto GetTextColor()
		{
			return GetColor(ColorType::Foreground);
		}
		[[nodiscard]] static auto GetBackgroundColor()
		{
			return GetColor(ColorType::Background);
		}
		[[nodiscard]] static auto GetForegroundColor()
		{
			return GetColor(ColorType::Foreground);
		}
		[[nodiscard]] static auto GetAccentDark3Color()
		{
			return GetColor(ColorType::AccentDark3);
		}
		[[nodiscard]] static auto GetAccentDark2Color()
		{
			return GetColor(ColorType::AccentDark2);
		}
		[[nodiscard]] static auto GetAccentDark1Color()
		{
			return GetColor(ColorType::AccentDark1);
		}
		[[nodiscard]] static auto GetAccentColor()
		{
			return GetColor(ColorType::Accent);
		}
		[[nodiscard]] static auto GetAccentLight3Color()
		{
			return GetColor(ColorType::AccentLight3);
		}
		[[nodiscard]] static auto GetAccentLight2Color()
		{
			return GetColor(ColorType::AccentLight2);
		}
		[[nodiscard]] static auto GetAccentLight1Color()
		{
			return GetColor(ColorType::AccentLight1);
		}

		private:
		inline static winrt::Windows::UI::ViewManagement::UISettings uiSettings;
		inline static Event<> colorValuesChangedEvent;
	};
}

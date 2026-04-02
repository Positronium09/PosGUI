module PGUI.UI.Theming.Theme;

import std;

import PGUI.Event;
import PGUI.UI.Color;
import PGUI.UI.Colors;
import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.SystemTheme;

namespace PGUI::UI::Theming
{
	auto ThemeContext::ChangeCurrentTheme(const Theme& theme) noexcept -> void
	{
		{
			std::scoped_lock lock{ currentThemeMutex };
			currentTheme = theme;
		}
		themeChangedEvent.Invoke(GetCurrentTheme());
	}

	auto ThemeContext::InitializeThemes() noexcept -> void
	{
		static std::once_flag initializedFlag;

		std::call_once(initializedFlag, []
		{
			RefreshThemes();
		});
	}

	auto ThemeContext::OnSystemThemeChanged() noexcept -> void
	{
		if (respondToSystemThemeChange && IsCurrentThemeBuiltin())
		{
			RefreshThemes();
			if (SystemTheme::IsDarkMode())
			{
				ChangeCurrentTheme(GetDarkTheme().Get());
			}
			else
			{
				ChangeCurrentTheme(GetLightTheme().Get());
			}
		}
	}

	auto ThemeContext::RefreshThemes() noexcept -> void
	{
		std::scoped_lock lock{ darkThemeMutex, lightThemeMutex };

		using namespace Colors;

		DarkTheme.colorContext = {
			.text = SystemTheme::GetTextColor().value_or(White),
			.background = SystemTheme::GetBackgroundColor().value_or(RGBA{ 0x1E1E1E }),
			.primary = SystemTheme::GetAccentLight1Color().value_or(RGBA{ 0x60CDFF }),
			.secondary = RGBA{ 0x1b1b1b },
			.accent = SystemTheme::GetAccentColor().value_or(RGBA{ 0x0078D4 })
		};
		DarkTheme.appWindowStyle = {
			.borderColor = Transparent,
			.captionColor = Transparent,
			.captionTextColor = Transparent,
			.darkMode = true,
			.cornerPreference = CornerPreference::Default
		};

		LightTheme.colorContext = {
			.text = SystemTheme::GetTextColor().value_or(Black),
			.background = SystemTheme::GetBackgroundColor().value_or(RGBA{ 0xF3F3F3 }),
			.primary = SystemTheme::GetAccentDark1Color().value_or(RGBA{ 0x005A9E }),
			.secondary = AntiqueWhite,
			.accent = SystemTheme::GetAccentColor().value_or(RGBA{ 0x0078D4 })
		};
		LightTheme.appWindowStyle = {
			.borderColor = Transparent,
			.captionColor = Transparent,
			.captionTextColor = Transparent,
			.darkMode = false,
			.cornerPreference = CornerPreference::Default
		};
	}
}

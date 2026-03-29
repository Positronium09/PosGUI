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
		static auto initialized = false;
		if (initialized)
		{
			return;
		}

		RefreshThemes();

		initialized = true;
	}

	auto ThemeContext::OnSystemThemeChanged() -> void
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

		DarkTheme.colorContext = {
			.text = SystemTheme::GetTextColor(),
			.background = SystemTheme::GetBackgroundColor(),
			.primary = SystemTheme::GetAccentLight1Color(),
			.secondary = RGBA{ 0x1b1b1b },
			.accent = SystemTheme::GetAccentColor()
		};
		DarkTheme.appWindowStyle = {
			.borderColor = Colors::Transparent,
			.captionColor = Colors::Transparent,
			.captionTextColor = Colors::Transparent,
			.darkMode = true,
			.cornerPreference = CornerPreference::Default
		};

		LightTheme.colorContext = {
			.text = SystemTheme::GetTextColor(),
			.background = SystemTheme::GetBackgroundColor(),
			.primary = SystemTheme::GetAccentDark1Color(),
			.secondary = Colors::AntiqueWhite,
			.accent = SystemTheme::GetAccentColor()
		};
		LightTheme.appWindowStyle = {
			.borderColor = Colors::Transparent,
			.captionColor = Colors::Transparent,
			.captionTextColor = Colors::Transparent,
			.darkMode = false,
			.cornerPreference = CornerPreference::Default
		};
	}
}

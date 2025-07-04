module PGUI.UI.Theming.Theme;

import std;

import PGUI.Event;
import PGUI.UI.Color;
import PGUI.UI.Colors;
import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.SystemTheme;

namespace PGUI::UI::Theming
{
	auto ThemeContext::GetCurrentTheme() noexcept -> const Theme&
	{
		std::scoped_lock lock{ themeMutex };
		return currentTheme;
	}

	auto ThemeContext::ChangeCurrentTheme(const Theme& theme) noexcept -> void
	{
		{
			std::scoped_lock lock{ themeMutex };
			currentTheme = theme;
		}
		themeChangedEvent.Invoke(currentTheme);
	}

	auto ThemeContext::InitializeThemes() noexcept -> void
	{
		static auto initialized = false;
		if (initialized)
		{
			return;
		}

		DarkTheme = Theme{
			.colorContext{
				.text = SystemTheme::GetTextColor(),
				.background = SystemTheme::GetBackgroundColor(),
				.primary = SystemTheme::GetAccentLight1Color(),
				.secondary = RGBA{ 0x1b1b1b },
				.accent = SystemTheme::GetAccentColor()
			},
			.appWindowStyle{
				.borderColor = Colors::Transparent,
				.captionColor = Colors::Transparent,
				.captionTextColor = Colors::Transparent,
				.darkMode = true,
				.cornerPreference = Default
			}
		};

		LightTheme = Theme{
			.colorContext{
				.text = SystemTheme::GetTextColor(),
				.background = SystemTheme::GetBackgroundColor(),
				.primary = SystemTheme::GetAccentDark1Color(),
				.secondary = Colors::AntiqueWhite,
				.accent = SystemTheme::GetAccentColor()
			},
			.appWindowStyle{
				.borderColor = Colors::Transparent,
				.captionColor = Colors::Transparent,
				.captionTextColor = Colors::Transparent,
				.darkMode = false,
				.cornerPreference = Default
			}
		};
	}

	auto ThemeContext::OnSystemThemeChanged() -> void
	{
		if (respondToSystemThemeChange &&
		    (currentTheme == DarkTheme || currentTheme == LightTheme))
		{
			InitializeThemes();
			currentTheme = SystemTheme::IsDarkMode() ? DarkTheme : LightTheme;
			themeChangedEvent.Invoke(currentTheme);
		}
	}
}

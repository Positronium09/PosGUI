module;
#include <any>
#include <mutex>
#include <typeindex>
#include <unordered_map>

module PGUI.UI.Theming.Theme;

import PGUI.Event;
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
	void ThemeContext::ChangeCurrentTheme(const Theme& theme) noexcept
	{
		{
			std::scoped_lock lock{ themeMutex };
			currentTheme = theme;
		}
		themeChangedEvent.Invoke(currentTheme);
	}

	void ThemeContext::InitializeThemes() noexcept
	{
		DarkTheme = Theme{
			.appWindowStyle{
				.borderColor = Colors::Transparent,
				.captionColor = Colors::Transparent,
				.captionTextColor = Colors::Transparent,
				.darkMode = true,
				.cornerPreference = CornerPreference::Default
			}
		};

		LightTheme = Theme{
			.appWindowStyle{
				.borderColor = Colors::Transparent,
				.captionColor = Colors::Transparent,
				.captionTextColor = Colors::Transparent,
				.darkMode = false,
				.cornerPreference = CornerPreference::Default
			}
		};
	}

	void ThemeContext::OnSystemThemeChanged()
	{
		if (respondToSystemThemeChange && 
			(&currentTheme == &DarkTheme || &currentTheme == &DarkTheme))
		{
			InitializeThemes();
			currentTheme = SystemTheme::IsDarkMode() ? DarkTheme : LightTheme;
			themeChangedEvent.Invoke(currentTheme);
		}
	}
}

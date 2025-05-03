module;
#include <any>
#include <mutex>
#include <typeindex>
#include <unordered_map>

module PGUI.UI.Theming.Theme;

import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.SystemTheme;
import PGUI.Event;

namespace PGUI::UI::Theming
{
	const auto& ThemeContext::GetCurrentTheme() noexcept
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
			(&currentTheme == &DarkTheme || &currentTheme == &LightTheme))
		{
			InitializeThemes();
			currentTheme = SystemTheme::IsDarkMode() ? DarkTheme : LightTheme;
			themeChangedEvent.Invoke(currentTheme);
		}
	}
}

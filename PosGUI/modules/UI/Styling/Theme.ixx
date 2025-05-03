module;
#include <any>
#include <typeindex>
#include <unordered_map>

export module PGUI.UI.Styling.Theme;

import PGUI.UI.Styling.Styles;
import PGUI.UI.SystemTheme;
import PGUI.Event;

namespace PGUI
{
	void Init();
}

export namespace PGUI::UI::Styling
{
	struct Theme
	{
		AppWindowStyle appWindowStyle{ };
		std::unordered_map<std::type_index, std::any> customStyles{ };

		template <typename T>
		void AddCustomStyle(const T& style) noexcept
		{
			customStyles[typeid(T)] = style;
		}
		template <typename T>
		[[nodiscard]] const auto& GetCustomStyle() const
		{
			return std::any_cast<const T&>(customStyles.at(typeid(T)));
		}
		template <typename T>
		[[nodiscard]] auto HasCustomStyle() const noexcept
		{
			return customStyles.contains(typeid(T));
		}
		template <typename T>
		void RemoveCustomStyle() noexcept
		{
			customStyles.erase(typeid(T));
		}
	};

	class ThemeContext
	{
		friend void PGUI::Init();

		public:
		inline static Theme DarkTheme;
		inline static Theme LightTheme;

		ThemeContext() = delete;

		[[nodiscard]] static const auto& GetCurrentTheme() noexcept { return currentTheme; }
		static void ChangeCurrentTheme(const Theme& theme) noexcept
		{
			currentTheme = theme;
			themeChangedEvent.Invoke(currentTheme);
		}

		[[nodiscard]] static auto& ThemeChangedEvent() noexcept { return themeChangedEvent; }

		[[nodiscard]] static auto& RespondToSystemThemeChange() noexcept { return respondToSystemThemeChange; }

		private:
		inline static bool respondToSystemThemeChange = true;
		inline static Theme currentTheme =
			PGUI::UI::SystemTheme::IsDarkMode() ? DarkTheme : LightTheme;
		inline static Event<const Theme&> themeChangedEvent;

		static void InitializeThemes() noexcept
		{
			DarkTheme = Theme {
				.appWindowStyle{
					.borderColor = Colors::Transparent,
					.captionColor = Colors::Transparent,
					.captionTextColor = Colors::Transparent,
					.darkMode = true,
					.cornerPreference = CornerPreference::Default
				}
			};

			LightTheme = Theme {
				.appWindowStyle{
					.borderColor = Colors::Transparent,
					.captionColor = Colors::Transparent,
					.captionTextColor = Colors::Transparent,
					.darkMode = false,
					.cornerPreference = CornerPreference::Default
				}
			};
		}

		static void OnSystemThemeChanged()
		{
			if (respondToSystemThemeChange)
			{
				InitializeThemes();
				currentTheme = PGUI::UI::SystemTheme::IsDarkMode() ? DarkTheme : LightTheme;
				themeChangedEvent.Invoke(currentTheme);
			}
		}
	};
}

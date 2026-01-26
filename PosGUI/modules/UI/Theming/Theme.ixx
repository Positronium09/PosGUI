module;
#include <ranges>

export module PGUI.UI.Theming.Theme;

import std;

import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.ColorContext;
import PGUI.UI.Theming.SystemTheme;
import PGUI.Event;
import PGUI.Mutex;

export namespace PGUI::UI::Theming
{
	struct Theme
	{
		ColorContext colorContext;
		AppWindowStyle appWindowStyle{ };
		std::unordered_map<std::type_index, std::any> customStyles{ };

		template <typename T>
		auto AddCustomStyle(const T& style) noexcept -> void
		{
			customStyles[typeid(T)] = style;
		}

		template <typename T>
		[[nodiscard]] const auto& GetCustomStyle() const
		{
			if (!customStyles.contains(typeid(T)))
			{
				throw std::runtime_error("Custom style not found");
			}
			return std::any_cast<const T&>(customStyles.at(typeid(T)));
		}

		template <typename T>
		[[nodiscard]] auto HasCustomStyle() const noexcept
		{
			return customStyles.contains(typeid(T));
		}

		template <typename T>
		auto RemoveCustomStyle() noexcept -> void
		{
			customStyles.erase(typeid(T));
		}

		auto operator==(const Theme& other) const noexcept -> bool
		{
			for (const auto& key : customStyles | std::views::keys)
			{
				if (!other.customStyles.contains(key))
				{
					return false;
				}
			}

			return colorContext == other.colorContext &&
			       appWindowStyle == other.appWindowStyle;
		}
	};

	class ThemeContext final
	{
		public:
		inline static Theme DarkTheme;
		inline static Theme LightTheme;

		ThemeContext() = delete;

		[[nodiscard]] static auto GetCurrentTheme() noexcept -> const Theme&;

		static auto ChangeCurrentTheme(const Theme& theme) noexcept -> void;

		[[nodiscard]] static auto& ThemeChangedEvent() noexcept { return themeChangedEvent; }

		[[nodiscard]] static auto& RespondToSystemThemeChange() noexcept { return respondToSystemThemeChange; }

		template <typename Function>
		static auto WithCurrentTheme(const Function& function) noexcept -> void requires std::invocable<
			Function, const Theme&>
		{
			std::scoped_lock lock{ themeMutex };
			function(currentTheme);
		}

		static auto InitializeThemes() noexcept -> void;

		static auto OnSystemThemeChanged() -> void;

		private:
		inline static std::atomic_bool respondToSystemThemeChange = true;
		inline static Theme currentTheme{ };
		inline static Event<const Theme&> themeChangedEvent;
		inline static Mutex::CSMutex themeMutex{ };
	};
}

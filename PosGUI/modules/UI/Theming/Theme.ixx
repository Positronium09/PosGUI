module;
#include <any>
#include <mutex>
#include <atomic>
#include <type_traits>
#include <concepts>
#include <typeindex>
#include <unordered_map>

export module PGUI.UI.Theming.Theme;

import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.ColorContext;
import PGUI.UI.Theming.SystemTheme;
import PGUI.Event;

namespace PGUI
{
	void Init();
}

export namespace PGUI::UI::Theming
{
	struct Theme
	{
		ColorContext colorContext;
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

		[[nodiscard]] static auto GetCurrentTheme() noexcept -> const Theme&;
		static void ChangeCurrentTheme(const Theme& theme) noexcept;

		[[nodiscard]] static auto& ThemeChangedEvent() noexcept { return themeChangedEvent; }

		[[nodiscard]] static auto& RespondToSystemThemeChange() noexcept { return respondToSystemThemeChange; }

		template <typename Function>
		static void WithCurrentTheme(const Function& function) noexcept
			requires std::invocable<Function, const Theme&>
		{
			std::scoped_lock lock{ themeMutex };
			function(currentTheme);
		}

		private:
		inline static std::atomic_bool respondToSystemThemeChange = true;
		inline static Theme currentTheme =
			SystemTheme::IsDarkMode() ? DarkTheme : LightTheme;
		inline static Event<const Theme&> themeChangedEvent;
		inline static std::mutex themeMutex{ };

		static void InitializeThemes() noexcept;

		static void OnSystemThemeChanged();
	};
}

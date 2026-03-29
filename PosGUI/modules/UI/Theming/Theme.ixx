module;
#include <ranges>

export module PGUI.UI.Theming.Theme;

import std;

import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.ColorContext;
import PGUI.UI.Theming.SystemTheme;
import PGUI.ErrorHandling;
import PGUI.Event;
import PGUI.Mutex;
import PGUI.Utils;

export namespace PGUI::UI::Theming
{
	struct Theme
	{
		ColorContext colorContext;
		AppWindowStyle appWindowStyle{ };
		std::unordered_map<std::type_index, std::any> customStyles{ };

		template <typename T>
		auto AddCustomStyle(const T& style) noexcept -> Error
		{
			try
			{
				customStyles.insert_or_assign(typeid(T), style);
			}
			catch (const std::bad_any_cast&)
			{
				return Error{ ErrorCode::InvalidCast }.SetCustomMessage(L"Failed to add custom style due to bad any cast");
			}
			catch (const std::exception& e)
			{
				return Error{ SystemErrorCode::STLFailure  }.SetCustomMessage(StringToWString(e.what()));
			}

			return Error{ ErrorCode::Success };
		}

		template <typename T>
		[[nodiscard]] auto GetCustomStyle() const -> Result<std::reference_wrapper<const T>>
		{
			const auto it = customStyles.find(typeid(T));
			if (it == customStyles.end())
			{
				return Unexpected{
					Error{ ErrorCode::NotFound }.SetCustomMessage(L"Custom style not found")
				};
			}
			try
			{
				return std::any_cast<const T&>(it->second);
			}
			catch (const std::bad_any_cast&)
			{
				//? This path should never happen but might as well add it
				return Unexpected{
					Error{ ErrorCode::InvalidCast }.SetCustomMessage(L"Failed to get custom style due to bad any cast")
				};
			}
			catch (const std::exception& e)
			{
				return Unexpected{
					Error{ SystemErrorCode::STLFailure  }.SetCustomMessage(StringToWString(e.what()))
				};
			}
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
			if (customStyles.size() != other.customStyles.size())
			{
				return false;
			}

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
		ThemeContext() = delete;

		[[nodiscard]] static auto GetCurrentTheme() noexcept
		{
			return AccessorProxy{ currentTheme, currentThemeMutex };
		}

		[[nodiscard]] static auto GetDarkTheme() noexcept
		{
			return AccessorProxy{ DarkTheme, darkThemeMutex };
		}
		[[nodiscard]] static auto GetLightTheme() noexcept
		{
			return AccessorProxy{ LightTheme, lightThemeMutex };
		}

		static auto ChangeCurrentTheme(const Theme& theme) noexcept -> void;

		[[nodiscard]] static auto& ThemeChangedEvent() noexcept { return themeChangedEvent; }

		[[nodiscard]] static auto& RespondToSystemThemeChange() noexcept { return respondToSystemThemeChange; }

		template <typename Function>
		static auto WithCurrentTheme(const Function& function) noexcept -> void 
			requires std::invocable<Function, const Theme&>
		{
			std::shared_lock lock{ currentThemeMutex };
			function(currentTheme);
		}
		template <typename Function>
		static auto WithDarkTheme(const Function& function) noexcept -> void
			requires std::invocable<Function, const Theme&>
		{
			std::shared_lock lock{ darkThemeMutex };
			function(DarkTheme);
		}
		template <typename Function>
		static auto WithLightTheme(const Function& function) noexcept -> void
			requires std::invocable<Function, const Theme&>
		{
			std::shared_lock lock{ lightThemeMutex };
			function(LightTheme);
		}

		template <typename Function>
		static auto ModifyCurrentTheme(const Function& function) noexcept -> void 
			requires std::invocable<Function, Theme&>
		{
			{
				std::scoped_lock lock{ currentThemeMutex };
				function(currentTheme);
			}
			themeChangedEvent.Invoke(GetCurrentTheme());
		}
		template <typename Function>
		static auto ModifyDarkTheme(const Function& function) noexcept -> void
			requires std::invocable<Function, Theme&>
		{
			{
				std::scoped_lock lock{ darkThemeMutex };
				function(DarkTheme);
			}
			if (IsCurrentThemeBuiltinDark())
			{
				ChangeCurrentTheme(GetDarkTheme().Get());
			}
		}
		template <typename Function>
		static auto ModifyLightTheme(const Function& function) noexcept -> void
			requires std::invocable<Function, Theme&>
		{
			{
				std::scoped_lock lock{ lightThemeMutex };
				function(LightTheme);
			}
			if (IsCurrentThemeBuiltinLight())
			{
				ChangeCurrentTheme(GetLightTheme().Get());
			}
		}

		static auto InitializeThemes() noexcept -> void;

		static auto OnSystemThemeChanged() -> void;

		private:
		inline static Theme DarkTheme;
		inline static Theme LightTheme;
		inline static std::atomic_bool respondToSystemThemeChange{ true };
		inline static Theme currentTheme{ };
		inline static Event<const AccessorProxy<Theme, Mutex::SRWMutex>&> themeChangedEvent;
		inline static Mutex::SRWMutex currentThemeMutex{ };
		inline static Mutex::SRWMutex darkThemeMutex{ };
		inline static Mutex::SRWMutex lightThemeMutex{ };

		static auto RefreshThemes() noexcept -> void;

		[[nodiscard]] static auto IsCurrentThemeBuiltinDark() noexcept
		{
			return GetCurrentTheme().Get() == GetDarkTheme().Get();
		}
		[[nodiscard]] static auto IsCurrentThemeBuiltinLight() noexcept
		{
			return GetCurrentTheme().Get() == GetLightTheme().Get();
		}
		[[nodiscard]] static auto IsCurrentThemeBuiltin() noexcept
		{
			return IsCurrentThemeBuiltinDark() || IsCurrentThemeBuiltinLight();
		}
	};
}

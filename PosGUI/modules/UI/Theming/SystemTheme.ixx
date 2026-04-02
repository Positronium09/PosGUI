module;
// ReSharper disable once CppUnusedIncludeDirective
#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.viewmanagement.h>

export module PGUI.UI.Theming.SystemTheme;

import PGUI.UI.Color;
import PGUI.ErrorHandling;
import PGUI.Event;

export namespace PGUI::UI::Theming
{
	enum class ColorType
	{
		Background = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::Background),
		Foreground = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::Foreground),
		AccentDark3 = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3),
		AccentDark2 = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2),
		AccentDark1 = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1),
		Accent = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::Accent),
		AccentLight1 = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1),
		AccentLight2 = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2),
		AccentLight3 = static_cast<int>(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3)
	};

	class SystemTheme final
	{
		public:
		SystemTheme() = delete;

		[[nodiscard]] static auto& ColorValuesChanged() noexcept { return colorValuesChangedEvent; }

		[[nodiscard]] static auto IsDarkMode() noexcept -> bool;

		[[nodiscard]] static auto IsLightMode() noexcept { return !IsDarkMode(); }

		[[nodiscard]] static auto GetColor(ColorType colorType) noexcept -> Result<RGBA>;

		[[nodiscard]] static auto GetTextColor() noexcept
		{
			return GetColor(ColorType::Foreground);
		}

		[[nodiscard]] static auto GetBackgroundColor() noexcept
		{
			return GetColor(ColorType::Background);
		}

		[[nodiscard]] static auto GetForegroundColor() noexcept
		{
			return GetColor(ColorType::Foreground);
		}

		[[nodiscard]] static auto GetAccentDark3Color() noexcept
		{
			return GetColor(ColorType::AccentDark3);
		}

		[[nodiscard]] static auto GetAccentDark2Color() noexcept
		{
			return GetColor(ColorType::AccentDark2);
		}

		[[nodiscard]] static auto GetAccentDark1Color() noexcept
		{
			return GetColor(ColorType::AccentDark1);
		}

		[[nodiscard]] static auto GetAccentColor() noexcept
		{
			return GetColor(ColorType::Accent);
		}

		[[nodiscard]] static auto GetAccentLight3Color() noexcept
		{
			return GetColor(ColorType::AccentLight3);
		}

		[[nodiscard]] static auto GetAccentLight2Color() noexcept
		{
			return GetColor(ColorType::AccentLight2);
		}

		[[nodiscard]] static auto GetAccentLight1Color() noexcept
		{
			return GetColor(ColorType::AccentLight1);
		}

		static auto RegisterSystemEventChanges() noexcept -> void
		{
			static std::once_flag onceFlag;
			std::call_once(onceFlag, []
			{
				[[maybe_unused]] auto colorValuesChangedToken = 
					GetUISettings().ColorValuesChanged([](
						const winrt::Windows::UI::ViewManagement::UISettings&,
						const winrt::Windows::Foundation::IInspectable&)
				{
					colorValuesChangedEvent.Invoke();
				});
			});
		}

		private:

		[[nodiscard]] static auto GetUISettings() noexcept -> winrt::Windows::UI::ViewManagement::UISettings&;

		inline static Event<> colorValuesChangedEvent;
	};
}

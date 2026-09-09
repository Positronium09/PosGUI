module;
#include <Windows.h>
#include <wil/registry.h>
#include <winrt/windows.ui.viewmanagement.h>

module PGUI.UI.Theming.SystemTheme;

import PGUI.Utils;
import PGUI.UI.Color;
import PGUI.ErrorHandling;

namespace PGUI::UI::Theming
{
	auto SystemTheme::IsDarkMode() noexcept -> Result<ThemeMode>
	{
		DWORD value = 0;

		if (const Error error{
				wil::reg::get_value_dword_nothrow(
					HKEY_CURRENT_USER,
					LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
					L"AppsUseLightTheme", &value)
			};
			error.IsFailure())
		{
			Logger::Error(L"Cannot read AppsUseLightTheme registry key");
		}

		return FromUnderlying<ThemeMode>(value);
	}

	auto SystemTheme::GetColor(ColorType colorType) noexcept -> Result<RGBA>
	{
		return WrapThrowing([&]
		{
			return RGBA{
				GetUISettings().GetColorValue(
					static_cast<winrt::Windows::UI::ViewManagement::UIColorType>(colorType))
			};
		});
	}

	auto SystemTheme::GetUISettings() noexcept -> winrt::Windows::UI::ViewManagement::UISettings&
	{
		static winrt::Windows::UI::ViewManagement::UISettings uiSettings;
		return uiSettings;
	}
}

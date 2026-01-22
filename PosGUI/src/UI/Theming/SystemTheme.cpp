module;
#include <Windows.h>
#define WIL_SUPPRESS_EXCEPTIONS 0
#include <wil/registry.h>
#include <winrt/windows.ui.viewmanagement.h>

module PGUI.UI.Theming.SystemTheme;

import PGUI.UI.Color;
import PGUI.ErrorHandling;

namespace PGUI::UI::Theming
{
	auto SystemTheme::IsDarkMode() -> bool
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

		return value == 0;
	}

	auto SystemTheme::GetColor(ColorType colorType) -> RGBA
	{
		try
		{
			return RGBA{
				uiSettings.GetColorValue(
					static_cast<winrt::Windows::UI::ViewManagement::UIColorType>(colorType))
			};
		}
		catch (winrt::hresult_error& error)
		{
			throw Exception{
				Error{
					static_cast<HRESULT>(error.code().value)
				}
			};
		}
	}
}

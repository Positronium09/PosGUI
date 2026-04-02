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
	auto SystemTheme::IsDarkMode() noexcept -> bool
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

	auto SystemTheme::GetColor(ColorType colorType) noexcept -> Result<RGBA>
	{
		try
		{
			return RGBA{
				GetUISettings().GetColorValue(
					static_cast<winrt::Windows::UI::ViewManagement::UIColorType>(colorType))
			};
		}
		catch (const winrt::hresult_error& error)
		{
			return Unexpected{
				Error{
					static_cast<HRESULT>(error.code().value)
				}.SetCustomMessage(error.message())
			};
		}
	}

	auto SystemTheme::GetUISettings() noexcept -> winrt::Windows::UI::ViewManagement::UISettings&
	{
		static winrt::Windows::UI::ViewManagement::UISettings uiSettings;
		return uiSettings;
	}
}

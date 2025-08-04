module;
#include <Windows.h>
#include <winrt/windows.ui.viewmanagement.h>

module PGUI.UI.Theming.SystemTheme;

import PGUI.UI.Color;
import PGUI.ErrorHandling;

namespace PGUI::UI::Theming
{
	auto SystemTheme::IsDarkMode() -> bool
	{
		auto value = 0UL;
		DWORD size = sizeof(value);

		if (const auto status = RegGetValueW(
				HKEY_CURRENT_USER,
				LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
				L"AppsUseLightTheme", RRF_RT_DWORD, nullptr, &value, &size);
			status != ERROR_SUCCESS)
		{
			Logger::Error(Error{ status }, L"Cannot read AppsUseLightTheme registry key");
		}

		return !value;
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
				}.AddTag(ErrorTags::System)
			};
		}
	}
}

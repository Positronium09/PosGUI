module;
#include <Windows.h>
#include <winrt//windows.ui.viewmanagement.h>

module PGUI.UI.Theming.SystemTheme;

import PGUI.UI.Color;
import PGUI.Exceptions;
import PGUI.Logging;

namespace PGUI::UI::Theming
{
	auto SystemTheme::IsDarkMode() -> bool

	{
		DWORD value = 0;
		DWORD size = sizeof(value);

		if (auto status = RegGetValueW(HKEY_CURRENT_USER,
			LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
			L"AppsUseLightTheme", RRF_RT_DWORD, nullptr, &value, &size);
			status != ERROR_SUCCESS)
		{
			Logger::Log(LogLevel::Error, L"Failed to read AppsUseLightTheme");
		}

		return !value;
	}

	auto SystemTheme::GetColor(ColorType colorType) -> RGBA
	{
		try
		{
			return RGBA{ uiSettings.GetColorValue(
				static_cast<winrt::Windows::UI::ViewManagement::UIColorType>(colorType)) };
		}
		catch (winrt::hresult_error& error)
		{
			throw HResultException{ error.code() };
		}
	}
}

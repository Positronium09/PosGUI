module;
#include <Windows.h>
#include <dwmapi.h>

export module PGUI.UI.Theming.Styles;

import PGUI.UI.Theming.SystemTheme;
import PGUI.UI.Brush;
import PGUI.UI.Color;
import PGUI.UI.Colors;

export namespace PGUI::UI::Theming
{
	enum CornerPreference
	{
		Default = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DEFAULT,
		DontRound = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DONOTROUND,
		Round = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUND,
		RoundSmall = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_ROUNDSMALL
	};

	struct AppWindowStyle
	{
		RGBA borderColor = Colors::Transparent;
		RGBA captionColor = Colors::Transparent;
		RGBA captionTextColor = Colors::Transparent;
		bool darkMode = SystemTheme::IsDarkMode();
		CornerPreference cornerPreference = CornerPreference::Default;
	};
}

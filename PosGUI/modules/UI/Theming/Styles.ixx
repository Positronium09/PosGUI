module;
#include <dwmapi.h>
#include <Windows.h>

export module PGUI.UI.Theming.Styles;

import PGUI.UI.Theming.SystemTheme;
import PGUI.UI.Brush;
import PGUI.UI.Color;
import PGUI.UI.Colors;

export namespace PGUI::UI::Theming
{
	enum class CornerPreference
	{
		Default = DWMWCP_DEFAULT,
		DontRound = DWMWCP_DONOTROUND,
		Round = DWMWCP_ROUND,
		RoundSmall = DWMWCP_ROUNDSMALL
	};

	struct AppWindowStyle
	{
		RGBA borderColor = Colors::Transparent;
		RGBA captionColor = Colors::Transparent;
		RGBA captionTextColor = Colors::Transparent;
		ThemeMode darkMode = SystemTheme::IsDarkMode().value_or(ThemeMode::Dark);
		CornerPreference cornerPreference = CornerPreference::Default;

		constexpr auto operator==(const AppWindowStyle&) const noexcept -> bool = default;
	};
}

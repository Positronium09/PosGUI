export module PGUI.UI.Theming.ColorContext;

import PGUI.UI.Color;
import PGUI.UI.Colors;
import PGUI.UI.Theming.SystemTheme;

export namespace PGUI::UI::Theming
{
	struct ColorContext
	{
		RGBA text{ };
		RGBA background{ };
		RGBA primary{ };
		RGBA secondary{ };
		RGBA accent{ };

		constexpr auto operator==(const ColorContext&) const noexcept -> bool = default;
	};
}

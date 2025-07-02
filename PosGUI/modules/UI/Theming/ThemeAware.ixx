export module PGUI.UI.Theming.ThemeAware;

import std;

import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.Theme;

export namespace PGUI::UI::Theming
{
	template <typename StyleType>
	class ThemeAware
	{
		public:
		virtual ~ThemeAware() noexcept = default;

		virtual auto ApplyStyle(const StyleType& style) -> void = 0;

		protected:
		ThemeAware() noexcept
		{
			ThemeContext::ThemeChangedEvent().AddCallback(
				std::bind_front(&ThemeAware::OnThemeChanged, this));
		}

		auto ApplyTheme(const Theme& theme) noexcept -> void
		{
			OnThemeChanged(theme);
		}

		auto ApplyCurrentTheme() -> void
		{
			ThemeContext::WithCurrentTheme([this](const Theme& theme)
			{
				ApplyTheme(theme);
			});
		}

		auto OnThemeChanged(const Theme& theme) -> void
		{
			if constexpr (std::is_same_v<StyleType, AppWindowStyle>)
			{
				auto& appWindowStyle = theme.appWindowStyle;
				ApplyStyle(appWindowStyle);
			}

			if (theme.HasCustomStyle<StyleType>())
			{
				const auto& style = theme.GetCustomStyle<StyleType>();
				ApplyStyle(style);
			}
		}
	};
}

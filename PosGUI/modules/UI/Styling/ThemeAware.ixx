module;
#include <any>
#include <functional>

export module PGUI.UI.Styling.ThemeAware;

import PGUI.UI.Styling.Styles;
import PGUI.UI.Styling.Theme;

export namespace PGUI::UI::Styling
{
	template <typename StyleType>
	class ThemeAware
	{
		public:
		virtual ~ThemeAware() noexcept = default;

		virtual void ApplyStyle(const StyleType& style) = 0;

		protected:
		ThemeAware() noexcept
		{
			ThemeContext::ThemeChangedEvent().AddCallback(
				std::bind_front(&ThemeAware::OnThemeChanged, this));
		}

		void ApplyTheme(const Theme& theme) noexcept
		{
			OnThemeChanged(theme);
		}

		void ApplyCurrentTheme()
		{
			const auto& currentTheme = ThemeContext::GetCurrentTheme();
			ApplyTheme(currentTheme);
		}

		void OnThemeChanged(const Theme& theme)
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

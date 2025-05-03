module;
#include <any>
#include <functional>

export module PGUI.UI.Theming.ThemeAware;

import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.Theme;

export namespace PGUI::UI::Theming
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
			ThemeContext::WithCurrentTheme([this](const Theme& theme)
			{
				ApplyTheme(theme);
			});
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

export module PGUI.UI.Theming.ThemeAware;

import std;

import PGUI.Event;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.Theming.Styles;
import PGUI.UI.Theming.Theme;

export namespace PGUI::UI::Theming
{
	template <typename StyleType>
	class ThemeAware
	{
		public:
		virtual ~ThemeAware() noexcept
		{
			ThemeContext::ThemeChangedEvent().RemoveCallback(callbackId);
		}

		virtual auto ApplyStyle(const StyleType& style) -> void = 0;

		protected:
		ThemeAware() noexcept
		{
			callbackId = ThemeContext::ThemeChangedEvent().AddCallback(
				[this](const AccessorProxy<Theme, Mutex::SRWMutex>& theme)
				{
					OnThemeChanged(theme.Get());
				});
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
			else
			{
				if (theme.HasCustomStyle<StyleType>())
				{
					if (const auto& style = theme.GetCustomStyle<StyleType>();
						style.has_value())
					{
						ApplyStyle(style.value().get());
					}
					else
					{
						Logger::Warning(style.error(), L"Cannot unwrap style object");
					}
				}
			}
		}

		private:
		CallbackId callbackId;
	};
}

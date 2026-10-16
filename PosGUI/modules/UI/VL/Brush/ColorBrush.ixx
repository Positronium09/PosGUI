module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Brush.CompositionColorBrush;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.UI.Color;
import PGUI.UI.Brush;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Brush.CompositionBrush;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionColorBrush : public CompositionBrushT<WUC::CompositionColorBrush>
	{
		public:
		using CompositionBrushT::CompositionBrushT;

		[[nodiscard]] static auto Create(const RGBA& color) noexcept -> Result<CompositionColorBrush>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &color]
			{
				return CompositionColorBrush{ compositor.CreateColorBrush(color) };
			});
		}

		[[nodiscard]] static auto CreateFrom(const Brush& brush) noexcept -> Result<CompositionColorBrush>
		{
			const auto& parameters = brush.GetParameters();
			if (!std::holds_alternative<RGBA>(parameters))
			{
				return Unexpected{ Error{ ErrorCode::InvalidArgument } };
			}

			const auto& color = std::get<RGBA>(parameters);
			return Create(color);
		}

		[[nodiscard]] auto GetColor() const noexcept -> Result<RGBA>
		{
			return CallProjected([this]
			{
				return RGBA{ Get().Color() };
			});
		}
		[[nodiscard]] auto SetColor(const RGBA& color) const noexcept -> Result<void>
		{
			return CallProjected([this, &color]
			{
				Get().Color(color);
			});
		}
	};
}

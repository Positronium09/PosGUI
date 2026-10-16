module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Visual.SpriteVisual;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.Shape;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Visual.ContainerVisual;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.Brush.CompositionBrush;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class SpriteVisual : public ContainerVisualT<WUC::SpriteVisual>
	{
		public:
		using ContainerVisualT::ContainerVisualT;

		[[nodiscard]] static auto Create() noexcept -> Result<SpriteVisual>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return SpriteVisual{ compositor.CreateSpriteVisual() };
			});
		}
		[[nodiscard]] static auto Create(const Point3F& position, const SizeF& size) noexcept -> Result<SpriteVisual>
		{
			return Create().and_then([position, size](const SpriteVisual& visual) noexcept -> Result<SpriteVisual>
			{
				if (auto ret = visual.SetPosition(position);
					!ret.has_value())
				{
					return Unexpected{ ret.error().SetCustomMessage(L"Failed to set position") };
				}
				if (auto ret = visual.SetSize(size);
					!ret.has_value())
				{
					return Unexpected{ ret.error().SetCustomMessage(L"Failed to set size") };
				}
				return visual;
			});
		}
		[[nodiscard]] static auto Create(const RectF& rect) noexcept -> Result<SpriteVisual>
		{
			return Create(rect.TopLeft(), rect.Size());
		}

		[[nodiscard]] auto GetBrush() const noexcept -> Result<CompositionBrush>
		{
			return CallProjected([this]
			{
				return CompositionBrush{ Get().Brush() };
			});
		}
		[[nodiscard]] auto SetBrush(const CompositionBrush& brush) const noexcept -> Result<void>
		{
			return CallProjected([this, &brush]
			{
				Get().Brush(brush.Get());
			});
		}

		//TODO Shadow CompositionShadow
	};
}

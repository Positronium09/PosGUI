module PGUI.UI.UIBase:UIComponent;

import :UIElement;
import PGUI.Shape2D;
import PGUI.UI.Clip;
import PGUI.UI.D2D;

namespace PGUI::UI
{
	auto UIComponent::Render(Graphics graphics) -> void
	{
		auto axisAligned = false;
		auto pushed = false;

		if (!clip)
		{
			axisAligned = true;
			graphics.PushAxisAlignedClip(rect, D2D::AntiAliasingMode::PerPrimitive);
		}
		else
		{
			if (const auto layerResult = graphics.CreateLayer();
				layerResult.has_value())
			{
				const auto& layer = layerResult.value();
				const D2D::LayerParameters layerParams{ rect };
				graphics.PushLayer(layer, layerParams);
				pushed = true;
			}
		}

		ClippedRender(graphics);

		if (axisAligned)
		{
			graphics.PopAxisAlignedClip();
		}
		else if (pushed)
		{
			graphics.PopLayer();
		}
	}

	auto UIComponent::Move(const PointF position) -> void
	{
		if (GetPosition() == position)
		{
			return;
		}

		PositionChangedEvent event;
		event.oldPosition = GetPosition();
		rect.Move(position);
		event.newPosition = GetPosition();
		HandleEvent(event);
	}

	auto UIComponent::Resize(const SizeF size) -> void
	{
		if (GetSize() == size)
		{
			return;
		}

		SizeChangedEvent event;
		event.oldSize = GetSize();
		rect.Resize(size);
		event.newSize = GetSize();
		HandleEvent(event);
	}

	auto UIComponent::MoveAndResize(const RectF newRect) -> void
	{
		if (GetRect() == newRect)
		{
			return;
		}

		RectChangedEvent event;
		event.oldRect = GetRect();
		rect = newRect;
		event.newRect = GetRect();
		HandleEvent(event);
	}

	auto UIComponent::HitTest(const PointF point) noexcept -> bool
	{
		if (!GetRect().Contains(point))
		{
			return false;
		}
		if (!clip)
		{
			return true;
		}

		if (const auto result = clip.GetGeometry().FillContainsPoint(point);
			result.has_value())
		{
			return result.value();
		}

		return false;
	}
}

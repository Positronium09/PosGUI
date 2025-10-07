module PGUI.UI.UIBase:UIComponent;

import :UIElement;
import PGUI.Shape2D;
import PGUI.UI.Clip;
import PGUI.UI.D2D;
import PGUI.ErrorHandling;

namespace PGUI::UI
{
	auto UIComponent::Render(Graphics graphics) -> void
	{
		auto axisAligned = false;
		auto pushed = false;

		if (!clip)
		{
			axisAligned = true;
			graphics.PushAxisAlignedClip(GetRect(), D2D::AntiAliasingMode::PerPrimitive);
		}
		else
		{
			if (const auto layerResult = graphics.CreateLayer();
				layerResult.has_value())
			{
				const auto& layer = layerResult.value();
				const D2D::LayerParameters layerParams{
					GetRect(),
					D2D::AntiAliasingMode::PerPrimitive,
					D2D::LayerOptions::None,
					GetClip().GetGeometry()
				};
				graphics.PushLayer(layer, layerParams);
				pushed = true;
			}
			else
			{
				Logger::Error(layerResult.error(), L"Cannot create layer for clipping");
				axisAligned = true;
				graphics.PushAxisAlignedClip(GetRect(), D2D::AntiAliasingMode::PerPrimitive);
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
		Invalidate();
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
		Invalidate();
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
		Invalidate();
	}

	auto UIComponent::HandleEvent(UIEvent& uiEvent) -> void
	{
		UIElement::HandleEvent(uiEvent);
		if (uiEvent.handled)
		{
			return;
		}

		if (uiEvent.type != EventType::RectChanged && uiEvent.type == EventType::SizeChanged)
		{
			return;
		}

		if (auto parameters = GetClip().GetParameters();
			std::holds_alternative<RoundedRectangleClipParameters>(parameters))
		{
			auto& params = std::get<RoundedRectangleClipParameters>(parameters);
			params.rect.MoveAndResize(GetRect());
			SetClip(Clip{ params });
		}
		else if (std::holds_alternative<RectangleClipParameters>(parameters))
		{
			auto& params = std::get<RectangleClipParameters>(parameters);
			params.rect = GetRect();
			SetClip(Clip{ params });
		}
		else if (std::holds_alternative<EllipseClipParameters>(parameters))
		{
			auto& params = std::get<EllipseClipParameters>(parameters);
			params.ellipse.center = GetRect().Center();
			SetClip(Clip{ params });
		}
		else if (std::holds_alternative<RoundCornerClipParameters>(parameters))
		{
			auto& params = std::get<RoundCornerClipParameters>(parameters);
			params.rect = GetRect();
			SetClip(Clip{ params });
		}
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

		if (const auto result = clip.GetGeometry().FillContainsPoint(point - GetPosition());
			result.has_value())
		{
			return result.value();
		}

		return false;
	}
}

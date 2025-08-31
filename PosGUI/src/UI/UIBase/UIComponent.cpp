module PGUI.UI.UIBase:UIComponent;

import std;

import PGUI.Shape2D;
import PGUI.WindowClass;
import PGUI.UI.Colors;
import PGUI.UI.Clip;
import PGUI.UI.D2D;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIComponent::UIComponent(const RectF bounds) :
		bounds{ bounds }
	{ }

	auto UIComponent::SetClip(const ClipParameters& params) noexcept -> void
	{
		clip.SetParameters(params);
		AdjustClip();
		clipChanged.Invoke();
	}

	auto UIComponent::ClearClip() noexcept -> void
	{
		clip.Clear();
		clipChanged.Invoke();
	}

	auto UIComponent::SetBounds(const RectF& rect) noexcept -> void
	{
		if (bounds == rect)
		{
			return;
		}

		const auto oldBounds = bounds;
		bounds = rect;
		if (adjustClipOnSize && oldBounds.Size() != bounds.Size())
		{
			AdjustClip();
		}
		boundsChanged.Invoke(oldBounds, bounds);
	}

	auto UIComponent::AdjustClip() noexcept -> void
	{
		if (auto& clipParams = clip.GetParameters();
			std::holds_alternative<EllipseClipParameters>(clipParams))
		{
			EllipseClipParameters& params = std::get<EllipseClipParameters>(clipParams);
			params.ellipse.center = bounds.Center();
		}
		else if (std::holds_alternative<RectangleClipParameters>(clipParams))
		{
			RectangleClipParameters& params = std::get<RectangleClipParameters>(clipParams);
			params.rect = bounds;
		}
		else if (std::holds_alternative<RoundedRectangleClipParameters>(clipParams))
		{
			RoundedRectangleClipParameters& params = std::get<RoundedRectangleClipParameters>(clipParams);
			params.rect.left = bounds.left;
			params.rect.top = bounds.top;
			params.rect.right = bounds.right;
			params.rect.bottom = bounds.bottom;
		}
		else if (std::holds_alternative<RoundCornerClipParameters>(clipParams))
		{
			RoundCornerClipParameters& params = std::get<RoundCornerClipParameters>(clipParams);
			params.rect = bounds;
		}

		clip.CreateClip();
	}

	auto UIComponent::HitTest(const PointF point) const noexcept -> bool
	{
		if (!HitTestBounds(point))
		{
			return false;
		}

		if (!hitTestClip || !clip)
		{
			return GetBounds().IsPointInside(point);
		}

		const auto& geometry = clip.GetGeometry();
		auto containsResult = geometry.FillContainsPoint(point);
		if (!containsResult.has_value())
		{
			Logger::Error(containsResult.error(), L"Geometry hit test failed");
			return false;
		}

		return containsResult.value();
	}

	auto UIComponent::HitTest(const RectF rect) const noexcept -> bool
	{
		if (!HitTestBounds(rect))
		{
			return false;
		}

		if (!hitTestClip || !clip)
		{
			return GetBounds().IsIntersectingRect(rect);
		}

		const auto& geometry = clip.GetGeometry();
		const auto rectangleGeometry = D2D::D2DRectangleGeometry{ rect };
		const auto comparisonResult = geometry.CompareWithGeometry(rectangleGeometry);

		if (!comparisonResult.has_value())
		{
			Logger::Error(comparisonResult.error(), L"Geometry comparison failed");
			return false;
		}

		const auto comparison = comparisonResult.value();
		return comparison == D2D::GeometryRelation::Disjoint;
	}

	auto UIComponent::HitTestBounds(const PointF point) const noexcept -> bool
	{
		return GetBounds().IsPointInside(point);
	}

	auto UIComponent::HitTestBounds(const RectF rect) const noexcept -> bool
	{
		return GetBounds().IsIntersectingRect(rect);
	}
}

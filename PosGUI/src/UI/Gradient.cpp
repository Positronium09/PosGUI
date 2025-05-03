module;
#include <d2d1_1.h>

module PGUI.UI.Gradient;
import PGUI.Utils;

namespace PGUI::UI
{
	Gradient::Gradient(const GradientStops& stops) noexcept :
		stops(stops)
	{
	}

	LinearGradient::LinearGradient(PointF start, PointF end, const GradientStops& stops) noexcept : 
		Gradient{ stops },
		start(start),
		end(end)
	{
	}

	void LinearGradient::ApplyReferenceRect(RectF rect) noexcept
	{
		start.x = MapToRange(start.x, rect.left, rect.right);
		start.y = MapToRange(start.y, rect.top, rect.bottom);

		end.x = MapToRange(end.x, rect.left, rect.right);
		end.y = MapToRange(end.y, rect.top, rect.bottom);
	}
	auto LinearGradient::ReferenceRectApplied(RectF rect) const noexcept -> LinearGradient
	{
		auto gradient = *this;
		gradient.ApplyReferenceRect(rect);

		return gradient;
	}

	RadialGradient::RadialGradient(Ellipse _ellipse, PointF _offset, const GradientStops& stops) noexcept :
		Gradient{ stops }, ellipse(_ellipse), offset(_offset)
	{
	}

	void RadialGradient::ApplyReferenceRect(RectF rect) noexcept
	{
		auto size = rect.Size();

		ellipse.center.x = MapToRange(ellipse.center.x, rect.right, rect.left);
		ellipse.center.y = MapToRange(ellipse.center.y, rect.bottom, rect.top);

		offset.x = MapToRange(offset.y, rect.right, rect.left);
		offset.y = MapToRange(offset.y, rect.bottom, rect.top);

		ellipse.xRadius = MapToRange(ellipse.xRadius, 0.0F, size.cx);
		ellipse.yRadius = MapToRange(ellipse.yRadius, 0.0F, size.cy);
	}

	auto RadialGradient::ReferenceRectApplied(RectF rect) const noexcept -> RadialGradient
	{
		auto gradient = *this;
		gradient.ApplyReferenceRect(rect);

		return gradient;
	}
}

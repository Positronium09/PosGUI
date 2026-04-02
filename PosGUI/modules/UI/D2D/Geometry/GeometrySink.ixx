module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.GeometrySink;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.D2D.SimplifiedGeometrySink;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	class GeometrySink : public SimplifiedGeometrySink<ID2D1GeometrySink>
	{
		public:
		GeometrySink() noexcept = default;

		explicit GeometrySink(const ComPtr<ID2D1GeometrySink>& ptr) noexcept :
			SimplifiedGeometrySink{ ptr }
		{ }

		const auto& AddLine(const PointF point) const noexcept
		{
			Get()->AddLine(point);
			return *this;
		}

		const auto& AddArc(const ArcSegment& arcSegment) const noexcept
		{
			Get()->AddArc(arcSegment);
			return *this;
		}

		const auto& AddBezier(const BezierSegment& bezierSegment) const noexcept
		{
			Get()->AddBezier(bezierSegment);
			return *this;
		}

		const auto& AddQuadraticBezier(const QuadraticBezierSegment quadraticBezierSegment) const noexcept
		{
			Get()->AddQuadraticBezier(quadraticBezierSegment);
			return *this;
		}

		const auto& AddQuadraticBeziers(const std::span<const QuadraticBezierSegment> segments) const noexcept
		{
			Get()->AddQuadraticBeziers(segments.data(), static_cast<UINT32>(segments.size()));
			return *this;
		}
	};
}

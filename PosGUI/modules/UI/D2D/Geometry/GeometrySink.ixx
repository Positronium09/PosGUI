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

		auto& AddLine(const PointF point) const
		{
			Get()->AddLine(point);
			return *this;
		}

		auto& AddArc(const ArcSegment& arcSegment) const
		{
			Get()->AddArc(arcSegment);
			return *this;
		}

		auto& AddBezier(const BezierSegment& bezierSegment) const
		{
			Get()->AddBezier(bezierSegment);
			return *this;
		}

		auto& AddQuadraticBezier(const QuadraticBezierSegment quadraticBezierSegment) const
		{
			Get()->AddQuadraticBezier(quadraticBezierSegment);
			return *this;
		}

		auto& AddQuadraticBeziers(const std::span<const QuadraticBezierSegment> segments) const
		{
			Get()->AddQuadraticBeziers(segments.data(), static_cast<UINT32>(segments.size()));
			return *this;
		}
	};
}

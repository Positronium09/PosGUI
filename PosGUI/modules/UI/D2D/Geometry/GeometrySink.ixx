module;
#include <span>
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.GeometrySink;

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
		GeometrySink(ComPtr<ID2D1GeometrySink> ptr) noexcept :
			SimplifiedGeometrySink{ ptr }
		{
		}

		void AddLine(PointF point)
		{
			Get()->AddLine(point);
		}

		void AddArc(ArcSegment arcSegment)
		{
			Get()->AddArc(arcSegment);
		}

		void AddBezier(BezierSegment bezierSegment)
		{
			Get()->AddBezier(bezierSegment);
		}

		void AddQuadraticBezier(QuadraticBezierSegment quadraticBezierSegment)
		{
			Get()->AddQuadraticBezier(quadraticBezierSegment);
		}

		void AddQuadraticBeziers(std::span<const QuadraticBezierSegment> segments)
		{
			Get()->AddQuadraticBeziers(segments.data(), static_cast<UINT32>(segments.size()));
		}
	};
}

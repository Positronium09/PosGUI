module;
#include <d2d1_3.h>
#include <wrl.h>

module PGUI.UI.D2D.D2DPathGeometry;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.GeometrySink;
import PGUI.UI.D2D.SimplifiedGeometrySink;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

namespace PGUI::UI::D2D
{
	D2DPathGeometry::D2DPathGeometry()
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		const auto hr = factory->CreatePathGeometry(GetAddress());
		ThrowFailed(hr);
	}

	D2DPathGeometry::D2DPathGeometry(const ComPtr<ID2D1PathGeometry1>& ptr) noexcept :
		D2DGeometry{ ptr }
	{ }

	auto D2DPathGeometry::GetFigureCount() -> UINT32
	{
		UINT32 count = 0;
		const auto hr = Get()->GetFigureCount(&count);
		ThrowFailed(hr);
		return count;
	}

	auto D2DPathGeometry::GetSegmentCount() -> UINT32
	{
		UINT32 count = 0;
		const auto hr = Get()->GetSegmentCount(&count);
		ThrowFailed(hr);
		return count;
	}

	auto D2DPathGeometry::Open() -> GeometrySink
	{
		ComPtr<ID2D1GeometrySink> sink;
		const auto hr = Get()->Open(sink.GetAddressOf());
		ThrowFailed(hr);
		return GeometrySink{ sink };
	}

	auto D2DPathGeometry::Stream(const GeometrySink& sink) -> void
	{
		const auto hr = Get()->Stream(sink.Get());
		ThrowFailed(hr);
	}

	auto D2DPathGeometry::CreateRoundRectWithPathGeometry(
		RectF rect,
		const float topLeft,
		const float topRight,
		const float bottomLeft,
		const float bottomRight) -> D2DPathGeometry
	{
		D2DPathGeometry pathGeometry;
		auto sink = pathGeometry.Open();

		std::array<PointF, 2> points;
		PointF& point1 = points[0];
		PointF& point2 = points[1];
		point1 = { rect.left + topLeft, rect.top };
		point2 = { rect.right - topRight, rect.top };

		sink.BeginFigure(point1, FigureBegin::Filled);
		sink.AddLines(points);

		point1 = { rect.right, rect.top + topRight };
		if (topRight)
		{
			const auto p = point1.Rotated(0, point2);
			const auto arcSegment = ArcSegment{
				p,
				D2D1::SizeF(topRight, topRight),
				0,
				ArcSize::Small,
				SweepDirection::Clockwise
			};
			sink.AddArc(arcSegment);
		}

		point2 = { rect.right, rect.bottom - bottomRight };
		sink.AddLines(points);

		point1 = { rect.right - bottomRight, rect.bottom };
		if (bottomRight)
		{
			const auto p = point1.Rotated(0, point2);
			const auto arcSegment = ArcSegment{
				p,
				D2D1::SizeF(bottomRight, bottomRight),
				0,
				ArcSize::Small,
				SweepDirection::Clockwise
			};
			sink.AddArc(arcSegment);
		}

		point2 = { rect.left + bottomLeft, rect.bottom };
		sink.AddLines(points);

		point1 = { rect.left, rect.bottom - bottomLeft };
		if (bottomLeft)
		{
			const auto p = point1.Rotated(0, point2);
			const auto arcSegment = ArcSegment{
				p,
				D2D1::SizeF(bottomLeft, bottomLeft),
				0,
				ArcSize::Small,
				SweepDirection::Clockwise
			};
			sink.AddArc(arcSegment);
		}

		point2 = { rect.left, rect.top + topLeft };
		sink.AddLines(points);
		point1 = { rect.left + topLeft, rect.top };
		if (topLeft)
		{
			const auto p = point1.Rotated(0, point2);
			const auto arcSegment = ArcSegment{
				p,
				D2D1::SizeF(topLeft, topLeft),
				0,
				ArcSize::Small,
				SweepDirection::Clockwise
			};
			sink.AddArc(arcSegment);
		}

		sink.EndFigure(FigureEnd::Closed);
		sink.Close();

		return pathGeometry;
	}
}

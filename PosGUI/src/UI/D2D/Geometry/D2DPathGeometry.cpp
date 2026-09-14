module;
#include <d2d1_3.h>

module PGUI.UI.D2D.D2DPathGeometry;

import std;

import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.Factories;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.GeometrySink;
import PGUI.UI.D2D.SimplifiedGeometrySink;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

namespace PGUI::UI::D2D
{
	D2DPathGeometry::D2DPathGeometry(const ComPtr<ID2D1PathGeometry1>& ptr) noexcept :
		D2DGeometry{ ptr }
	{ }

	auto D2DPathGeometry::Create() noexcept -> Result<D2DPathGeometry>
	{
		const auto& factory = Factories::D2DFactory::GetFactory();

		ComPtr<ID2D1PathGeometry1> geometry;
		if (const auto error = Error{ factory->CreatePathGeometry(geometry.put()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return D2DPathGeometry{ geometry };
	}

	auto D2DPathGeometry::GetFigureCount() const noexcept -> Result<UINT32>
	{
		UINT32 count = 0;

		if (const auto hr = Get()->GetFigureCount(&count);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get figure count");
			return Unexpected{ error };
		}

		return count;
	}

	auto D2DPathGeometry::GetSegmentCount() const noexcept -> Result<UINT32>
	{
		UINT32 count = 0;
		if (const auto hr = Get()->GetSegmentCount(&count);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get segment count");
			return Unexpected{ error };
		}
		return count;
	}

	auto D2DPathGeometry::Open() const noexcept -> Result<GeometrySink>
	{
		ComPtr<ID2D1GeometrySink> sink;
		if (const auto hr = Get()->Open(sink.put());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to open geometry sink");
			return Unexpected{ error };
		}
		return GeometrySink{ sink };
	}

	auto D2DPathGeometry::Stream(const GeometrySink& sink) const noexcept -> Result<void>
	{
		Error error{
			Get()->Stream(sink.GetRaw())
		};
		LogIfFailed(error, L"Streaming failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto D2DPathGeometry::CreateRoundRectWithPathGeometry(
		RectF rect,
		const float topLeft,
		const float topRight,
		const float bottomLeft,
		const float bottomRight) noexcept -> Result<D2DPathGeometry>
	{
		auto pathGeometryResult = Create();
		if (!pathGeometryResult.has_value())
		{
			return Unexpected{ pathGeometryResult.error() };
		}
		const auto& pathGeometry = pathGeometryResult.value();

		auto sinkResult = pathGeometry.Open();
		if (!sinkResult.has_value())
		{
			return Unexpected{ sinkResult.error() };
		}

		auto& sink = sinkResult.value();

		std::array<PointF, 2> points;
		PointF& point1 = points[0];
		PointF& point2 = points[1];
		point1 = { rect.left + topLeft, rect.top };
		point2 = { rect.right - topRight, rect.top };

		Unused(sink.BeginFigure(point1, FigureBegin::Filled));
		Unused(sink.AddLines(points));

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
			Unused(sink.AddArc(arcSegment));
		}

		point2 = { rect.right, rect.bottom - bottomRight };
		Unused(sink.AddLines(points));

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
			Unused(sink.AddArc(arcSegment));
		}

		point2 = { rect.left + bottomLeft, rect.bottom };
		Unused(sink.AddLines(points));

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
			Unused(sink.AddArc(arcSegment));
		}

		point2 = { rect.left, rect.top + topLeft };
		Unused(sink.AddLines(points));

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
			Unused(sink.AddArc(arcSegment));
		}

		Unused(sink.EndFigure(FigureEnd::Closed));

		if (const auto result = sink.Close();
			!result.has_value())
		{
			return Unexpected{ result.error() };
		}

		return pathGeometryResult;
	}
}

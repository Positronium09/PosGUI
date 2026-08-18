module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.SimplifiedGeometrySink;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.ErrorHandling;
import PGUI.Factories;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	template <std::derived_from<ID2D1SimplifiedGeometrySink> Interface = ID2D1SimplifiedGeometrySink>
	class SimplifiedGeometrySink : public ComPtrHolder<Interface>
	{
		public:
		SimplifiedGeometrySink() noexcept = default;

		explicit(false) SimplifiedGeometrySink(const ComPtr<Interface>& ptr) noexcept :
			ComPtrHolder<Interface>{ ptr }
		{ }

		const auto& BeginFigure(PointF startPoint, FigureBegin figureBegin) const noexcept
		{
			this->Get()->BeginFigure(startPoint, static_cast<D2D1_FIGURE_BEGIN>(figureBegin));
			return *this;
		}

		const auto& EndFigure(FigureEnd figureEnd) const noexcept
		{
			this->Get()->EndFigure(static_cast<D2D1_FIGURE_END>(figureEnd));
			return *this;
		}

		[[nodiscard]] auto Close() const noexcept -> Result<void>
		{
			Error error{ this->Get()->Close() };
			LogIfFailed(error, L"Failed to close the sink");
			if (error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}

		const auto& SetFillMode(FillMode fillMode) const noexcept
		{
			this->Get()->SetFillMode(static_cast<D2D1_FILL_MODE>(fillMode));
			return *this;
		}

		const auto& SetSegmentFlags(SegmentFlags segmentFlags) const noexcept
		{
			this->Get()->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(segmentFlags));
			return *this;
		}

		const auto& AddLines(const std::span<const PointF> points) const noexcept
		{
			this->Get()->AddLines(reinterpret_cast<const D2D1_POINT_2F*>(points.data()),
			                      static_cast<UINT32>(points.size()));
			return *this;
		}

		const auto& AddBeziers(const std::span<const BezierSegment> beziers) const noexcept
		{
			this->Get()->AddBeziers(static_cast<const D2D1_BEZIER_SEGMENT*>(beziers.data()),
			                        static_cast<UINT32>(beziers.size()));
			return *this;
		}

		explicit(false) constexpr operator SimplifiedGeometrySink<>() const noexcept
		{
			return SimplifiedGeometrySink<>{ this->Get() };
		}
	};
}

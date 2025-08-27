module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.SimplifiedGeometrySink;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import PGUI.Factories;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	template <typename Interface = ID2D1SimplifiedGeometrySink>
	class SimplifiedGeometrySink : public ComPtr<Interface>
	{
		static_assert(
			std::is_same_v<Interface, ID2D1SimplifiedGeometrySink> || std::is_same_v<Interface, ID2D1GeometrySink>,
			"Interface must be ID2D1SimplifiedGeometrySink or ID2D1GeometrySink");

		public:
		SimplifiedGeometrySink() noexcept = default;

		explicit(false) SimplifiedGeometrySink(ComPtr<Interface> ptr) noexcept :
			ComPtr<Interface>(ptr)
		{ }

		auto BeginFigure(PointF startPoint, FigureBegin figureBegin) noexcept -> void
		{
			this->Get()->BeginFigure(startPoint, static_cast<D2D1_FIGURE_BEGIN>(figureBegin));
		}

		auto EndFigure(FigureEnd figureEnd) noexcept -> void
		{
			this->Get()->EndFigure(static_cast<D2D1_FIGURE_END>(figureEnd));
		}

		auto Close() noexcept -> Error
		{
			Error error{ this->Get()->Close() };
			LogIfFailed(error, L"Failed to close the sink");
			return error;
		}

		auto SetFillMode(FillMode fillMode) noexcept -> void
		{
			this->Get()->SetFillMode(static_cast<D2D1_FILL_MODE>(fillMode));
		}

		auto SetSegmentFlags(SegmentFlags segmentFlags) noexcept -> void
		{
			this->Get()->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(segmentFlags));
		}

		auto AddLines(const std::span<const PointF> points) noexcept -> void
		{
			this->Get()->AddLines(std::bit_cast<const D2D1_POINT_2F*>(points.data()),
			                      static_cast<UINT32>(points.size()));
		}

		auto AddBeziers(const std::span<const BezierSegment> beziers) noexcept -> void
		{
			this->Get()->AddBeziers(static_cast<const D2D1_BEZIER_SEGMENT*>(beziers.data()),
			                        static_cast<UINT32>(beziers.size()));
		}

		explicit(false) constexpr operator SimplifiedGeometrySink<>() const noexcept
		{
			return SimplifiedGeometrySink<>{ this->Get() };
		}
	};
}

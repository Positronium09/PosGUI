module;
#include <bit>
#include <span>
#include <concepts>
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.SimplifiedGeometrySink;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Exceptions;
import PGUI.Factories;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	template <typename Interface = ID2D1SimplifiedGeometrySink>
	class SimplifiedGeometrySink : public ComPtr<Interface>
	{
		static_assert(std::is_same_v<Interface, ID2D1SimplifiedGeometrySink> || std::is_same_v<Interface, ID2D1GeometrySink>, 
			"Interface must be ID2D1SimplifiedGeometrySink or ID2D1GeometrySink");

		public:
		SimplifiedGeometrySink() noexcept = default;
		SimplifiedGeometrySink(ComPtr<Interface> ptr) noexcept : 
			ComPtr<Interface>(ptr)
		{
		}

		void BeginFigure(PointF startPoint, FigureBegin figureBegin) noexcept
		{
			this->Get()->BeginFigure(startPoint, static_cast<D2D1_FIGURE_BEGIN>(figureBegin));
		}

		void EndFigure(FigureEnd figureEnd) noexcept
		{
			this->Get()->EndFigure(static_cast<D2D1_FIGURE_END>(figureEnd));
		}

		void Close()
		{
			auto hr = this->Get()->Close(); ThrowFailed(hr);
		}

		void SetFillMode(FillMode fillMode) noexcept
		{
			this->Get()->SetFillMode(static_cast<D2D1_FILL_MODE>(fillMode));
		}

		void SetSegmentFlags(SegmentFlags segmentFlags) noexcept
		{
			this->Get()->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(segmentFlags));
		}

		void AddLines(std::span<const PointF> points) noexcept
		{
			this->Get()->AddLines(std::bit_cast<const D2D1_POINT_2F*>(points.data()), static_cast<UINT32>(points.size()));
		}

		void AddBeziers(std::span<const BezierSegment> beziers) noexcept
		{
			this->Get()->AddBeziers(static_cast<const D2D1_BEZIER_SEGMENT*>(beziers.data()), static_cast<UINT32>(beziers.size()));
		}

		explicit(false) constexpr operator SimplifiedGeometrySink<>() const noexcept
		{
			return SimplifiedGeometrySink<>{ this->Get() };
		}
	};
}

module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.D2DPathGeometry;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.GeometrySink;

export namespace PGUI::UI::D2D
{
	class D2DPathGeometry : public D2DGeometry<ID2D1PathGeometry1>
	{
		public:
		D2DPathGeometry();

		explicit(false) D2DPathGeometry(ComPtr<ID2D1PathGeometry1> ptr) noexcept;

		auto GetFigureCount() -> UINT32;

		auto GetSegmentCount() -> UINT32;

		auto Open() -> GeometrySink;

		auto Stream(GeometrySink sink) -> void;

		[[nodiscard]] static auto CreateRoundRectWithPathGeometry(
			RectF rect,
			float topLeft, float topRight, float bottomLeft,
			float bottomRight) -> D2DPathGeometry;
	};
}

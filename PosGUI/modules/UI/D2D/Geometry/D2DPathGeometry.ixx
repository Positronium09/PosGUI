module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.D2DPathGeometry;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.GeometrySink;

export namespace PGUI::UI::D2D
{
	class D2DPathGeometry : public D2DGeometry<ID2D1PathGeometry1>
	{
		public:
		D2DPathGeometry() noexcept;

		explicit(false) D2DPathGeometry(const ComPtr<ID2D1PathGeometry1>& ptr) noexcept;

		[[nodiscard]] auto GetFigureCount() const noexcept -> Result<UINT32>;

		[[nodiscard]] auto GetSegmentCount() const noexcept -> Result<UINT32>;

		[[nodiscard]] auto Open() const noexcept -> Result<GeometrySink>;
		auto Stream(const GeometrySink& sink) const noexcept -> Error;

		[[nodiscard]] static auto CreateRoundRectWithPathGeometry(
			RectF rect,
			float topLeft,
			float topRight,
			float bottomLeft,
			float bottomRight) noexcept -> Result<D2DPathGeometry>;
	};
}

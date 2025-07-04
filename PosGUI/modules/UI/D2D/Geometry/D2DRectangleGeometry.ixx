module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DRectangleGeometry;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.D2D.D2DGeometry;

export namespace PGUI::UI::D2D
{
	class D2DRectangleGeometry : public D2DGeometry<ID2D1RectangleGeometry>
	{
		public:
		constexpr D2DRectangleGeometry() noexcept = default;

		explicit(false) constexpr D2DRectangleGeometry(const ComPtr<ID2D1RectangleGeometry>& ptr) noexcept :
			D2DGeometry{ ptr }
		{ }

		explicit D2DRectangleGeometry(const RectF rect)
		{
			const auto& factory = Factories::D2DFactory::GetFactory();

			const auto hr = factory->CreateRectangleGeometry(rect, GetAddress());
			ThrowFailed(hr);
		}

		[[nodiscard]] auto GetRect() const noexcept -> RectF
		{
			D2D1_RECT_F rect{ };
			Get()->GetRect(&rect);

			return rect;
		}
	};
}

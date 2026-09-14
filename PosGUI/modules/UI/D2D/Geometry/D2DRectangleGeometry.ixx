module;
#include <d2d1_3.h>

export module PGUI.UI.D2D.D2DRectangleGeometry;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.Factories;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DGeometry;

export namespace PGUI::UI::D2D
{
	class D2DRectangleGeometry : public D2DGeometry<ID2D1RectangleGeometry>
	{
		public:
		constexpr D2DRectangleGeometry() noexcept = default;

		explicit(false) constexpr D2DRectangleGeometry(const ComPtr<ID2D1RectangleGeometry>& ptr) noexcept :
			D2DGeometry{ ptr }
		{
		}

		[[nodiscard]] static auto Create(const RectF rect) noexcept -> Result<D2DRectangleGeometry>
		{
			const auto& factory = Factories::D2DFactory::GetFactory();

			ComPtr<ID2D1RectangleGeometry> geometry;
			if (auto error = Error{ factory->CreateRectangleGeometry(rect, geometry.put()) };
				error.IsFailure())
			{
				error.AddDetail(L"rect", std::format(L"{}", rect));
				return Unexpected{ error };
			}

			return D2DRectangleGeometry{ geometry };
		}

		[[nodiscard]] auto GetRect() const noexcept -> RectF
		{
			D2D1_RECT_F rect{ };
			Get()->GetRect(&rect);

			return rect;
		}
	};
}

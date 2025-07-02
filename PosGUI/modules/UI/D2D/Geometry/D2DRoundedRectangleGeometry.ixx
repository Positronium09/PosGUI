module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DRoundedRectangleGeometry;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.D2D.D2DGeometry;

export namespace PGUI::UI::D2D
{
	class D2DRoundedRectangleGeometry : public D2DGeometry<ID2D1RoundedRectangleGeometry>
	{
		public:
		constexpr D2DRoundedRectangleGeometry() noexcept = default;

		explicit(false) constexpr D2DRoundedRectangleGeometry(const ComPtr<ID2D1RoundedRectangleGeometry>& ptr) noexcept :
			D2DGeometry{ ptr }
		{ }

		explicit D2DRoundedRectangleGeometry(const RoundedRect& roundedRect)
		{
			const auto& factory = Factories::D2DFactory::GetFactory();

			const auto hr = factory->CreateRoundedRectangleGeometry(roundedRect, GetAddress());
			ThrowFailed(hr);
		}

		[[nodiscard]] auto GetRoundedRect() const noexcept -> RoundedRect
		{
			D2D1_ROUNDED_RECT roundedRect{ };
			Get()->GetRoundedRect(&roundedRect);

			return roundedRect;
		}
	};
}

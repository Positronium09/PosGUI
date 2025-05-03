module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DEllipseGeometry;

import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.D2D.D2DGeometry;

export namespace PGUI::UI::D2D
{
	class D2DEllipseGeometry : public D2DGeometry<ID2D1EllipseGeometry>
	{
		public:
		constexpr D2DEllipseGeometry() noexcept = default;
		constexpr D2DEllipseGeometry(ComPtr<ID2D1EllipseGeometry> ptr) noexcept :
			D2DGeometry{ ptr }
		{
		}
		D2DEllipseGeometry(Ellipse ellipse)
		{
			const auto& factory = Factories::D2DFactory::GetFactory();

			auto hr = factory->CreateEllipseGeometry(ellipse, GetAddress()); ThrowFailed(hr);
		}

		[[nodiscard]] auto GetEllipse() const noexcept -> Ellipse
		{
			D2D1_ELLIPSE ellipse{ };
			Get()->GetEllipse(&ellipse);

			return ellipse;
		}
	};
}

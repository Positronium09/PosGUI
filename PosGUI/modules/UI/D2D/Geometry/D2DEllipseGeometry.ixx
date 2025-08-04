module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DEllipseGeometry;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DGeometry;

export namespace PGUI::UI::D2D
{
	class D2DEllipseGeometry : public D2DGeometry<ID2D1EllipseGeometry>
	{
		public:
		constexpr D2DEllipseGeometry() noexcept = default;

		explicit(false) constexpr D2DEllipseGeometry(const ComPtr<ID2D1EllipseGeometry>& ptr) noexcept :
			D2DGeometry{ ptr }
		{ }

		explicit D2DEllipseGeometry(const Ellipse ellipse)
		{
			const auto& factory = Factories::D2DFactory::GetFactory();

			if (const auto hr = factory->CreateEllipseGeometry(ellipse, GetAddress());
				FAILED(hr))
			{
				Logger::Error(L"Failed to create ellipse geometry with ellipse {}", ellipse);
			}
		}

		[[nodiscard]] auto GetEllipse() const noexcept -> Ellipse
		{
			D2D1_ELLIPSE ellipse{ };
			Get()->GetEllipse(&ellipse);

			return ellipse;
		}
	};
}

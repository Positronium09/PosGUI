module;
#include <d2d1_3.h>

export module PGUI.UI.D2D.D2DEllipseGeometry;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
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
		{
		}

		[[nodiscard]] static auto Create(const Ellipse ellipse) noexcept -> Result<D2DEllipseGeometry>
		{
			const auto& factory = Factories::D2DFactory::GetFactory();

			ComPtr<ID2D1EllipseGeometry> geometry;
			if (auto error = Error{ factory->CreateEllipseGeometry(ellipse, geometry.put()) };
				error.IsFailure())
			{
				error.AddDetail(L"ellipse", std::format(L"{}", ellipse));
				return Unexpected{ error };
			}

			return D2DEllipseGeometry{ geometry };
		}

		[[nodiscard]] auto GetEllipse() const noexcept -> Ellipse
		{
			D2D1_ELLIPSE ellipse{ };
			Get()->GetEllipse(&ellipse);

			return ellipse;
		}
	};
}

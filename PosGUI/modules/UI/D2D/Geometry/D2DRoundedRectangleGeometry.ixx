module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DRoundedRectangleGeometry;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.ErrorHandling;
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

			if (const auto hr = factory->CreateRoundedRectangleGeometry(roundedRect, GetAddress());
				FAILED(hr))
			{
				Logger::Error(Error{ hr }
					.AddDetail(L"RoundedRect", std::format(L"{}", roundedRect))
					.AddTag(ErrorTags::D2D),
					L"Failed to create rounded rectangle geometry");
			}
		}

		[[nodiscard]] auto GetRoundedRect() const noexcept -> RoundedRect
		{
			D2D1_ROUNDED_RECT roundedRect{ };
			Get()->GetRoundedRect(&roundedRect);

			return roundedRect;
		}
	};
}

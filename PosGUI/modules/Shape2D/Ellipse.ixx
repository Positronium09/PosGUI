module;
#include <d2d1_1.h>

export module PGUI.Shape2D:Ellipse;

import :Point;

export namespace PGUI
{
	class Ellipse
	{
		public:
		PointF center{ };
		float xRadius = 0.0F;
		float yRadius = 0.0F;

		constexpr Ellipse() noexcept = default;

		constexpr Ellipse(const PointF center, const float radius) noexcept :
			center{ center }, xRadius{ radius }, yRadius{ radius }
		{ }

		constexpr Ellipse(const PointF center, const float xRadius, const float yRadius) noexcept :
			center{ center }, xRadius{ xRadius }, yRadius{ yRadius }
		{ }

		explicit(false) constexpr Ellipse(const D2D1_ELLIPSE ellipse) noexcept :
			center{ ellipse.point }, xRadius{ ellipse.radiusX }, yRadius{ ellipse.radiusY }
		{ }

		[[nodiscard]] constexpr auto operator==(const Ellipse&) const noexcept -> bool = default;

		explicit(false) operator D2D1_ELLIPSE() const noexcept
		{
			return D2D1_ELLIPSE{ center, xRadius, yRadius };
		}
	};
}

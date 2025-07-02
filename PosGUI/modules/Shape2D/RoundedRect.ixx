module;
#include <d2d1_1.h>
#include <Windows.h>

export module PGUI.Shape2D:RoundedRect;

import std;

import :Point;
import :Size;
import :Rect;

export namespace PGUI
{
	struct RoundedRect : public RectF
	{
		public:
		float xRadius = 0.0F;
		float yRadius = 0.0F;

		//using Rect::Rect;
		constexpr RoundedRect() noexcept = default;

		constexpr RoundedRect(float left, float top, float right, float bottom, float xRadius = 0.0F, float yRadius = 0.0F) noexcept :
			Rect{ left, top, right, bottom }, xRadius{ xRadius }, yRadius{ yRadius }
		{
		}
		
		template<typename T> requires std::is_arithmetic_v<T>
		explicit(false) constexpr RoundedRect(const Rect<T>& rc, float xRadius = 0.0F, float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{
		}
		explicit(false) constexpr RoundedRect(const RECT& rc, float xRadius = 0.0F, float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{
		}
		explicit(false) constexpr RoundedRect(const D2D1_RECT_F& rc, float xRadius = 0.0F, float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{
		}
		explicit(false) constexpr RoundedRect(const D2D1_RECT_U& rc, float xRadius = 0.0F, float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{
		}
		explicit(false) constexpr RoundedRect(const D2D1_ROUNDED_RECT& rrc) noexcept :
			Rect<float>{ rrc.rect }, xRadius{ rrc.radiusX }, yRadius{ rrc.radiusY }
		{
		}

		[[nodiscard]] constexpr auto operator==(const RoundedRect& other) const noexcept -> bool = default;

		explicit(false) operator D2D1_ROUNDED_RECT() const noexcept
		{
			return D2D1_ROUNDED_RECT{ *this, xRadius, yRadius };
		}
	};
}

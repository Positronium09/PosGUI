module;
#include <d2d1_1.h>
#include <wincodec.h>
#include <Windows.h>

#undef min
#undef max

export module PGUI.Shape2D:Rect;

import std;

import :Point;
import :Size;

export namespace PGUI
{
	template <typename T> requires std::is_arithmetic_v<T>
	struct Rect
	{
		T left = static_cast<T>(0);
		T top = static_cast<T>(0);
		T right = static_cast<T>(0);
		T bottom = static_cast<T>(0);

		constexpr Rect() noexcept = default;

		constexpr Rect(T left, T top, T right, T bottom) noexcept :
			left{ left }, top{ top }, right{ right }, bottom{ bottom }
		{ }

		explicit(false) constexpr Rect(const RECT& rc) noexcept :
			left{ static_cast<T>(rc.left) }, top{ static_cast<T>(rc.top) },
			right{ static_cast<T>(rc.right) }, bottom{ static_cast<T>(rc.bottom) }
		{ }

		explicit(false) constexpr Rect(const D2D1_RECT_F& rc) noexcept :
			left{ static_cast<T>(rc.left) }, top{ static_cast<T>(rc.top) },
			right{ static_cast<T>(rc.right) }, bottom{ static_cast<T>(rc.bottom) }
		{ }

		explicit(false) constexpr Rect(const D2D1_RECT_U& rc) noexcept :
			left{ static_cast<T>(rc.left) }, top{ static_cast<T>(rc.top) },
			right{ static_cast<T>(rc.right) }, bottom{ static_cast<T>(rc.bottom) }
		{ }

		explicit(false) constexpr Rect(const WICRect& rc) noexcept :
			Rect{ Point{ rc.X, rc.Y }, PGUI::Size{ rc.Width, rc.Width } }
		{ }

		constexpr Rect(Point<T> topLeft, Point<T> bottomRight) noexcept :
			Rect{ topLeft.x, topLeft.y, bottomRight.x, bottomRight.y }
		{ }

		constexpr Rect(T width, T height) noexcept :
			Rect{ { }, { width, height } }
		{ }

		constexpr Rect(Point<T> position, Size<T> size) noexcept :
			left{ position.x }, top{ position.y },
			right{ position.x + size.cx }, bottom{ position.y + size.cy }
		{ }

		~Rect() noexcept = default;

		[[nodiscard]] constexpr auto operator==(const Rect& other) const noexcept -> bool = default;

		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return Rect{ left * factor, top * factor, right * factor, bottom * factor };
		}

		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return Rect{ left / factor, top / factor, right / factor, bottom / factor };
		}

		constexpr auto& operator*=(T factor) noexcept
		{
			left *= factor;
			top *= factor;
			right *= factor;
			bottom *= factor;
			return *this;
		}
		constexpr auto& operator/=(T factor) noexcept
		{
			left /= factor;
			top /= factor;
			right /= factor;
			bottom /= factor;
			return *this;
		}

		[[nodiscard]] constexpr auto TopLeft() const noexcept
		{
			return Point<T>{ left, top };
		}

		[[nodiscard]] constexpr auto BottomLeft() const noexcept
		{
			return Point<T>{ left, bottom };
		}

		[[nodiscard]] constexpr auto TopRight() const noexcept
		{
			return Point<T>{ right, top };
		}

		[[nodiscard]] constexpr auto BottomRight() const noexcept
		{
			return Point<T>{ right, bottom };
		}

		[[nodiscard]] constexpr auto Width() const noexcept
		{
			return right - left;
		}

		[[nodiscard]] constexpr auto Height() const noexcept
		{
			return bottom - top;
		}

		[[nodiscard]] constexpr auto Size() const noexcept
		{
			return PGUI::Size<T>{ Width(), Height() };
		}

		[[nodiscard]] constexpr auto IsPointInside(Point<T> p) const noexcept
		{
			return
				left <= p.x && p.x <= right &&
				top <= p.y && p.y <= bottom;
		}

		[[nodiscard]] constexpr auto IsIntersectingRect(Rect rect) const noexcept
		{
			return left < rect.right
			       && right > rect.left
			       && top < rect.bottom
			       && bottom > top;
		}

		[[nodiscard]] constexpr auto IntersectRect(Rect rect) const noexcept
		{
			if (!IsIntersectingRect(rect))
			{
				return Rect{ };
			}
			return Rect{
				std::max(left, rect.left),
				std::max(top, rect.top),
				std::min(right, rect.right),
				std::min(bottom, rect.bottom)
			};
		}

		constexpr auto Area() const noexcept
		{
			auto size = Size();
			return size.cx * size.cy;
		}

		constexpr auto Shift(T xOffset, T yOffset) noexcept -> void
		{
			left += xOffset;
			right += xOffset;
			top += yOffset;
			bottom += yOffset;
		}

		constexpr auto Shift(Point<T> offset) noexcept -> void
		{
			Shift(offset.x, offset.y);
		}

		[[nodiscard]] constexpr auto Shifted(T xOffset, T yOffset) const noexcept
		{
			auto rect = *this;
			rect.Shift(xOffset, yOffset);
			return rect;
		}

		constexpr auto Shifted(Point<T> offset) noexcept
		{
			return Shifted(offset.x, offset.y);
		}

		constexpr auto Inflate(T dx, T dy) noexcept -> void
		{
			left += -dx;
			right += dx;
			top -= dy;
			bottom += dy;
		}

		[[nodiscard]] constexpr auto Inflated(T dx, T dy) const noexcept
		{
			auto rect = *this;
			rect.Inflate(dx, dy);
			return rect;
		}

		[[nodiscard]] constexpr auto Center() const noexcept
		{
			return Point<T>((left + right) / 2, (top + bottom) / 2);
		}

		constexpr auto CenterAround(Point<T> p) noexcept -> void
		{
			auto size = Size() / static_cast<T>(2);

			left = p.x - size.cx;
			right = p.x + size.cx;
			top = p.y - size.cy;
			bottom = p.y + size.cy;
		}

		[[nodiscard]] constexpr auto CenteredAround(Point<T> p) const noexcept
		{
			auto size = Size() / static_cast<T>(2);

			return Rect{
				p.x - size.cx,
				p.y - size.cy,
				p.x + size.cx,
				p.y + size.cy
			};
		}

		template <typename U> requires std::is_arithmetic_v<U>
		explicit(false) constexpr operator Rect<U>() const noexcept
		{
			return Rect<U>{
				static_cast<U>(left),
				static_cast<U>(top),
				static_cast<U>(right),
				static_cast<U>(bottom)
			};
		}

		explicit(false) constexpr operator RECT() const noexcept
		{
			return RECT{
				static_cast<LONG>(left),
				static_cast<LONG>(top),
				static_cast<LONG>(right),
				static_cast<LONG>(bottom)
			};
		}

		explicit(false) constexpr operator D2D1_RECT_F() const noexcept
		{
			return D2D1_RECT_F{
				static_cast<FLOAT>(left),
				static_cast<FLOAT>(top),
				static_cast<FLOAT>(right),
				static_cast<FLOAT>(bottom)
			};
		}

		explicit(false) constexpr operator D2D1_RECT_U() const noexcept
		{
			return D2D1_RECT_U{
				static_cast<UINT32>(left),
				static_cast<UINT32>(top),
				static_cast<UINT32>(right),
				static_cast<UINT32>(bottom)
			};
		}

		explicit(false) constexpr operator WICRect() const noexcept
		{
			auto size = Size();

			return WICRect{
				static_cast<INT>(left),
				static_cast<INT>(top),
				static_cast<INT>(size.cx),
				static_cast<INT>(size.cy)
			};
		}
	};

	template <std::floating_point T>
	[[nodiscard]] constexpr auto InfiniteRect() noexcept
	{
		constexpr auto infinity = std::numeric_limits<T>::infinity();
		return Rect<T>{
			-infinity,
			-infinity,
			infinity,
			infinity
		};
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator*(const Rect<T>& v, T factor) noexcept
	{
		return Rect{ v.left * factor, v.top * factor, v.right * factor, v.bottom * factor };
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator/(const Rect<T>& v, T factor) noexcept
	{
		return Rect{ v.left / factor, v.top / factor, v.right / factor, v.bottom / factor };
	}

	using RectF = Rect<float>;
	using RectI = Rect<int>;
	using RectL = Rect<long>;
	using RectU = Rect<std::uint32_t>;
}

export template <typename T, typename Char>
struct std::formatter<PGUI::Rect<T>, Char>
{
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto iter = ctx.begin();
		const auto end = ctx.end();
		if (iter == end || *iter == '}')
		{
			return iter;
		}
		throw std::format_error{ "No formatting args supported for Rect<T>" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Rect<T>& rect, FormatContext& ctx) const
	{
		if constexpr (std::floating_point<T>)
		{
			return std::format_to(
				ctx.out(), 
				"top: {:.5f}, left: {:.5f}, bottom: {:.5f}, right: {:.5f}", 
				rect.top, rect.left, rect.bottom, rect.right);
		}
		else
		{
			return std::format_to(
				ctx.out(),
				"top: {}, left: {}, bottom: {}, right: {}",
				rect.top, rect.left, rect.bottom, rect.right);
		}
	}
};

module;
#include <d2d1_1.h>
#include <Windows.h>

export module PGUI.Shape2D:Point;

import std;

export namespace PGUI
{
	template <typename T> requires std::is_arithmetic_v<T>
	struct Point
	{
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);

		[[nodiscard]] static auto Distance(const Point a, const Point b) noexcept
		{
			return static_cast<T>(std::sqrtl(static_cast<long double>(DistanceSqr(a, b))));
		}

		[[nodiscard]] static constexpr auto DistanceSqr(Point a, Point b) noexcept
		{
			return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
		}

		[[nodiscard]] static constexpr auto Lerp(Point a, Point b, T t) noexcept
		{
			return Point{
				static_cast<T>(std::lerp(a.x, b.x, t)),
				static_cast<T>(std::lerp(a.y, b.y, t))
			};
		}

		constexpr Point() noexcept = default;

		constexpr Point(const T& x, const T& y) noexcept :
			x{ x }, y{ y }
		{ }

		explicit(false) constexpr Point(const POINT& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point(const POINTS& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point(const D2D1_POINT_2F& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point(const D2D1_POINT_2U& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		constexpr auto& operator+=(const Point& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		constexpr auto& operator-=(const Point& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		constexpr auto& operator*=(const T& factor) noexcept
		{
			x *= factor;
			y *= factor;
			return *this;
		}

		constexpr auto& operator/=(const T& factor) noexcept
		{
			x /= factor;
			y /= factor;
			return *this;
		}

		[[nodiscard]] constexpr auto operator+(const Point& other) const noexcept
		{
			return Point{ x + other.x, y + other.y };
		}

		[[nodiscard]] constexpr auto operator-(const Point& other) const noexcept
		{
			return Point{ x - other.x, y - other.y };
		}

		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return Point{ x * factor, y * factor };
		}

		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return Point{ x / factor, y / factor };
		}

		[[nodiscard]] constexpr auto operator-() const noexcept
		{
			return Point{ -x, -y };
		}

		constexpr auto operator+() const noexcept -> Point
		{
			return *this;
		}

		[[nodiscard]] constexpr auto operator==(const Point&) const noexcept -> bool = default;

		[[nodiscard]] auto Distance(const Point<T>& other) const noexcept
		{
			return Point::Distance(*this, other);
		}

		[[nodiscard]] constexpr auto DistanceSqr(const Point& other) const noexcept
		{
			return Point::DistanceSqr(*this, other);
		}

		template<typename F> requires std::is_arithmetic_v<F>
		auto Rotate(const F angleDegrees, Point point = Point{ }) noexcept -> void
		{
			x -= point.x;
			y -= point.y;

			const long double angleRadians = angleDegrees / 180.0 * std::numbers::pi;
			const long double prevX = x;
			const long double prevY = y;
			x = static_cast<T>(prevX * std::cosl(angleRadians) - prevY * std::sinl(angleRadians));
			y = static_cast<T>(prevX * std::sinl(angleRadians) + prevY * std::cosl(angleRadians));

			x += point.x;
			y += point.y;
		}

		template<typename F> requires std::is_arithmetic_v<F>
		[[nodiscard]] auto Rotated(const F angleDegrees, Point centerPoint = Point{ }) noexcept
		{
			auto point = *this;
			point.Rotate(angleDegrees, centerPoint);
			return point;
		}

		constexpr auto Shift(const T dx, const T dy) noexcept -> void
		{
			x += dx;
			y += dy;
		}

		constexpr auto Shift(const Point& other) noexcept -> void
		{
			x += other.x;
			y += other.y;
		}

		[[nodiscard]] constexpr auto Shifted(const T dx, const T dy) const noexcept
		{
			return Point{ x + dx, y + dy };
		}

		[[nodiscard]] constexpr auto Shifted(const Point& other) const noexcept
		{
			auto point = *this;
			point.Shift(other);
			return point;
		}

		[[nodiscard]] constexpr auto IsZero() const noexcept -> bool
		{
			return x == static_cast<T>(0) && y == static_cast<T>(0);
		}

		template <typename U> requires std::is_arithmetic_v<U>
		explicit(false) constexpr operator Point<U>() const noexcept
		{
			return Point<U>{ static_cast<U>(x), static_cast<U>(y) };
		}

		explicit(false) constexpr operator POINT() const noexcept
		{
			return POINT{ static_cast<LONG>(x), static_cast<LONG>(y) };
		}

		explicit(false) constexpr operator POINTS() const noexcept
		{
			return POINTS{ static_cast<SHORT>(x), static_cast<SHORT>(y) };
		}

		explicit(false) constexpr operator D2D1_POINT_2F() const noexcept
		{
			return D2D1_POINT_2F{ static_cast<FLOAT>(x), static_cast<FLOAT>(y) };
		}

		explicit(false) constexpr operator D2D1_POINT_2U() const noexcept
		{
			return D2D1_POINT_2U{ static_cast<UINT32>(x), static_cast<UINT32>(y) };
		}
	};

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator*(T factor, const Point<T>& v) noexcept
	{
		return Point<T>(v.x * factor, v.y * factor);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator/(T factor, const Point<T>& v) noexcept
	{
		return Point<T>(v.x / factor, v.y / factor);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator*(const Point<T>& v, T factor) noexcept
	{
		return Point<T>(v.x * factor, v.y * factor);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator/(const Point<T>& v, T factor) noexcept
	{
		return Point<T>(v.x / factor, v.y / factor);
	}

	using PointF = Point<float>;
	using PointI = Point<int>;
	using PointL = Point<long>;
	using PointU = Point<std::uint32_t>;
}

export template <typename T, typename Char>
struct std::formatter<PGUI::Point<T>, Char>
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
		throw std::format_error{ "No formatting args supported for Point<T>" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Point<T>& point, FormatContext& ctx) const
	{
		if constexpr (std::floating_point<T>)
		{
			return std::format_to(ctx.out(), "x: {:.5f}, y: {:.5f}", point.x, point.y);
		}
		else
		{
			return std::format_to(ctx.out(), "x: {}, y: {}", point.x, point.y);
		}
	}
};

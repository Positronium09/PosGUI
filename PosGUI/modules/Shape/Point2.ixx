module;
#include <d2d1_1.h>
#include <Windows.h>
#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.Shape:Point2;

import std;

namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI
{
	template <typename T> requires std::is_arithmetic_v<T>
	struct Point2
	{
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);

		[[nodiscard]] static auto Distance(const Point2 a, const Point2 b) noexcept
		{
			return static_cast<T>(std::sqrtl(static_cast<long double>(DistanceSqr(a, b))));
		}

		[[nodiscard]] static constexpr auto DistanceSqr(Point2 a, Point2 b) noexcept
		{
			return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
		}

		[[nodiscard]] static constexpr auto Lerp(Point2 a, Point2 b, T t) noexcept
		{
			return Point2{
				static_cast<T>(std::lerp(a.x, b.x, t)),
				static_cast<T>(std::lerp(a.y, b.y, t))
			};
		}

		constexpr Point2() noexcept = default;

		constexpr Point2(const T& x, const T& y) noexcept :
			x{ x }, y{ y }
		{ }

		explicit(false) constexpr Point2(const WFN::float2& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point2(const POINT& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point2(const POINTS& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point2(const D2D1_POINT_2F& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		explicit(false) constexpr Point2(const D2D1_POINT_2U& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }
		{ }

		[[nodiscard]] auto Distance(const Point2& other) const noexcept
		{
			return Point2::Distance(*this, other);
		}

		[[nodiscard]] constexpr auto DistanceSqr(const Point2& other) const noexcept
		{
			return Point2::DistanceSqr(*this, other);
		}

		template<typename F> requires std::is_arithmetic_v<F>
		auto Rotate(const F rotationAngle, Point2 point = Point2{ }) noexcept -> void
		{
			x -= point.x;
			y -= point.y;

			const long double angleRadians = static_cast<double>(rotationAngle);
			const long double prevX = x;
			const long double prevY = y;
			const auto cos = std::cosl(angleRadians);
			const auto sin = std::sinl(angleRadians);

			x = static_cast<T>(prevX * cos - prevY * sin);
			y = static_cast<T>(prevX * sin + prevY * cos);

			x += point.x;
			y += point.y;
		}

		template<typename F> requires std::is_arithmetic_v<F>
		[[nodiscard]] auto Rotated(const F rotationAngle, Point2 centerPoint = Point2{ }) const noexcept
		{
			auto point = *this;
			point.Rotate(rotationAngle, centerPoint);
			return point;
		}

		constexpr auto Shift(const T dx, const T dy) noexcept -> void
		{
			x += dx;
			y += dy;
		}

		constexpr auto Shift(const Point2& other) noexcept -> void
		{
			x += other.x;
			y += other.y;
		}

		[[nodiscard]] constexpr auto Shifted(const T dx, const T dy) const noexcept
		{
			return Point2{ x + dx, y + dy };
		}

		[[nodiscard]] constexpr auto Shifted(const Point2& other) const noexcept
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
		explicit(false) constexpr operator Point2<U>() const noexcept
		{
			return Point2<U>{ static_cast<U>(x), static_cast<U>(y) };
		}

		explicit(false) constexpr operator Point2() const noexcept
		{
			return Point2{ static_cast<LONG>(x), static_cast<LONG>(y) };
		}

		explicit(false) constexpr operator POINTS() const noexcept
		{
			return POINTS{ static_cast<SHORT>(x), static_cast<SHORT>(y) };
		}

		explicit(false) constexpr operator POINT() const noexcept
		{
			return POINT{ static_cast<LONG>(x), static_cast<LONG>(y) };
		}

		explicit(false) constexpr operator D2D1_POINT_2F() const noexcept
		{
			return D2D1_POINT_2F{ static_cast<FLOAT>(x), static_cast<FLOAT>(y) };
		}

		explicit(false) constexpr operator D2D1_POINT_2U() const noexcept
		{
			return D2D1_POINT_2U{ static_cast<UINT32>(x), static_cast<UINT32>(y) };
		}

		explicit(false) constexpr operator WFN::float2() const noexcept
		{
			return WFN::float2{ static_cast<float>(x), static_cast<float>(y) };
		}

		explicit(false) constexpr operator WFN::float3() const noexcept
		{
			return WFN::float3{ static_cast<float>(x), static_cast<float>(y), 0.0F };
		}

		explicit(false) constexpr operator WFN::float4() const noexcept
		{
			return WFN::float4{ static_cast<float>(x), static_cast<float>(y), 0.0F, 0.0F };
		}

		constexpr auto operator+() const noexcept -> Point2
		{
			return *this;
		}

		[[nodiscard]] constexpr auto operator-() const noexcept requires std::is_signed_v<T>
		{
			return Point2{ -x, -y };
		}

		[[nodiscard]] constexpr auto operator+(const Point2& other) const noexcept
		{
			return Point2{ x + other.x, y + other.y };
		}
		constexpr auto& operator+=(const Point2& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		[[nodiscard]] constexpr auto operator-(const Point2& other) const noexcept
		{
			return Point2{ x - other.x, y - other.y };
		}
		constexpr auto& operator-=(const Point2& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return Point2{ x * factor, y * factor };
		}
		constexpr auto& operator*=(const T& factor) noexcept
		{
			x *= factor;
			y *= factor;
			return *this;
		}
		[[nodiscard]] friend constexpr auto operator*(const T factor, const Point2& v) noexcept
		{
			return v * factor;
		}

		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return Point2{ x / factor, y / factor };
		}
		constexpr auto& operator/=(const T& factor) noexcept
		{
			x /= factor;
			y /= factor;
			return *this;
		}

		[[nodiscard]] constexpr auto operator==(const Point2&) const noexcept -> bool = default;
	};

	using Point2F = Point2<float>;
	using Point2I = Point2<int>;
	using Point2L = Point2<long>;
	using Point2U = Point2<std::uint32_t>;

	template <typename T>
	using Point = Point2<T>;

	using PointF = Point2F;
	using PointI = Point2I;
	using PointL = Point2L;
	using PointU = Point2U;
}

template <typename T, typename Char>
struct std::formatter<PGUI::Point2<T>, Char>
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
		throw std::format_error{ "No formatting args supported for Point2<T>" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Point2<T>& point, FormatContext& ctx) const
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

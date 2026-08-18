module;
#include <d2d1_1.h>
#include <Windows.h>
#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.Shape:Point3;

import std;

import :Point2;

namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI
{
	template <typename T> requires std::is_arithmetic_v<T>
	struct Point3
	{
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);

		[[nodiscard]] static auto Distance(const Point3 a, const Point3 b) noexcept
		{
			return static_cast<T>(std::sqrtl(static_cast<long double>(DistanceSqr(a, b))));
		}

		[[nodiscard]] static constexpr auto DistanceSqr(Point3 a, Point3 b) noexcept
		{
			return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
		}

		[[nodiscard]] static constexpr auto Lerp(Point3 a, Point3 b, T t) noexcept
		{
			return Point3{
				static_cast<T>(std::lerp(a.x, b.x, t)),
				static_cast<T>(std::lerp(a.y, b.y, t)),
				static_cast<T>(std::lerp(a.z, b.z, t))
			};
		}

		constexpr Point3() noexcept = default;

		constexpr Point3(T x, T y, T z = static_cast<T>(0)) noexcept :
			x{ x }, y{ y }, z{ z }
		{ }

		template <typename U>
		explicit(false) constexpr Point3(const Point2<U>& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(0) }
		{ }

		explicit(false) constexpr Point3(const WFN::float2& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(0) }
		{ }

		explicit(false) constexpr Point3(const WFN::float3& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(p.z) }
		{ }

		explicit(false) constexpr Point3(const POINT& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(0) }
		{ }

		explicit(false) constexpr Point3(const POINTS& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(0) }
		{ }

		explicit(false) constexpr Point3(const D2D1_POINT_2F& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(0) }
		{ }

		explicit(false) constexpr Point3(const D2D1_POINT_2U& p) noexcept :
			x{ static_cast<T>(p.x) }, y{ static_cast<T>(p.y) }, z{ static_cast<T>(0) }
		{ }

		[[nodiscard]] auto Distance(const Point3& other) const noexcept
		{
			return Point3::Distance(*this, other);
		}

		[[nodiscard]] constexpr auto DistanceSqr(const Point3& other) const noexcept
		{
			return Point3::DistanceSqr(*this, other);
		}

		template <typename F> requires std::is_arithmetic_v<F>
		auto& RotateX(const F rotationAngle, Point3 point = Point3{ }) noexcept
		{
			y -= point.y;
			z -= point.z;

			const long double angleRadians = static_cast<double>(rotationAngle);

			const long double prevY = y;
			const long double prevZ = z;
			const auto cos = std::cosl(angleRadians);
			const auto sin = std::sinl(angleRadians);

			y = static_cast<T>(prevY * cos - prevZ * sin);
			z = static_cast<T>(prevY * sin + prevZ * cos);
		}

		template <typename F> requires std::is_arithmetic_v<F>
		auto& RotateY(const F rotationAngle, Point3 point = Point3{ }) noexcept
		{
			x -= point.x;
			z -= point.z;

			const long double angleRadians = static_cast<double>(rotationAngle);

			const long double prevX = x;
			const long double prevZ = z;
			const auto cos = std::cosl(angleRadians);
			const auto sin = std::sinl(angleRadians);

			x = static_cast<T>(prevZ * sin + prevX * cos);
			z = static_cast<T>(prevZ * cos - prevX * sin);
		}

		template <typename F> requires std::is_arithmetic_v<F>
		auto& RotateZ(const F rotationAngle, Point3 point = Point3{ }) noexcept
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

			return *this;
		}

		template <typename F> requires std::is_arithmetic_v<F>
		[[nodiscard]] auto RotatedX(const F rotationAngle, Point3 point = Point3{ }) const noexcept
		{
			auto p = *this;
			p.RotateX(rotationAngle, point);
			return p;
		}

		template <typename F> requires std::is_arithmetic_v<F>
		[[nodiscard]] auto RotatedY(const F rotationAngle, Point3 point = Point3{ }) const noexcept
		{
			auto p = *this;
			p.RotateY(rotationAngle, point);
			return p;
		}

		template <typename F> requires std::is_arithmetic_v<F>
		[[nodiscard]] auto RotatedZ(const F rotationAngle, Point3 point = Point3{ }) const noexcept
		{
			auto p = *this;
			p.RotateZ(rotationAngle, point);
			return p;
		}

		auto Shift(const T dx, const T dy, const T dz) noexcept -> void
		{
			x += dx;
			y += dy;
			z += dz;
		}

		auto Shift(const Point3& other) noexcept -> void
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}

		[[nodiscard]] auto Shifted(const T dx, const T dy, const T dz) const noexcept
		{
			return Point3{ x + dx, y + dy, z + dz };
		}
		[[nodiscard]] auto Shifted(const Point3& other) const noexcept
		{
			auto point = *this;
			point.Shift(other);
			return point;
		}

		[[nodiscard]] auto IsZero() const noexcept
		{
			return x == static_cast<T>(0) && y == static_cast<T>(0) && z == static_cast<T>(0);
		}


		template <typename U> requires std::is_arithmetic_v<U>
		explicit(false) constexpr operator Point3<U>() const noexcept
		{
			return Point3<U>{ static_cast<U>(x), static_cast<U>(y), static_cast<U>(z) };
		}

		template <typename U> requires std::is_arithmetic_v<U>
		explicit(false) constexpr operator Point2<U>() const noexcept
		{
			return Point2<U>{ static_cast<U>(x), static_cast<U>(y) };
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

		explicit(false) constexpr operator WFN::float2() const noexcept
		{
			return WFN::float2{ static_cast<float>(x), static_cast<float>(y) };
		}

		explicit(false) constexpr operator WFN::float3() const noexcept
		{
			return WFN::float3{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };
		}

		explicit(false) constexpr operator WFN::float4() const noexcept
		{
			return WFN::float4{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0F };
		}

		constexpr auto operator+() const noexcept -> Point3
		{
			return *this;
		}

		[[nodiscard]] constexpr auto operator-() const noexcept requires std::is_signed_v<T>
		{
			return Point3{ -x, -y, -z };
		}

		[[nodiscard]] constexpr auto operator+(const Point3& other) const noexcept
		{
			return Point3{ x + other.x, y + other.y, z + other.z };
		}
		constexpr auto& operator+=(const Point3& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		[[nodiscard]] constexpr auto operator-(const Point3& other) const noexcept
		{
			return Point3{ x - other.x, y - other.y, z - other.z };
		}
		constexpr auto& operator-=(const Point3& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return Point3{ x * factor, y * factor, z * factor };
		}
		constexpr auto& operator*=(const T& factor) noexcept
		{
			x *= factor;
			y *= factor;
			z *= factor;
			return *this;
		}
		[[nodiscard]] friend constexpr auto operator*(const T factor, const Point3& v) noexcept
		{
			return v * factor;
		}

		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return Point3{ x / factor, y / factor, z / factor };
		}
		constexpr auto& operator/=(const T& factor) noexcept
		{
			x /= factor;
			y /= factor;
			z /= factor;
			return *this;
		}

		[[nodiscard]] constexpr auto operator==(const Point3&) const noexcept -> bool = default;
	};

	using Point3F = Point3<float>;
	using Point3I = Point3<int>;
	using Point3L = Point3<long>;
	using Point3U = Point3<std::uint32_t>;
}

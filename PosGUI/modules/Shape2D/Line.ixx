export module PGUI.Shape2D:Line;

import std;

import :Point;
import :LineSegment;

export namespace PGUI::UI
{
	template <typename T> requires std::floating_point<T>
	struct Line
	{
		T m = static_cast<T>(0);
		T c = static_cast<T>(0);

		constexpr Line() noexcept = default;

		constexpr Line(T m, T c) noexcept :
			m{ m }, c{ c }
		{ }

		constexpr Line(Point<T> p1, Point<T> p2) noexcept :
			m{ (p2.y - p1.y) / (p2.x - p1.x) }, c{ p1.y - m * p1.x }
		{ }

		explicit constexpr Line(LineSegment<T> segment) noexcept :
			Line{ segment.start, segment.end }
		{ }

		[[nodiscard]] constexpr auto Slope() const noexcept
		{
			return m;
		}

		[[nodiscard]] constexpr auto YIntercept() const noexcept
		{
			return c;
		}

		[[nodiscard]] constexpr auto XIntercept() const noexcept
		{
			return -c / m;
		}

		[[nodiscard]] constexpr auto IsVertical() const noexcept
		{
			return std::isinf(m);
		}

		[[nodiscard]] constexpr auto IsHorizontal() const noexcept
		{
			return m < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] auto IsParallel(const Line& other) const noexcept
		{
			return std::abs(m == other.m) < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] auto IsPerpendicular(const Line& other) const noexcept
		{
			return std::abs(m * other.m + 1) < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] auto Intersection(const Line& other) const noexcept -> std::expected<Point<T>, std::monostate>
		{
			if (IsParallel(other))
			{
				return std::unexpected(std::monostate{ });
			}
			if (IsVertical())
			{
				return Point<T>{ -c, other.m * -c + other.c };
			}
			if (other.IsVertical())
			{
				return Point<T>{ other.c, m * other.c + c };
			}

			auto x = (other.c - c) / (m - other.m);
			auto y = m * x + c;

			return Point<T>{ x, y };
		}

		[[nodiscard]] constexpr auto operator==(const Line& other) const noexcept -> bool = default;

		[[nodiscard]] constexpr auto operator()(T x) const noexcept
		{
			return m * x + c;
		}

		[[nodiscard]] constexpr auto operator()(Point<T> p) const noexcept
		{
			return m * p.x + c;
		}
	};

	using LineF = Line<float>;
}

export module PGUI.Shape2D:LineSegment;

import std;

import :Point;

export namespace PGUI
{
	template <typename T> requires std::floating_point<T>
	struct LineSegment
	{
		Point<T> start{ };
		Point<T> end{ };

		constexpr LineSegment() noexcept = default;

		constexpr LineSegment(const Point<T>& start, const Point<T>& end) noexcept :
			start{ start }, end{ end }
		{
		}

		[[nodiscard]] constexpr auto operator==(const LineSegment& other) const noexcept -> bool = default;
		
		[[nodiscard]] auto Length() const noexcept
		{
			return start.Distance(end);
		}
		[[nodiscard]] constexpr auto LengthSqr() const noexcept
		{
			return start.DistanceSqr(end);
		}

		[[nodiscard]] constexpr auto Slope() const noexcept
		{
			return (end.y - start.y) / (end.x - start.x);
		}
		[[nodiscard]] constexpr auto YIntercept() const noexcept
		{
			return start.y - Slope() * start.x;
		}
		[[nodiscard]] constexpr auto XIntercept() const noexcept
		{
			return -YIntercept() / Slope();
		}
		[[nodiscard]] constexpr auto IsVertical() const noexcept
		{
			return std::isinf(Slope());
		}
		[[nodiscard]] constexpr auto IsHorizontal() const noexcept
		{
			return Slope() < std::numeric_limits<T>::epsilon();
		}
		[[nodiscard]] constexpr auto Midpoint() const noexcept
		{
			return Point{ (start.x + end.x) / 2, (start.y + end.y) / 2 };
		}
		[[nodiscard]] auto Angle() const noexcept
		{
			return std::atan2(end.y - start.y, end.x - start.x);
		}

		[[nodiscard]] auto IsParallel(LineSegment lineSegment) const noexcept
		{
			return std::abs(Slope() - lineSegment.Slope()) < std::numeric_limits<T>::epsilon();
		}
		[[nodiscard]] auto IsPerpendicular(LineSegment lineSegment) const noexcept
		{
			return std::abs(Slope() * lineSegment.Slope() + 1) < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] constexpr auto IsOnLineSegment(Point<T> p) const noexcept
		{
			return p.x >= std::min(start.x, end.x) && p.x <= std::max(start.x, end.x) &&
				p.y >= std::min(start.y, end.y) && p.y <= std::max(start.y, end.y);
		}

		[[nodiscard]] auto Intersect(LineSegment lineSegment) const noexcept -> std::expected<Point<T>, std::monostate>
		{
			if (IsParallel(lineSegment))
			{
				return std::monostate{ };
			}

			auto x = (lineSegment.YIntercept() - YIntercept()) / (Slope() - lineSegment.Slope());
			auto y = Slope() * x + YIntercept();
			
			auto intersection = Point{ x, y };
		
			if (IsOnLineSegment(intersection) && lineSegment.IsOnLineSegment(intersection))
			{
				return intersection;
			}

			return std::unexpected(std::monostate{ });
		}

		[[nodiscard]] constexpr auto operator+(const Point<T>& point) const noexcept
		{
			return LineSegment{ start + point, end + point };
		}
		[[nodiscard]] constexpr auto operator-(const Point<T>& point) const noexcept
		{
			return LineSegment{ start - point, end - point };
		}
		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return LineSegment{ start * factor, end * factor };
		}
		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return LineSegment{ start / factor, end / factor };
		}
		[[nodiscard]] constexpr auto operator-() const noexcept
		{
			return LineSegment{ -start, -end };
		}
	};

	using LineSegmentF = LineSegment<float>;
}

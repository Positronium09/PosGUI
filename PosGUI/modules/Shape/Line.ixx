export module PGUI.Shape:Line;

import std;

import :Point2;
import :LineSegment;

namespace
{
	template <std::floating_point T>
	using FloatBits = std::conditional_t<sizeof(T) == sizeof(std::uint32_t), std::uint32_t, std::uint64_t>;

	template <std::floating_point T>
	[[nodiscard]] constexpr auto Abs(const T x) noexcept -> T
	{
		static_assert(sizeof(T) == sizeof(FloatBits<T>));
		constexpr auto signMask = FloatBits<T>{ 1 } << (std::numeric_limits<FloatBits<T>>::digits - 1);

		return std::bit_cast<T>(static_cast<FloatBits<T>>(std::bit_cast<FloatBits<T>>(x) & ~signMask));
	}
}

export namespace PGUI
{
	template <std::floating_point T>
	struct Line
	{
		T m = static_cast<T>(0);
		T c = static_cast<T>(0);

		constexpr Line() noexcept = default;

		constexpr Line(T m, T c) noexcept :
			m{ m }, c{ c }
		{ }

		Line(T a, T b, T c) noexcept
		{
			if (Abs(b) < std::numeric_limits<T>::epsilon())
			{
				m = std::numeric_limits<T>::infinity();
				c = -c / a;
			}
			else if (Abs(a) < std::numeric_limits<T>::epsilon())
			{
				m = static_cast<T>(0);
				c = -c / b;
			}
		}

		Line(Point<T> p1, Point<T> p2) noexcept
		{
			if (Abs(p2.x - p1.x) < std::numeric_limits<T>::epsilon())
			{
				m = std::numeric_limits<T>::infinity();
				c = -p1.x;
			}
			else if (Abs(p2.y - p1.y) < std::numeric_limits<T>::epsilon())
			{
				m = static_cast<T>(0);
				c = p1.y;
			}
		}

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
			if (IsHorizontal())
			{
				return std::numeric_limits<T>::infinity();
			}
			return -c / m;
		}

		[[nodiscard]] auto IsVertical() const noexcept
		{
			return std::isinf(m);
		}

		[[nodiscard]] auto IsHorizontal() const noexcept
		{
			return Abs(m) < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] auto IsParallel(const Line& other) const noexcept
		{
			if (IsVertical() && other.IsVertical())
			{
				return true;
			}
			return Abs(m - other.m) < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] auto IsPerpendicular(const Line& other) const noexcept
		{
			return Abs(m * other.m + 1) < std::numeric_limits<T>::epsilon();
		}

		[[nodiscard]] auto Intersection(const Line& other) const noexcept -> std::optional<Point<T>>
		{
			if (IsParallel(other))
			{
				return std::nullopt;
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

template <typename T, typename Char>
struct std::formatter<PGUI::Line<T>, Char>
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
		throw std::format_error{ "No formatting args supported for Line<T>" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Line<T>& line, FormatContext& ctx) const
	{
		return std::format_to(
			ctx.out(),
			"m: {}, c: {}", line.m, line.c);
	}
};

module;
#include <Windows.h>

export module PGUI.Utils:ArithmeticUtils;

import std;

import PGUI.Shape;

export namespace PGUI
{
	namespace Detail
	{
		template <std::floating_point T>
		using FloatBits = std::conditional_t<sizeof(T) == sizeof(std::uint32_t), std::uint32_t, std::uint64_t>;

		template <std::floating_point T>
		[[nodiscard]] constexpr auto FloatSignMask() noexcept -> FloatBits<T>
		{
			static_assert(sizeof(T) == sizeof(FloatBits<T>));
			return FloatBits<T>{ 1 } << (std::numeric_limits<FloatBits<T>>::digits - 1);
		}
	}

	template <std::signed_integral T>
	[[nodiscard]] constexpr auto Sign(T x) noexcept -> std::int32_t
	{
		if (x == 0)
		{
			return 0;
		}
		return x > 0 ? 1 : -1;
	}

	template <std::unsigned_integral T>
	[[nodiscard]] constexpr auto Sign(T x) noexcept -> std::int32_t
	{
		if (x == 0)
		{
			return 0;
		}
		return 1;
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr auto Sign(T x) noexcept -> std::int32_t
	{
		if (x == 0)
		{
			return 0;
		}
		return (std::bit_cast<Detail::FloatBits<T>>(x) & Detail::FloatSignMask<T>()) != 0 ? -1 : 1;
	}

	template <std::signed_integral T>
	[[nodiscard]] constexpr auto Abs(T x) noexcept -> T
	{
		if (x < 0)
		{
			return -x;
		}

		return x;
	}

	template <std::unsigned_integral T>
	[[nodiscard]] constexpr auto Abs(T x) noexcept -> T
	{
		return x;
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr auto Abs(T x) noexcept -> T
	{
		return std::bit_cast<T>(
			static_cast<Detail::FloatBits<T>>(std::bit_cast<Detail::FloatBits<T>>(x) & ~Detail::FloatSignMask<T>()));
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr auto MapToRange(
		T value,
		T outRangeMin, T outRangeMax, T inRangeMin = 0,
		T inRangeMax = 1) noexcept -> T
	{
		return outRangeMin + (outRangeMax - outRangeMin) * (value - inRangeMin) / (inRangeMax - inRangeMin);
	}

	[[nodiscard]] auto MapPoints(HWND from, HWND to, std::span<PointL> points) noexcept -> std::span<PointL>;

	[[nodiscard]] auto MapPoint(HWND from, HWND to, PointL point) noexcept -> PointL;

	[[nodiscard]] auto MapRects(HWND from, HWND to, std::span<RectL> rects) noexcept -> std::span<RectL>;

	[[nodiscard]] auto MapRect(HWND from, HWND to, RectL rect) noexcept -> RectL;
}

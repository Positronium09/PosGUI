module;
#include <Windows.h>

export module PGUI.Utils:ArithmeticUtils;

import std;

import PGUI.Shape2D;

export namespace PGUI
{
	template <std::integral T>
	[[nodiscard]] constexpr auto sign(T x) noexcept
	{
		if (x == 0)
		{
			return 0;
		}
		return x > 0 ? 1 : -1;
	}

	template <std::floating_point T>
	[[nodiscard]] constexpr auto sign(T x) noexcept
	{
		if (x == 0)
		{
			return 0;
		}
		return std::signbit(x) ? -1 : 1;
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

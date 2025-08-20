module;
#include <Windows.h>

module PGUI.Utils:ArithmeticUtils;

import std;

import PGUI.Shape2D;

namespace PGUI
{
	auto MapPoints(const HWND from, const HWND to, std::span<PointL> points) noexcept -> std::span<PointL>
	{
		if (points.size() == 2U)
		{
			MapWindowPoints(
				from, to,
				std::bit_cast<LPPOINT>(points.data()), 1U);
			MapWindowPoints(
				from, to,
				std::next(std::bit_cast<LPPOINT>(points.data())), 1U);

			return points;
		}

		MapWindowPoints(
			from, to,
			std::bit_cast<LPPOINT>(points.data()),
			static_cast<UINT>(points.size()));

		return points;
	}

	auto MapPoint(const HWND from, const HWND to, PointL point) noexcept -> PointL
	{
		MapWindowPoints(
			from, to,
			std::bit_cast<LPPOINT>(&point), 1U);

		return point;
	}

	auto MapRects(const HWND from, const HWND to, std::span<RectL> rects) noexcept -> std::span<RectL>
	{
		MapWindowPoints(
			from, to,
			std::bit_cast<LPPOINT>(rects.data()),
			static_cast<UINT>(rects.size()) * 2);

		return rects;
	}

	auto MapRect(const HWND from, const HWND to, RectL rect) noexcept -> RectL
	{
		MapWindowPoints(
			from, to,
			std::bit_cast<LPPOINT>(&rect), 2U);

		return rect;
	}
}

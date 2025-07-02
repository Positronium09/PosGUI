module;
#include <Windows.h>

module PGUI.Utils:ArithmeticUtils;

import std;

import PGUI.Shape2D;

namespace PGUI
{
	auto MapPoints(HWND from, HWND to, std::span<PointL> points) noexcept -> std::span<PointL>
	{
		MapWindowPoints(from, to,
			std::bit_cast<LPPOINT>(points.data()),
			static_cast<UINT>(points.size()));

		return points;
	}

	auto MapPoint(HWND from, HWND to, PointL point) noexcept -> PointL
	{
		MapWindowPoints(from, to,
			std::bit_cast<LPPOINT>(&point), 1U);

		return point;
	}

	auto MapRects(HWND from, HWND to, std::span<RectL> rects) noexcept -> std::span<RectL>
	{
		MapWindowPoints(from, to,
			std::bit_cast<LPPOINT>(rects.data()),
			static_cast<UINT>(rects.size()) * 2);

		return rects;
	}

	auto MapRect(HWND from, HWND to, RectL rect) noexcept -> RectL
	{
		MapWindowPoints(from, to,
			std::bit_cast<LPPOINT>(&rect), 2U);

		return rect;
	}
}

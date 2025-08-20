module;
#include <d2d1_1.h>
#include <Windows.h>

export module PGUI.Shape2D:RoundedRect;

import std;

import :Point;
import :Size;
import :Rect;

export namespace PGUI
{
	struct RoundedRect : RectF
	{
		float xRadius = 0.0F;
		float yRadius = 0.0F;

		//using Rect::Rect;
		constexpr RoundedRect() noexcept = default;

		constexpr RoundedRect(const float left, const float top, const float right, const float bottom,
			const float xRadius = 0.0F,
			const float yRadius = 0.0F) noexcept :
			Rect{ left, top, right, bottom }, xRadius{ xRadius }, yRadius{ yRadius }
		{ }

		template <typename T> requires std::is_arithmetic_v<T>
		explicit(false) constexpr RoundedRect(const Rect<T>& rc,
			const float xRadius = 0.0F, const float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{ }

		explicit(false) constexpr RoundedRect(const RECT& rc,
			const float xRadius = 0.0F, const float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{ }

		explicit(false) constexpr RoundedRect(const D2D1_RECT_F& rc,
			const float xRadius = 0.0F, 
			const float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{ }

		explicit(false) constexpr RoundedRect(
			const D2D1_RECT_U& rc,
			const float xRadius = 0.0F,
			const float yRadius = 0.0F) noexcept :
			Rect<float>{ rc }, xRadius{ xRadius }, yRadius{ yRadius }
		{ }

		explicit(false) constexpr RoundedRect(const D2D1_ROUNDED_RECT& rrc) noexcept :
			Rect<float>{ rrc.rect }, xRadius{ rrc.radiusX }, yRadius{ rrc.radiusY }
		{ }

		[[nodiscard]] constexpr auto operator==(const RoundedRect& other) const noexcept -> bool = default;

		explicit(false) operator D2D1_ROUNDED_RECT() const noexcept
		{
			return D2D1_ROUNDED_RECT{ *this, xRadius, yRadius };
		}
	};
}

export template <typename Char>
struct std::formatter<PGUI::RoundedRect, Char>
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
		throw std::format_error{ "No formatting args supported for RoundedRect" };
	}

	template <typename FormatContext>
	auto format(const PGUI::RoundedRect& rect, FormatContext& ctx) const
	{
		return std::format_to(
			ctx.out(),
			"Rect: (top: {:.5f}, left: {:.5f}, bottom: {:.5f}, right: {:.5f}), Radii: (x: {:.5f}, y: {:.5f})",
			rect.top, rect.left, rect.bottom, rect.right, rect.xRadius, rect.yRadius);
	}
};

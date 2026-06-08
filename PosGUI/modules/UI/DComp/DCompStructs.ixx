module;
#include <d2d1_1.h>

export module PGUI.UI.DComp.DCompStructs;

import std;

import PGUI.Shape2D;
import PGUI.UI.Color;

export namespace PGUI::UI::DComp
{
	struct Vector2F : public D2D1_VECTOR_2F
	{
		constexpr Vector2F() noexcept :
			D2D1_VECTOR_2F{ .x = 0.0F, .y = 0.0F }
		{ }

		constexpr Vector2F(const float x, const float y) noexcept :
			D2D1_VECTOR_2F{ .x = x, .y = y }
		{ }

		explicit(false) constexpr Vector2F(const D2D1_VECTOR_2F& vec) noexcept :
			D2D1_VECTOR_2F{ vec }
		{ }

		explicit(false) constexpr Vector2F(const PointF& point) noexcept :
			D2D1_VECTOR_2F{ .x = point.x, .y = point.y }
		{ }

		[[nodiscard]] constexpr auto operator==(const Vector2F& other) const noexcept -> bool
		{
			return x == other.x && y == other.y;
		}

		explicit(false) constexpr operator PointF() const noexcept
		{
			return PointF{ x, y };
		}
	};

	struct Vector4F : public D2D1_VECTOR_4F
	{
		constexpr Vector4F() noexcept :
			D2D1_VECTOR_4F{ .x = 0.0F, .y = 0.0F, .z = 0.0F, .w = 0.0F }
		{ }

		constexpr Vector4F(const float x, const float y, const float z, const float w) noexcept :
			D2D1_VECTOR_4F{ .x = x, .y = y, .z = z, .w = w }
		{ }

		explicit(false) constexpr Vector4F(const D2D1_VECTOR_4F& vec) noexcept :
			D2D1_VECTOR_4F{ vec }
		{ }

		explicit(false) constexpr Vector4F(const RGBA& color) noexcept :
			D2D1_VECTOR_4F{ .x = color.r, .y = color.g, .z = color.b, .w = color.a }
		{ }

		[[nodiscard]] constexpr auto operator==(const Vector4F& other) const noexcept -> bool
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		explicit(false) constexpr operator RGBA() const noexcept
		{
			return RGBA{ x, y, z, w };
		}
	};
}

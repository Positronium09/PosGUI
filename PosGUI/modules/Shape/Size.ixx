module;
#include <d2d1_1.h>
#include <Windows.h>
#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.Shape:Size;

import std;

namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI
{
	template <typename T> requires std::is_arithmetic_v<T>
	struct Size
	{
		T cx = static_cast<T>(0);
		T cy = static_cast<T>(0);

		constexpr Size() noexcept = default;

		constexpr Size(const T& cx, const T& cy) noexcept :
			cx{ cx }, cy{ cy }
		{ }

		explicit constexpr Size(const T& sz) noexcept :
			cx{ sz }, cy{ sz }
		{ }

		explicit(false) constexpr Size(const WFN::float2& sz) noexcept :
			cx{ static_cast<T>(sz.x) }, cy{ static_cast<T>(sz.y) }
		{ }

		explicit(false) constexpr Size(const SIZE& sz) noexcept :
			cx{ static_cast<T>(sz.cx) }, cy{ static_cast<T>(sz.cy) }
		{ }

		explicit(false) constexpr Size(const D2D1_SIZE_F& sz) noexcept :
			cx{ static_cast<T>(sz.width) }, cy{ static_cast<T>(sz.height) }
		{ }

		explicit(false) constexpr Size(const D2D1_SIZE_U& sz) noexcept :
			cx{ static_cast<T>(sz.width) }, cy{ static_cast<T>(sz.height) }
		{ }

		template <typename U> requires std::is_arithmetic_v<U>
		explicit(false) constexpr operator Size<U>() const noexcept
		{
			return Size<U>{ static_cast<U>(cx), static_cast<U>(cy) };
		}

		explicit(false) constexpr operator WFN::float2() const noexcept
		{
			return WFN::float2{ static_cast<float>(cx), static_cast<float>(cy) };
		}

		explicit(false) constexpr operator SIZE() const noexcept
		{
			return SIZE{ static_cast<LONG>(cx), static_cast<LONG>(cy) };
		}

		explicit(false) constexpr operator D2D1_SIZE_F() const noexcept
		{
			return D2D1_SIZE_F{ static_cast<FLOAT>(cx), static_cast<FLOAT>(cy) };
		}

		explicit(false) constexpr operator D2D1_SIZE_U() const noexcept
		{
			return D2D1_SIZE_U{ static_cast<UINT32>(cx), static_cast<UINT32>(cy) };
		}

		[[nodiscard]] constexpr auto operator-() const noexcept requires std::is_signed_v<T>
		{
			return Size{ -cx, -cy };
		}

		[[nodiscard]] constexpr auto operator+(const Size& other) const noexcept
		{
			return Size{ cx + other.cx, cy + other.cy };
		}
		constexpr auto& operator+=(const Size& other) noexcept
		{
			cx += other.cx;
			cy += other.cy;
			return *this;
		}

		[[nodiscard]] constexpr auto operator-(const Size& other) const noexcept
		{
			return Size{ cx - other.cx, cy - other.cy };
		}
		constexpr auto& operator-=(const Size& other) noexcept
		{
			cx -= other.cx;
			cy -= other.cy;
			return *this;
		}

		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return Size{ cx * factor, cy * factor };
		}
		constexpr auto& operator*=(const T& factor) noexcept
		{
			cx *= factor;
			cy *= factor;
			return *this;
		}
		[[nodiscard]] friend constexpr auto operator*(const T factor, const Size& v) noexcept
		{
			return v * factor;
		}

		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return Size{ cx / factor, cy / factor };
		}
		constexpr auto& operator/=(const T& factor) noexcept
		{
			cx /= factor;
			cy /= factor;
			return *this;
		}

		[[nodiscard]] constexpr auto operator==(const Size<T>& other) const noexcept -> bool = default;
	};

	template <std::floating_point T>
	[[nodiscard]] constexpr auto InfiniteSize() noexcept
	{
		constexpr auto infinity = std::numeric_limits<T>::infinity();
		return Size<T>{
			infinity,
			infinity
		};
	}

	using SizeF = Size<float>;
	using SizeI = Size<int>;
	using SizeL = Size<long>;
	using SizeU = Size<std::uint32_t>;
}

template <typename T, typename Char>
struct std::formatter<PGUI::Size<T>, Char>
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
		throw std::format_error{ "No formatting args supported for Size<T>" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Size<T>& size, FormatContext& ctx) const
	{
		if constexpr (std::floating_point<T>)
		{
			return std::format_to(ctx.out(), "cx: {:.5f}, cy: {:.5f}", size.cx, size.cy);
		}
		else
		{
			return std::format_to(ctx.out(), "cx: {}, cy: {}", size.cx, size.cy);
		}
	}
};

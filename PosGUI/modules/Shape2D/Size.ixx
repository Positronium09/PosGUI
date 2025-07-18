module;
#include <cstdint>
#include <d2d1_1.h>
#include <type_traits>
#include <Windows.h>

export module PGUI.Shape2D:Size;

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

		explicit(false) constexpr Size(const SIZE& sz) noexcept :
			cx{ static_cast<T>(sz.cx) }, cy{ static_cast<T>(sz.cy) }
		{ }

		explicit(false) constexpr Size(const D2D1_SIZE_F& sz) noexcept :
			cx{ static_cast<T>(sz.width) }, cy{ static_cast<T>(sz.height) }
		{ }

		explicit(false) constexpr Size(const D2D1_SIZE_U& sz) noexcept :
			cx{ static_cast<T>(sz.width) }, cy{ static_cast<T>(sz.height) }
		{ }

		[[nodiscard]] constexpr auto operator==(const Size<T>& other) const noexcept -> bool = default;

		constexpr auto& operator+=(const Size& other) noexcept
		{
			cx += other.cx;
			cy += other.cy;
			return *this;
		}

		constexpr auto& operator-=(const Size& other) noexcept
		{
			cx -= other.cx;
			cy -= other.cy;
			return *this;
		}

		constexpr auto& operator*=(const T& factor) noexcept
		{
			cx *= factor;
			cy *= factor;
			return *this;
		}

		constexpr auto& operator/=(const T& factor) noexcept
		{
			cx /= factor;
			cy /= factor;
			return *this;
		}

		[[nodiscard]] constexpr auto operator-() const noexcept
		{
			return Size{ -cx, -cy };
		}

		[[nodiscard]] constexpr auto operator+(const Size& other) const noexcept
		{
			return Size{ cx + other.cx, cy + other.cy };
		}

		[[nodiscard]] constexpr auto operator-(const Size& other) const noexcept
		{
			return Size{ cx - other.cx, cy - other.cy };
		}

		[[nodiscard]] constexpr auto operator*(T factor) const noexcept
		{
			return Size{ cx * factor, cy * factor };
		}

		[[nodiscard]] constexpr auto operator/(T factor) const noexcept
		{
			return Size{ cx / factor, cy / factor };
		}

		

		template <typename U> requires std::is_arithmetic_v<U>
		explicit(false) operator Size<U>() const noexcept
		{
			return Size<U>{ static_cast<U>(cx), static_cast<U>(cy) };
		}

		explicit(false) operator SIZE() const noexcept
		{
			return SIZE{ static_cast<LONG>(cx), static_cast<LONG>(cy) };
		}

		explicit(false) operator D2D1_SIZE_F() const noexcept
		{
			return D2D1_SIZE_F{ static_cast<FLOAT>(cx), static_cast<FLOAT>(cy) };
		}

		explicit(false) operator D2D1_SIZE_U() const noexcept
		{
			return D2D1_SIZE_U{ static_cast<UINT32>(cx), static_cast<UINT32>(cy) };
		}
	};

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator*(T factor, const Size<T>& v) noexcept
	{
		return Size<T>(v.cx * factor, v.cy * factor);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator/(T factor, const Size<T>& v) noexcept
	{
		return Size<T>(v.cx / factor, v.cy / factor);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator*(const Size<T>& v, T factor) noexcept
	{
		return Size<T>(v.cx * factor, v.cy * factor);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr auto operator/(const Size<T>& v, T factor) noexcept
	{
		return Size<T>(v.cx / factor, v.cy / factor);
	}

	using SizeF = Size<float>;
	using SizeI = Size<int>;
	using SizeL = Size<long>;
	using SizeU = Size<std::uint32_t>;
}

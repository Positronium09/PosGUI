module;
#include <d2d1.h>
#include <wincodec.h>
#include <Windows.h>
#include <winrt/windows.ui.viewmanagement.h>

#undef RGB
#undef CMYK

export module PGUI.UI.Color;

import std;

export namespace PGUI::UI
{
	class HSL;
	class HSV;
	class CMYK;

	class RGBA
	{
		public:
		constexpr RGBA() noexcept = default;

		constexpr RGBA(const float r, const float g, const float b, const float a = 1.0F) noexcept :
			r{ r }, g{ g }, b{ b }, a{ a }
		{ }
		constexpr RGBA(const double r, const double g, const double b, const double a = 1.0F) noexcept :
			r{ static_cast<float>(r) },
			g{ static_cast<float>(g) },
			b{ static_cast<float>(b) },
			a{ static_cast<float>(a) }
		{ }

		constexpr RGBA(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = 255) noexcept :
			r{ r / 255.0F },
			g{ g / 255.0F },
			b{ b / 255.0F },
			a{ a / 255.0F }
		{ }

		explicit(false) constexpr RGBA(const std::uint32_t rgb, const float a = 1.0F) noexcept :
			r{ ((rgb & 0xff << 16) >> 16) / 255.0F },
			g{ ((rgb & 0xff << 8) >> 8) / 255.0F },
			b{ (rgb & 0xff) / 255.0F },
			a{ a }
		{ }

		template <std::floating_point T>
		explicit constexpr RGBA(std::span<T, 3> values) :
			r{ static_cast<float>(values[0]) },
			g{ static_cast<float>(values[1]) },
			b{ static_cast<float>(values[2]) },
			a{ 1.0F }
		{ }

		template <std::floating_point T>
		explicit constexpr RGBA(std::span<T, 4> values) :
			r{ static_cast<float>(values[0]) },
			g{ static_cast<float>(values[1]) },
			b{ static_cast<float>(values[2]) },
			a{ static_cast<float>(values[3]) }
		{ }

		explicit(false) RGBA(HSL hsl) noexcept;

		explicit(false) RGBA(HSV hsv) noexcept;

		explicit(false) constexpr RGBA(const D2D1_COLOR_F& color) noexcept :
			r{ color.r }, g{ color.g }, b{ color.b }, a{ color.a }
		{ }

		explicit(false) constexpr RGBA(const winrt::Windows::UI::Color& color) noexcept :
			r{ color.R / 255.0F },
			g{ color.G / 255.0F },
			b{ color.B / 255.0F },
			a{ color.A / 255.0F }
		{ }

		explicit(false) constexpr operator D2D1_COLOR_F() const noexcept
		{
			return D2D1_COLOR_F{ .r = r, .g = g, .b = b, .a = a };
		}

		explicit(false) constexpr operator WICColor() const noexcept
		{
			return static_cast<WICColor>(a * 255) << 24 |
			       static_cast<WICColor>(r * 255) << 16 |
			       static_cast<WICColor>(g * 255) << 8 |
			       static_cast<WICColor>(b * 255);
		}

		explicit(false) constexpr operator winrt::Windows::UI::Color() const noexcept
		{
			return winrt::Windows::UI::Color{
				static_cast<BYTE>(a / 255.0F),
				static_cast<BYTE>(r / 255.0F),
				static_cast<BYTE>(g / 255.0F),
				static_cast<BYTE>(b / 255.0F)
			};
		}

		explicit(false) constexpr operator COLORREF() const noexcept
		{
			const auto R = static_cast<DWORD>(r * 255);
			const auto G = static_cast<DWORD>(g * 255);
			const auto B = static_cast<DWORD>(b * 255);

			return R | G << 8 | B << 16;
		}

		[[nodiscard]] constexpr auto operator==(const RGBA& other) const noexcept -> bool = default;

		constexpr auto Lighten(const float amount) noexcept -> void
		{
			r = std::clamp(r + amount, 0.0F, 1.0F);
			g = std::clamp(g + amount, 0.0F, 1.0F);
			b = std::clamp(b + amount, 0.0F, 1.0F);
		}

		constexpr auto Darken(const float amount) noexcept -> void
		{
			r = std::clamp(r - amount, 0.0F, 1.0F);
			g = std::clamp(g - amount, 0.0F, 1.0F);
			b = std::clamp(b - amount, 0.0F, 1.0F);
		}

		[[nodiscard]] constexpr auto Lightened(const float amount) const noexcept
		{
			auto color = *this;
			color.Lighten(amount);
			return color;
		}

		[[nodiscard]] constexpr auto Darkened(const float amount) const noexcept
		{
			auto color = *this;
			color.Darken(amount);
			return color;
		}

		[[nodiscard]] constexpr auto ToArray() const noexcept -> std::array<float, 4>
		{
			return { r, g, b, a };
		}

		template <std::floating_point T>
		[[nodiscard]] constexpr auto ToArray() const noexcept -> std::array<T, 4>
		{
			return {
				static_cast<T>(r),
				static_cast<T>(g),
				static_cast<T>(b),
				static_cast<T>(a)
			};
		}

		template <template <std::floating_point> typename Container, std::floating_point T = float>
		[[nodiscard]] constexpr auto ToContainer() const noexcept -> Container<T>
		{
			return Container<T>{
				static_cast<T>(r),
				static_cast<T>(g),
				static_cast<T>(b),
				static_cast<T>(a)
			};
		}

		float r = 0.0F;
		float g = 0.0F;
		float b = 0.0F;
		float a = 0.0F;
	};

	class HSL
	{
		public:
		constexpr HSL() noexcept = default;

		constexpr HSL(const float h, const float s, const float l) noexcept :
			h{ h }, s{ s }, l{ l }
		{ }

		explicit(false) HSL(const RGBA& rgb) noexcept;

		explicit(false) operator RGBA() const noexcept;

		[[nodiscard]] constexpr auto operator==(const HSL& other) const noexcept -> bool = default;

		float h = 0.0F;
		float s = 0.0F;
		float l = 0.0F;
	};

	class HSV
	{
		public:
		constexpr HSV() noexcept = default;

		constexpr HSV(const float h, const float s, const float v) noexcept :
			h{ h }, s{ s }, v{ v }
		{ }

		explicit(false) HSV(const RGBA& rgb) noexcept;

		explicit(false) operator RGBA() const noexcept;

		[[nodiscard]] constexpr auto operator==(const HSV& other) const noexcept -> bool = default;

		float h = 0.0F;
		float s = 0.0F;
		float v = 0.0F;
	};

	class CMYK
	{
		public:
		constexpr CMYK() noexcept = default;

		constexpr CMYK(const float c, const float m, const float y, const float k) noexcept :
			c{ c }, m{ m }, y{ y }, k{ k }
		{ }

		explicit(false) constexpr CMYK(const RGBA& rgb) noexcept
		{
			k = std::max({ rgb.r, rgb.g, rgb.b });

			c = (1 - rgb.r - k) / (1 - k);
			m = (1 - rgb.g - k) / (1 - k);
			y = (1 - rgb.b - k) / (1 - k);
		}

		explicit(false) constexpr operator RGBA() const noexcept
		{
			return RGBA{ *this };
		}

		[[nodiscard]] constexpr auto operator==(const CMYK& other) const noexcept -> bool = default;

		float c = 0.0F;
		float m = 0.0F;
		float y = 0.0F;
		float k = 0.0F;
	};

	[[nodiscard]] constexpr auto WICColorToRGBA(const WICColor color) noexcept
	{
		return RGBA{ color, ((color & 0xFF000000) >> 24) / 255.0F };
	}
}

export template <typename Char>
struct std::formatter<PGUI::UI::RGBA, Char>
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

		outFloating = false;

		const auto current = *iter;

		if (current == 'f' || current == 'F')
		{
			outFloating = true;
		}
		else if (
			current == 'h' || current == 'H' ||
			current == 'x' || current == 'X')
		{
			outHex = true;
		}
		else if (current == 'n' || current == 'N')
		{
			outNumber = true;
		}
		else if (
			current == 'e' || current == 'E' ||
			current == 'b' || current == 'B' ||
			current == '8')
		{
			outEightBit = true;
		}
		else
		{
			throw std::format_error{ "Unknown format specifier" };
		}

		std::advance(iter, 1);
		if (iter != end && *iter != '}')
		{
			throw std::format_error{ "Only one format specifier is allowed" };
		}
		if (!outFloating && !outHex && !outNumber && !outEightBit)
		{
			throw std::format_error{ "No format specifier is specified" };
		}

		return iter;
	}

	template <typename FormatContext>
	auto format(const PGUI::UI::RGBA& color, FormatContext& ctx) const
	{
		if (outFloating)
		{
			return std::format_to(ctx.out(), 
				"r: {:.3F}, g: {:.3F}, b: {:.3F}, a: {:.3F}", 
				color.r, color.g, color.b, color.a);
		}
		if (outHex)
		{
			return std::format_to(ctx.out(), 
				"#{:02X}{:02X}{:02X}{:02X}", 
				static_cast<int>(color.r * 255), 
				static_cast<int>(color.g * 255), 
				static_cast<int>(color.b * 255), 
				static_cast<int>(color.a * 255));
		}
		if (outNumber)
		{
			return std::format_to(ctx.out(), "{}",
				static_cast<int>(color.a * 255) <<  24 |
				static_cast<int>(color.r * 255) << 16 |
				static_cast<int>(color.g * 255) << 8 |
				static_cast<int>(color.b * 255));
		}
		if (outEightBit)
		{
			return std::format_to(ctx.out(), "r: {}, g: {}, b: {}, a: {}", 
				static_cast<int>(color.r * 255), 
				static_cast<int>(color.g * 255), 
				static_cast<int>(color.b * 255), 
				static_cast<int>(color.a * 255));
		}

		std::unreachable();
	}

	bool outFloating = true;
	bool outHex = false;
	bool outNumber = false;
	bool outEightBit = false;
};

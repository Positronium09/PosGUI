module;
#include <winrt/windows.ui.viewmanagement.h>

#undef RGB
#undef CMYK

module PGUI.UI.Color;

import std;

namespace PGUI::UI
{
	#pragma region RGBA

	RGBA::RGBA(const HSL hsl) noexcept :
		a(1.0F)
	{
		const auto C = (1 - std::abs(2 * hsl.l - 1)) * hsl.s;
		const auto hPrime = hsl.h / 60.0F;

		const auto X = C * (1 - std::abs(std::fmodf(hPrime, 2) - 1));

		float rPrime = 0;
		float gPrime = 0;
		float bPrime = 0;

		if (5 <= hPrime)
		{
			rPrime = C;
			bPrime = X;
		}
		else if (4 <= hPrime)
		{
			rPrime = X;
			bPrime = C;
		}
		else if (3 <= hPrime)
		{
			gPrime = X;
			bPrime = C;
		}
		else if (2 <= hPrime)
		{
			gPrime = C;
			bPrime = X;
		}
		else if (1 <= hPrime)
		{
			rPrime = X;
			gPrime = C;
		}
		else
		{
			rPrime = C;
			gPrime = X;
		}

		const auto m = hsl.l - C / 2.0F;

		r = rPrime + m;
		g = gPrime + m;
		b = bPrime + m;
	}

	RGBA::RGBA(const HSV hsv) noexcept :
		a(1.0F)
	{
		const auto C = hsv.v * hsv.s;

		const auto hPrime = hsv.h / 60.0F;

		const auto X = C * (1 - std::abs(std::fmodf(hPrime, 2) - 1));

		float rPrime = 0;
		float gPrime = 0;
		float bPrime = 0;

		if (5 <= hPrime)
		{
			rPrime = C;
			bPrime = X;
		}
		else if (4 <= hPrime)
		{
			rPrime = X;
			bPrime = C;
		}
		else if (3 <= hPrime)
		{
			gPrime = X;
			bPrime = C;
		}
		else if (2 <= hPrime)
		{
			gPrime = C;
			bPrime = X;
		}
		else if (1 <= hPrime)
		{
			rPrime = X;
			gPrime = C;
		}
		else
		{
			rPrime = C;
			gPrime = X;
		}

		const auto m = hsv.v - C;

		r = rPrime + m;
		g = gPrime + m;
		b = bPrime + m;
	}

	#pragma endregion

	#pragma region HSL

	HSL::HSL(const RGBA& rgb) noexcept
	{
		const auto cMax = std::max({ rgb.r, rgb.g, rgb.b });
		const auto cMin = std::min({ rgb.r, rgb.g, rgb.b });

		const auto delta = cMax - cMin;

		l = (cMax + cMin) / 2.0F;
		s = delta / (1 - std::abs(2 * l - 1));

		if (delta == 0)
		{
			h = 0.0F;
		}
		else if (cMax == rgb.r)
		{
			h = std::fmodf(rgb.g - rgb.b / delta, 6);
		}
		else if (cMax == rgb.g)
		{
			h = (rgb.b - rgb.r) / delta + 2;
		}
		else
		{
			h = (rgb.r - rgb.g) / delta + 4;
		}
		h *= 60.0F;
	}

	HSL::operator RGBA() const noexcept
	{
		return RGBA{ *this };
	}

	#pragma endregion

	#pragma region HSV

	HSV::HSV(const RGBA& rgb) noexcept
	{
		const auto cMax = std::max({ rgb.r, rgb.g, rgb.b });
		const auto cMin = std::min({ rgb.r, rgb.g, rgb.b });

		const auto delta = cMax - cMin;

		v = cMax;

		if (delta == 0)
		{
			h = 0.0F;
		}
		else if (cMax == rgb.r)
		{
			h = std::fmodf(rgb.g - rgb.b / delta, 6);
		}
		else if (cMax == rgb.g)
		{
			h = (rgb.b - rgb.r) / delta + 2;
		}
		else
		{
			h = (rgb.r - rgb.g) / delta + 4;
		}
		h *= 60.0F;

		if (cMax == 0)
		{
			s = 0.0F;
		}
		else
		{
			s = delta / cMax;
		}
	}

	HSV::operator RGBA() const noexcept
	{
		return RGBA{ *this };
	}

	#pragma endregion
}

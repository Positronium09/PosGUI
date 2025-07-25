module;
#include <ranges>
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.Palette;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.UI.Color;

namespace PGUI::UI::Imaging
{
	Palette::Palette()
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		const auto hr = factory->CreatePalette(GetAddress());
		ThrowFailed(hr);
	}

	Palette::Palette(const ComPtr<IWICPalette>& palette) noexcept :
		ComPtrHolder{ palette }
	{ }

	Palette::Palette(PaletteType paletteType, const bool addTransparentColor)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		auto hr = factory->CreatePalette(GetAddress());
		ThrowFailed(hr);

		hr = Get()->InitializePredefined(static_cast<WICBitmapPaletteType>(paletteType),
		                                 addTransparentColor);
		ThrowFailed(hr);
	}

	Palette::Palette(std::span<RGBA> colors)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		auto hr = factory->CreatePalette(GetAddress());
		ThrowFailed(hr);

		auto wicColors = colors | std::ranges::to<std::vector<WICColor>>();

		hr = Get()->InitializeCustom(
			wicColors.data(),
			static_cast<UINT>(wicColors.size()));
		ThrowFailed(hr);
	}

	auto Palette::GetType() const -> PaletteType
	{
		WICBitmapPaletteType type{ };
		const auto hr = Get()->GetType(&type);
		ThrowFailed(hr);

		return static_cast<PaletteType>(type);
	}

	auto Palette::GetColorCount() const -> UINT
	{
		UINT count{ };
		const auto hr = Get()->GetColorCount(&count);
		ThrowFailed(hr);

		return count;
	}

	auto Palette::GetColors() const -> std::vector<RGBA>
	{
		auto count = GetColorCount();
		std::vector<WICColor> wicColors(count);
		const auto hr = Get()->GetColors(count, wicColors.data(), &count);
		ThrowFailed(hr);

		return wicColors | std::ranges::views::transform([](auto color)
		{
			return WICColorToRGBA(color);
		}) | std::ranges::to<std::vector<RGBA>>();
	}

	auto Palette::IsBlackWhite() const -> bool
	{
		BOOL isBlackWhite{ };
		const auto hr = Get()->IsBlackWhite(&isBlackWhite);
		ThrowFailed(hr);

		return isBlackWhite;
	}

	auto Palette::IsGrayscale() const -> bool
	{
		BOOL isGrayscale{ };
		const auto hr = Get()->IsGrayscale(&isGrayscale);
		ThrowFailed(hr);

		return isGrayscale;
	}

	auto Palette::HasAlpha() const -> bool
	{
		BOOL hasAlpha{ };
		const auto hr = Get()->HasAlpha(&hasAlpha);
		ThrowFailed(hr);

		return hasAlpha;
	}
}

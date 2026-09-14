module;
#include <ranges>
#include <wincodec.h>

module PGUI.UI.Imaging.Palette;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Factories;
import PGUI.UI.Color;

namespace PGUI::UI::Imaging
{
	Palette::Palette(const ComPtr<IWICPalette>& palette) noexcept :
		ComPtrHolder{ palette }
	{ }

	auto Palette::Create() noexcept -> Result<Palette>
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		ComPtr<IWICPalette> palette;
		if (const auto error = Error{ factory->CreatePalette(palette.put()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return Palette{ palette };
	}

	auto Palette::CreateFromColors(const std::span<const RGBA> colors) noexcept -> Result<Palette>
	{
		auto paletteResult = Create();
		if (!paletteResult.has_value())
		{
			return Unexpected{ paletteResult.error() };
		}

		auto wicColors = colors | std::ranges::to<std::vector<WICColor>>();

		if (auto error = Error{
				paletteResult.value().Get()->InitializeCustom(
					wicColors.data(),
					static_cast<UINT>(wicColors.size()))
			};
			error.IsFailure())
		{
			error.AddDetail(L"colorCount", std::to_wstring(wicColors.size()));
			return Unexpected{ error };
		}

		return paletteResult;
	}

	auto Palette::CreateFromType(
		const PaletteType paletteType,
		const bool addTransparentColor) noexcept -> Result<Palette>
	{
		auto paletteResult = Create();
		if (!paletteResult.has_value())
		{
			return Unexpected{ paletteResult.error() };
		}

		if (const auto error = Error{
				paletteResult.value().Get()->InitializePredefined(
					ToUnderlying<WICBitmapPaletteType>(paletteType),
					addTransparentColor)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return paletteResult;
	}

	auto Palette::GetType() const noexcept -> Result<PaletteType>
	{
		WICBitmapPaletteType type{ };
		if (const auto hr = Get()->GetType(&type);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get palette type");
			return Unexpected{ error };
		}

		return FromUnderlying<PaletteType>(type);
	}

	auto Palette::GetColorCount() const noexcept -> Result<UINT>
	{
		UINT count{ };
		if (const auto hr = Get()->GetColorCount(&count);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get color count");
			return Unexpected{ error };
		}

		return count;
	}

	auto Palette::GetColors() const noexcept -> Result<std::vector<RGBA>>
	{
		return GetColorCount().and_then([this](const auto count) -> Result<std::vector<RGBA>>
		{
			std::vector<WICColor> wicColors(count);
			UINT written = 0;
			if (const auto hr = Get()->GetColors(count, wicColors.data(), &written);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to get colors from palette");
				return Unexpected{ error };
			}

			return wicColors | std::ranges::views::transform([](auto color)
			{
				return WICColorToRGBA(color);
			}) | std::ranges::to<std::vector<RGBA>>();
		});
	}

	auto Palette::IsBlackWhite() const noexcept -> Result<bool>
	{
		BOOL isBlackWhite{ };
		if (const auto hr = Get()->IsBlackWhite(&isBlackWhite);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to check black and white");
			return Unexpected{ error };
		}

		return static_cast<bool>(isBlackWhite);
	}

	auto Palette::IsGrayscale() const noexcept -> Result<bool>
	{
		BOOL isGrayscale{ };
		if (const auto hr = Get()->IsGrayscale(&isGrayscale);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to check grayscale");
			return Unexpected{ error };
		}

		return static_cast<bool>(isGrayscale);
	}

	auto Palette::HasAlpha() const noexcept -> Result<bool>
	{
		BOOL hasAlpha{ };
		if (const auto hr = Get()->HasAlpha(&hasAlpha);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to check alpha channel presence");
			return Unexpected{ error };
		}

		return static_cast<bool>(hasAlpha);
	}
}

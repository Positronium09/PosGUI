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
	Palette::Palette() noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreatePalette(GetAddress());
			FAILED(hr))
		{
			Logger::Error(L"Failed to create palette {}",
			              Error{ hr }
			              .AddTag(ErrorTags::Imaging)
			              .AddTag(ErrorTags::Creation));
		}
	}

	Palette::Palette(const ComPtr<IWICPalette>& palette) noexcept :
		ComPtrHolder{ palette }
	{ }

	Palette::Palette(PaletteType paletteType, const bool addTransparentColor) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreatePalette(GetAddress());
			FAILED(hr))
		{
			Logger::Error(L"Failed to create palette {}",
			              Error{ hr }
			              .AddTag(ErrorTags::Imaging)
			              .AddTag(ErrorTags::Creation));
			return;
		}

		if (const auto hr = Get()->InitializePredefined(
				static_cast<WICBitmapPaletteType>(paletteType),
				addTransparentColor);
			FAILED(hr))
		{
			Logger::Error(L"Failed to initialize palette from bitmap {}",
			              Error{ hr }
			              .AddTag(ErrorTags::Imaging)
			              .AddTag(ErrorTags::Initialization));
		}
	}

	Palette::Palette(std::span<RGBA> colors) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreatePalette(GetAddress());
			FAILED(hr))
		{
			Logger::Error(L"Failed to create palette {}",
			              Error{ hr }
			              .AddTag(ErrorTags::Imaging)
			              .AddTag(ErrorTags::Creation));
		}

		auto wicColors = colors | std::ranges::to<std::vector<WICColor>>();

		if (const auto hr = Get()->InitializeCustom(
				wicColors.data(),
				static_cast<UINT>(wicColors.size()));
			FAILED(hr))
		{
			Logger::Error(L"Failed to initialize palette from bitmap {}",
			              Error{ hr }
			              .AddTag(ErrorTags::Imaging)
			              .AddTag(ErrorTags::Initialization));
		}
	}

	auto Palette::GetType() const noexcept -> Result<PaletteType>
	{
		WICBitmapPaletteType type{ };
		if (const auto hr = Get()->GetType(&type);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return static_cast<PaletteType>(type);
	}

	auto Palette::GetColorCount() const noexcept -> Result<UINT>
	{
		UINT count{ };
		if (const auto hr = Get()->GetColorCount(&count);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Imaging)
				};
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
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return static_cast<bool>(isBlackWhite);
	}

	auto Palette::IsGrayscale() const noexcept -> Result<bool>
	{
		BOOL isGrayscale{ };
		if (const auto hr = Get()->IsGrayscale(&isGrayscale);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return static_cast<bool>(isGrayscale);
	}

	auto Palette::HasAlpha() const noexcept -> Result<bool>
	{
		BOOL hasAlpha{ };
		if (const auto hr = Get()->HasAlpha(&hasAlpha);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return static_cast<bool>(hasAlpha);
	}
}

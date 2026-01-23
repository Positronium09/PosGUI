module;
#include <wincodec.h>

export module PGUI.UI.Imaging.Palette;

import std;

import PGUI.ComPtr;
import PGUI.UI.Color;
import PGUI.ErrorHandling;
import PGUI.Factories;
import PGUI.UI.Imaging.BitmapSource;

export namespace PGUI::UI::Imaging
{
	enum class PaletteType
	{
		Grayscale = WICBitmapPaletteTypeFixedBW,
		RGB = WICBitmapPaletteTypeFixedHalftone8,
		Indexed = WICBitmapPaletteTypeFixedHalftone64,
		Gray4 = WICBitmapPaletteTypeFixedHalftone256,
		Gray16 = WICBitmapPaletteTypeFixedGray4,
		Gray256 = WICBitmapPaletteTypeFixedGray16,
		Gray2 = WICBitmapPaletteTypeFixedGray256,
	};

	class Palette : public ComPtrHolder<IWICPalette>
	{
		public:
		Palette() noexcept;

		explicit(false) Palette(const ComPtr<IWICPalette>& palette) noexcept;

		template <typename T>
		Palette(BitmapSource<T> source, UINT count, bool addTransparentColor) noexcept
		{
			const auto& factory = Factories::WICFactory::GetFactory();

			auto hr = factory->CreatePalette(Put());
			if (FAILED(hr))
			{
				Logger::Error(L"Failed to create palette");
				return;
			}

			hr = InitializeFromBitmap(source.Get(), count, addTransparentColor);
			if (FAILED(hr))
			{
				Logger::Error(L"Failed to initialize palette from bitmap");
			}
		}

		Palette(PaletteType paletteType, bool addTransparentColor) noexcept;

		explicit Palette(std::span<RGBA> colors) noexcept;

		[[nodiscard]] auto GetType() const noexcept -> Result<PaletteType>;

		[[nodiscard]] auto GetColorCount() const noexcept -> Result<UINT>;

		[[nodiscard]] auto GetColors() const noexcept -> Result<std::vector<RGBA>>;

		[[nodiscard]] auto IsBlackWhite() const noexcept -> Result<bool>;

		[[nodiscard]] auto IsGrayscale() const noexcept -> Result<bool>;

		[[nodiscard]] auto HasAlpha() const noexcept -> Result<bool>;
	};
}

module;
#include <wrl.h>
#include <wincodec.h>

export module PGUI.UI.Imaging.Palette;

import std;

import PGUI.ComPtr;
import PGUI.UI.Color;
import PGUI.Exceptions;
import PGUI.Factories;
import PGUI.UI.Imaging.BitmapSource;

export namespace PGUI::UI::Imaging
{
	enum class PaletteType
	{
		Grayscale = WICBitmapPaletteType::WICBitmapPaletteTypeFixedBW,
		RGB = WICBitmapPaletteType::WICBitmapPaletteTypeFixedHalftone8,
		Indexed = WICBitmapPaletteType::WICBitmapPaletteTypeFixedHalftone64,
		Gray4 = WICBitmapPaletteType::WICBitmapPaletteTypeFixedHalftone256,
		Gray16 = WICBitmapPaletteType::WICBitmapPaletteTypeFixedGray4,
		Gray256 = WICBitmapPaletteType::WICBitmapPaletteTypeFixedGray16,
		Gray2 = WICBitmapPaletteType::WICBitmapPaletteTypeFixedGray256,
	};

	class Palette : public ComPtrHolder<IWICPalette>
	{
		public:
		Palette();
		Palette(ComPtr<IWICPalette> palette) noexcept;
		template <typename T>
		Palette(BitmapSource<T> source, UINT count, bool addTransparentColor) noexcept
		{
			auto factory = Factories::WICFactory::GetFactory();
			
			auto hr = factory->CreatePalette(GetAddress()); ThrowFailed(hr);

			hr = InitializeFromBitmap(source.Get(), count, addTransparentColor); ThrowFailed(hr);
		}
		Palette(PaletteType paletteType, bool addTransparentColor);
		explicit Palette(std::span<RGBA> colors);

		[[nodiscard]] auto GetType() const -> PaletteType;
		[[nodiscard]] auto GetColorCount() const -> UINT;
		[[nodiscard]] auto GetColors() const -> std::vector<RGBA>;
		[[nodiscard]] auto IsBlackWhite() const -> bool;
		[[nodiscard]] auto IsGrayscale() const -> bool;
		[[nodiscard]] auto HasAlpha() const -> bool;
	};
}

module;
#include <wincodec.h>
#include <wrl.h>

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
		Palette();

		explicit(false) Palette(ComPtr<IWICPalette> palette) noexcept;

		template <typename T>
		Palette(BitmapSource<T> source, UINT count, bool addTransparentColor) noexcept
		{
			const auto& factory = Factories::WICFactory::GetFactory();

			auto hr = factory->CreatePalette(GetAddress());
			ThrowFailed(hr);

			hr = InitializeFromBitmap(source.Get(), count, addTransparentColor);
			ThrowFailed(hr);
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

module;
#include <wincodec.h>

export module PGUI.UI.Imaging.Palette;

import std;

import PGUI.ComPtr;
import PGUI.UI.Color;
import PGUI.ErrorHandling;
import PGUI.UI.Imaging.BitmapSource;

export namespace PGUI::UI::Imaging
{
	enum class PaletteType
	{
		Custom = WICBitmapPaletteTypeCustom,
		MedianCut = WICBitmapPaletteTypeMedianCut,
		BlackAndWhite = WICBitmapPaletteTypeFixedBW,
		Halftone8 = WICBitmapPaletteTypeFixedHalftone8,
		Halftone27 = WICBitmapPaletteTypeFixedHalftone27,
		Halftone64 = WICBitmapPaletteTypeFixedHalftone64,
		Halftone125 = WICBitmapPaletteTypeFixedHalftone125,
		Halftone216 = WICBitmapPaletteTypeFixedHalftone216,
		WebPalette = WICBitmapPaletteTypeFixedWebPalette,
		Halftone252 = WICBitmapPaletteTypeFixedHalftone252,
		Halftone256 = WICBitmapPaletteTypeFixedHalftone256,
		Gray4 = WICBitmapPaletteTypeFixedGray4,
		Gray16 = WICBitmapPaletteTypeFixedGray16,
		Gray256 = WICBitmapPaletteTypeFixedGray256
	};

	class Palette : public ComPtrHolder<IWICPalette>
	{
		public:
		Palette() noexcept = default;

		explicit(false) Palette(const ComPtr<IWICPalette>& palette) noexcept;

		[[nodiscard]] static auto Create() noexcept -> Result<Palette>;

		[[nodiscard]] static auto CreateFromColors(std::span<const RGBA> colors) noexcept -> Result<Palette>;

		[[nodiscard]] static auto CreateFromType(
			PaletteType paletteType,
			bool addTransparentColor) noexcept -> Result<Palette>;

		template <typename T>
		[[nodiscard]] static auto CreateFromBitmap(
			const BitmapSource<T>& source,
			const UINT count,
			const bool addTransparentColor) noexcept -> Result<Palette>
		{
			auto paletteResult = Create();
			if (!paletteResult.has_value())
			{
				return Unexpected{ paletteResult.error() };
			}

			if (auto error = Error{
					paletteResult.value().Get()->InitializeFromBitmap(
						source.GetRaw(), count, addTransparentColor)
				};
				error.IsFailure())
			{
				error.AddDetail(L"count", std::to_wstring(count));
				return Unexpected{ error };
			}

			return paletteResult;
		}

		[[nodiscard]] auto GetType() const noexcept -> Result<PaletteType>;

		[[nodiscard]] auto GetColorCount() const noexcept -> Result<UINT>;

		[[nodiscard]] auto GetColors() const noexcept -> Result<std::vector<RGBA>>;

		[[nodiscard]] auto IsBlackWhite() const noexcept -> Result<bool>;

		[[nodiscard]] auto IsGrayscale() const noexcept -> Result<bool>;

		[[nodiscard]] auto HasAlpha() const noexcept -> Result<bool>;
	};
}

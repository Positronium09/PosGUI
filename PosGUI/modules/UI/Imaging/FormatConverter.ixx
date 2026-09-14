module;
#include <wincodec.h>

export module PGUI.UI.Imaging.FormatConverter;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;

export namespace PGUI::UI::Imaging
{
	class FormatConverter : public BitmapSource<IWICFormatConverter>
	{
		public:
		FormatConverter() noexcept = default;

		explicit(false) FormatConverter(const ComPtr<IWICFormatConverter>& converter) noexcept;

		[[nodiscard]] static auto Create(
			const BitmapSource<>& bitmapSource,
			WICPixelFormatGUID destinationFormat,
			WICBitmapDitherType ditherType = WICBitmapDitherTypeNone,
			double alphaThresholdPercent = 0.0,
			const std::optional<Palette>& palette = std::nullopt,
			WICBitmapPaletteType paletteType = WICBitmapPaletteTypeCustom) noexcept -> Result<FormatConverter>;

		[[nodiscard]] static auto CreateForD2DBitmap(
			const BitmapSource<>& bitmapSource,
			WICBitmapDitherType ditherType = WICBitmapDitherTypeNone,
			double alphaThresholdPercent = 0.0,
			const std::optional<Palette>& palette = std::nullopt,
			WICBitmapPaletteType paletteType = WICBitmapPaletteTypeCustom) noexcept -> Result<FormatConverter>;

		[[nodiscard]] auto CanConvert(
			WICPixelFormatGUID sourceFormat,
			WICPixelFormatGUID destinationFormat) const noexcept -> Result<bool>;
	};
}

module;
#include <wrl.h>
#include <wincodec.h>

export module PGUI.UI.Imaging.FormatConverter;

import std;

import PGUI.ComPtr;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;

export namespace PGUI::UI::Imaging
{
	class FormatConverter : public BitmapSource<IWICFormatConverter>
	{
		public:
		FormatConverter() noexcept = default;
		FormatConverter(ComPtr<IWICFormatConverter> converter) noexcept;
		FormatConverter(BitmapSource<> bitmapSource, WICPixelFormatGUID destinationFormat,
			WICBitmapDitherType ditherType = WICBitmapDitherTypeNone, 
			double alphaThresholdPercent = 0.0F, std::optional<Palette> palette = std::nullopt, 
			WICBitmapPaletteType paletteType = WICBitmapPaletteTypeCustom);

		[[nodiscard]] auto CanConvert(WICPixelFormatGUID sourceFormat, WICPixelFormatGUID destinationFormat) const -> bool;

		[[nodiscard]] static auto ConvertToD2DBitmap(BitmapSource<> bitmapSource,
			WICBitmapDitherType ditherType = WICBitmapDitherTypeNone,
			double alphaThresholdPercent = 0.0F, std::optional<Palette> palette = std::nullopt,
			WICBitmapPaletteType paletteType = WICBitmapPaletteTypeCustom) -> FormatConverter;
	};
}

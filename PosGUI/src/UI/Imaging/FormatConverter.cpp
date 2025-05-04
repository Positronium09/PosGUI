module;
#include <optional>
#include <wrl.h>
#include <wincodec.h>

module PGUI.UI.Imaging.FormatConverter;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;


namespace PGUI::UI::Imaging
{
	FormatConverter::FormatConverter(ComPtr<IWICFormatConverter> converter) noexcept : 
		BitmapSource{ converter }
	{
	}

	FormatConverter::FormatConverter(BitmapSource<> bitmapSource, WICPixelFormatGUID destinationFormat, 
		WICBitmapDitherType ditherType, double alphaThresholdPercent, 
		std::optional<Palette> palette, WICBitmapPaletteType paletteType)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto hr = factory->CreateFormatConverter(GetAddress()); ThrowFailed(hr);

		auto srcPixelFormat = bitmapSource.GetPixelFormat();
		if (srcPixelFormat == destinationFormat)
		{
			ThrowFailed(E_INVALIDARG);
			return;
		}

		hr = Get()->Initialize(bitmapSource.GetRaw(), destinationFormat, 
			ditherType, palette.has_value() ? palette->GetRaw() : nullptr,
			alphaThresholdPercent, paletteType); ThrowFailed(hr);
	}

	auto FormatConverter::CanConvert(WICPixelFormatGUID sourceFormat, WICPixelFormatGUID destinationFormat) const -> bool
	{
		BOOL canConvert = FALSE;
		auto hr = Get()->CanConvert(sourceFormat, destinationFormat, &canConvert); ThrowFailed(hr);

		return canConvert;
	}

	auto FormatConverter::ConvertToD2DBitmap(BitmapSource<> bitmapSource, WICBitmapDitherType ditherType, double alphaThresholdPercent, std::optional<Palette> palette, WICBitmapPaletteType paletteType) -> FormatConverter
	{
		return FormatConverter{ bitmapSource, GUID_WICPixelFormat32bppPBGRA, ditherType, alphaThresholdPercent, palette, paletteType };
	}
}

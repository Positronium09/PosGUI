module;
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.FormatConverter;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;

namespace PGUI::UI::Imaging
{
	FormatConverter::FormatConverter(const ComPtr<IWICFormatConverter>& converter) noexcept :
		BitmapSource{ converter }
	{ }

	FormatConverter::FormatConverter(
		BitmapSource<> bitmapSource, const WICPixelFormatGUID destinationFormat,
		const WICBitmapDitherType ditherType, const double alphaThresholdPercent,
		std::optional<Palette> palette, const WICBitmapPaletteType paletteType)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto hr = factory->CreateFormatConverter(GetAddress());
		ThrowFailed(hr);

		if (const auto srcPixelFormat = bitmapSource.GetPixelFormat();
			srcPixelFormat == destinationFormat)
		{
			ThrowFailed(E_INVALIDARG);
			return;
		}

		hr = Get()->Initialize(bitmapSource.GetRaw(), destinationFormat,
		                       ditherType, palette.has_value() ? palette->GetRaw() : nullptr,
		                       alphaThresholdPercent, paletteType);
		ThrowFailed(hr);
	}

	auto FormatConverter::CanConvert(
		const WICPixelFormatGUID sourceFormat,
		const WICPixelFormatGUID destinationFormat) const -> bool
	{
		auto canConvert = FALSE;
		const auto hr = Get()->CanConvert(sourceFormat, destinationFormat, &canConvert);
		ThrowFailed(hr);

		return canConvert;
	}

	auto FormatConverter::ConvertToD2DBitmap(
		const BitmapSource<>& bitmapSource, const WICBitmapDitherType ditherType,
		const double alphaThresholdPercent, const std::optional<Palette>& palette,
		const WICBitmapPaletteType paletteType) -> FormatConverter
	{
		return FormatConverter{
			bitmapSource,
			GUID_WICPixelFormat32bppPBGRA,
			ditherType,
			alphaThresholdPercent,
			palette,
			paletteType
		};
	}
}

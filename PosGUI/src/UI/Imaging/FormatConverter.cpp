module;
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.FormatConverter;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.ErrorHandling;
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
		std::optional<Palette> palette, const WICBitmapPaletteType paletteType) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		if (const auto hr = factory->CreateFormatConverter(GetAddress());
			FAILED(hr))
		{
			Logger::Error(
				L"Cannot create format converted {}",
				Error{ hr }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
			return;
		}

		if (const auto srcPixelFormat = bitmapSource.GetPixelFormat();
			srcPixelFormat == destinationFormat)
		{
			Logger::Error(
				L"Source and destination pixel formats are the same {}",
				Error{ E_INVALIDARG }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
			return;
		}


		if (const auto hr = Get()->Initialize(
				bitmapSource.GetRaw(), destinationFormat,
				ditherType, palette.has_value() ? palette->GetRaw() : nullptr,
				alphaThresholdPercent, paletteType);
			FAILED(hr))
		{
			Logger::Error(
				L"Cannot initialize format converter {}",
				Error{ E_INVALIDARG }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
		}
	}

	auto FormatConverter::CanConvert(
		const WICPixelFormatGUID sourceFormat,
		const WICPixelFormatGUID destinationFormat) const noexcept -> Result<bool>
	{
		auto canConvert = FALSE;
		if (const auto hr = Get()->CanConvert(sourceFormat, destinationFormat, &canConvert);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return static_cast<bool>(canConvert);
	}

	auto FormatConverter::ConvertToD2DBitmap(
		const BitmapSource<>& bitmapSource, const WICBitmapDitherType ditherType,
		const double alphaThresholdPercent, const std::optional<Palette>& palette,
		const WICBitmapPaletteType paletteType) noexcept -> Result<FormatConverter>
	{
		FormatConverter converted{
			bitmapSource,
			GUID_WICPixelFormat32bppPBGRA,
			ditherType,
			alphaThresholdPercent,
			palette,
			paletteType
		};

		if (!converted.IsInitialized())
		{
			return Unexpected{
				Error{ E_FAIL }
				.AddTag(ErrorTags::Imaging)
				.AddTag(ErrorTags::Creation)
				.SuggestFix(L"FormatConverted constructor failed look at logs")
			};
		}

		return converted;
	}
}

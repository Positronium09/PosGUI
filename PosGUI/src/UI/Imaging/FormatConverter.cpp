module;
#include <wincodec.h>

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

	auto FormatConverter::Create(
		const BitmapSource<>& bitmapSource, const WICPixelFormatGUID destinationFormat,
		const WICBitmapDitherType ditherType, const double alphaThresholdPercent,
		const std::optional<Palette>& palette,
		const WICBitmapPaletteType paletteType) noexcept -> Result<FormatConverter>
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		ComPtr<IWICFormatConverter> converter;
		if (const auto error = Error{ factory->CreateFormatConverter(converter.put()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		if (const auto sourceFormat = bitmapSource.GetPixelFormat();
			sourceFormat == destinationFormat)
		{
			return Unexpected{
				Error{ ErrorCode::InvalidArgument }
				.AddDetail(L"destinationFormat", std::format(L"{}", destinationFormat))
				.SetCustomMessage(L"Source and destination pixel formats are the same")
			};
		}

		if (auto error = Error{
				converter->Initialize(
					bitmapSource.GetRaw(), destinationFormat,
					ditherType, palette.has_value() ? palette->GetRaw() : nullptr,
					alphaThresholdPercent, paletteType)
			};
			error.IsFailure())
		{
			error.AddDetail(L"destinationFormat", std::format(L"{}", destinationFormat));
			return Unexpected{ error };
		}

		return FormatConverter{ converter };
	}

	auto FormatConverter::CreateForD2DBitmap(
		const BitmapSource<>& bitmapSource, const WICBitmapDitherType ditherType,
		const double alphaThresholdPercent, const std::optional<Palette>& palette,
		const WICBitmapPaletteType paletteType) noexcept -> Result<FormatConverter>
	{
		auto converterResult = Create(
			bitmapSource,
			GUID_WICPixelFormat32bppPBGRA,
			ditherType,
			alphaThresholdPercent,
			palette,
			paletteType);

		if (!converterResult.has_value())
		{
			auto error = converterResult.error();
			if (const auto sourceFormat = bitmapSource.GetPixelFormat();
				sourceFormat.has_value())
			{
				error.AddDetail(L"sourceFormat", std::format(L"{}", *sourceFormat));
			}
			else
			{
				error.AddDetail(L"sourceFormat", L"Unknown");
			}
			return Unexpected{ error };
		}

		return converterResult;
	}

	auto FormatConverter::CanConvert(
		const WICPixelFormatGUID sourceFormat,
		const WICPixelFormatGUID destinationFormat) const noexcept -> Result<bool>
	{
		auto canConvert = FALSE;
		if (const auto hr = Get()->CanConvert(sourceFormat, destinationFormat, &canConvert);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Source Format", std::format(L"{}", sourceFormat))
				.AddDetail(L"Destination Format", std::format(L"{}", destinationFormat));
			return Unexpected{ error };
		}

		return static_cast<bool>(canConvert);
	}
}

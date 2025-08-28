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
				Error{ hr },
				L"Cannot create format converted"
			);
			return;
		}

		if (const auto srcPixelFormat = bitmapSource.GetPixelFormat();
			srcPixelFormat == destinationFormat)
		{
			Logger::Error(
				Error{ ErrorCode::InvalidArgument },
				L"Source and destination pixel formats are the same"
			);
			return;
		}


		if (const auto hr = Get()->Initialize(
				bitmapSource.GetRaw(), destinationFormat,
				ditherType, palette.has_value() ? palette->GetRaw() : nullptr,
				alphaThresholdPercent, paletteType);
			FAILED(hr))
		{
			Logger::Error(
				Error{ ErrorCode::InvalidArgument },
				L"Cannot initialize format converter"
			);
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
			Error error{ hr };
			error
				.AddDetail(L"Source Format", std::format(L"{}", sourceFormat))
				.AddDetail(L"Destination Format", std::format(L"{}", destinationFormat));
			Logger::Error(error, L"Failed to check if conversion is possible");
			return Unexpected{ error };
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
			Error error{ ErrorCode::Failure };
			error
				.AddDetail(L"Destination Format", std::format(L"{}", GUID_WICPixelFormat32bppPBGRA));
			if (const auto pixelFormat = bitmapSource.GetPixelFormat();
				pixelFormat.has_value())
			{
				error.AddDetail(L"Source Format", std::format(L"{}", *pixelFormat));
			}
			else
			{
				error.AddDetail(L"Source Format", L"Unknown");
			}
			Logger::Error(error, L"Failed to convert bitmap source to D2D bitmap format");
			return Unexpected{ error };
		}

		return converted;
	}
}

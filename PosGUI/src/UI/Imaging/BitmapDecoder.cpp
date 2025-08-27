module;
#include <ranges>
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.BitmapDecoder;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Factories;

namespace PGUI::UI::Imaging
{
	BitmapDecoder::BitmapDecoder(const ComPtr<IWICBitmapDecoder>& decoder) noexcept :
		ComPtrHolder{ decoder }
	{ }

	BitmapDecoder::BitmapDecoder(
		const ContainerFormat& containerFormat,
		const std::optional<GUID>& vendorGUID) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreateDecoder(
			containerFormat,
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr, GetAddress());
			FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              ,
			              L"Failed to create decoder");
		}
	}

	BitmapDecoder::BitmapDecoder(
		const ULONG_PTR fileHandle,
		BitmapDecoderOptions decoderOptions,
		const std::optional<GUID>& vendorGUID) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();


		if (const auto hr = factory->CreateDecoderFromFileHandle(
			fileHandle,
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress());
			FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              ,
			              L"Failed to create decoder");
		}
	}

	BitmapDecoder::BitmapDecoder(
		const std::filesystem::path& fileName, DesiredAccess desiredAccess,
		BitmapDecoderOptions decoderOptions,
		const std::optional<GUID>& vendorGUID) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreateDecoderFromFilename(
			fileName.c_str(),
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<DWORD>(desiredAccess),
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress());
			FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              ,
			              L"Failed to create decoder");
		}
	}

	BitmapDecoder::BitmapDecoder(
		const ComPtr<IStream>& stream, BitmapDecoderOptions decoderOptions,
		const std::optional<GUID>& vendorGUID) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreateDecoderFromStream(
			stream.Get(),
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress());
			FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              ,
			              L"Failed to create decoder");
		}
	}

	auto BitmapDecoder::Initialize(
		const ComPtr<IStream>& stream,
		BitmapDecoderOptions decoderOptions) const noexcept -> Error
	{
		return Error{
			Get()->Initialize(
			stream.Get(),
			static_cast<WICDecodeOptions>(decoderOptions))
		};
	}

	auto BitmapDecoder::CopyPalette() noexcept -> Result<Palette>
	{
		Palette palette{ };

		if (const auto hr = Get()->CopyPalette(palette.GetRaw());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to copy palette from decoder");
			return Unexpected{ error };
		}

		return palette;
	}

	auto BitmapDecoder::GetMetadataReader() const noexcept -> Result<MetadataReader>
	{
		ComPtr<IWICMetadataQueryReader> reader;

		if (const auto hr = Get()->GetMetadataQueryReader(reader.GetAddressOf());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get metadata query reader from decoder");
			return Unexpected{ error };
		}

		return reader;
	}

	auto BitmapDecoder::GetPreview() const noexcept -> Result<BitmapSource<>>
	{
		ComPtr<IWICBitmapSource> preview;
		if (const auto hr = Get()->GetPreview(preview.GetAddressOf());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get preview from decoder");
			return Unexpected{ error };
		}

		return preview;
	}

	auto BitmapDecoder::GetThumbnail() const noexcept -> Result<BitmapSource<>>
	{
		ComPtr<IWICBitmapSource> thumbnail;
		if (const auto hr = Get()->GetThumbnail(thumbnail.GetAddressOf());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get thumbnail from decoder");
			return Unexpected{ error };
		}

		return thumbnail;
	}

	auto BitmapDecoder::GetContainerFormat() const noexcept -> Result<ContainerFormat>
	{
		GUID format{ };
		if (const auto hr = Get()->GetContainerFormat(&format);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get container format from decoder");
			return Unexpected{ error };
		}

		return format;
	}

	auto BitmapDecoder::GetColorContexts(
		const UINT count) const noexcept -> Result<std::vector<ComPtr<IWICColorContext>>>
	{
		std::vector<IWICColorContext*> contexts(count);

		if (const auto hr = Get()->GetColorContexts(count, contexts.data(), nullptr);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get color contexts from decoder");
			return Unexpected{ error };
		}

		return contexts | std::views::transform([](auto context)
		{
			return ComPtr<IWICColorContext>{ context };
		}) | std::ranges::to<std::vector<ComPtr<IWICColorContext>>>();
	}

	auto BitmapDecoder::GetFrameCount() const noexcept -> Result<UINT>
	{
		UINT count{ };
		if (const auto hr = Get()->GetFrameCount(&count);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get frame count from decoder");
			return Unexpected{ error };
		}

		return count;
	}

	auto BitmapDecoder::GetFrame(const UINT index) const noexcept -> Result<BitmapFrameDecode>
	{
		const auto frameCountResult = GetFrameCount();
		return frameCountResult.and_then([this, index](const UINT frameCount) -> Result<BitmapFrameDecode>
		{
			if (index >= frameCount)
			{
				Error error{ E_INVALIDARG };
				error
					.AddDetail(L"Frame Index", std::to_wstring(index))
					.AddDetail(L"Frame Count", std::to_wstring(frameCount))
					.SuggestFix(L"Requested index is is bigger than or equal to frame count");
				Logger::Error(error, L"Invalid frame index requested");
				return Unexpected{ error };
			}
			ComPtr<IWICBitmapFrameDecode> frameDecode;

			if (const auto hr = Get()->GetFrame(index, frameDecode.GetAddressOf());
				FAILED(hr))
			{
				Error error{ hr };
				error
					.AddDetail(L"Frame Index", std::to_wstring(index));
				Logger::Error(error, L"Failed to get frame from decoder");
				return Unexpected{ error };
			}

			return BitmapFrameDecode{ frameDecode };
		}).or_else([](const Error& error) -> Result<BitmapFrameDecode>
		{
			Logger::Error(error, L"Failed to get frame from decoder");
			return Unexpected{ error };
		});
	}

	auto BitmapDecoder::GetAllFrames() const noexcept -> Result<std::vector<BitmapFrameDecode>>
	{
		return GetFrameCount().and_then([this](const auto& frameCount) -> Result<std::vector<BitmapFrameDecode>>
		{
			std::vector<BitmapFrameDecode> frames(frameCount);

			for (const auto i : std::views::iota(0U, frameCount))
			{
				auto frameResult = GetFrame(i);
				if (!frameResult.has_value())
				{
					Logger::Log(frameResult.error(), L"Failed to get frame from decoder");
					return Unexpected{ frameResult.error() };
				}
				frames[i] = frameResult.value();
			}

			return frames;
		});
	}

	auto BitmapDecoder::QueryCapabilities(
		const ComPtr<IStream>& stream) const noexcept -> Result<WICBitmapDecoderCapabilities>
	{
		DWORD capabilities{ };

		if (const auto hr = Get()->QueryCapability(stream.Get(), &capabilities);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to query capabilities from decoder");
			return Unexpected{ error };
		}

		return static_cast<WICBitmapDecoderCapabilities>(capabilities);
	}
}

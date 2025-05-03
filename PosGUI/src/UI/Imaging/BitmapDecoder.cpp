module;
#include <optional>
#include <filesystem>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <ranges>
#include <wrl.h>
#include <wincodec.h>

module PGUI.UI.Imaging.BitmapDecoder;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.Factories;

namespace PGUI::UI::Imaging
{
	BitmapDecoder::BitmapDecoder(ComPtr<IWICBitmapDecoder> decoder) noexcept : 
		ComPtrHolder<IWICBitmapDecoder>{ decoder }
	{
	}
	BitmapDecoder::BitmapDecoder(const ContainerFormat& containerFormat, std::optional<GUID> vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		auto hr = factory->CreateDecoder(containerFormat,
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr, GetAddress()); ThrowFailed(hr);
	}
	BitmapDecoder::BitmapDecoder(ULONG_PTR fileHandle, 
		BitmapDecoderOptions decoderOptions, std::optional<GUID> vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		auto hr = factory->CreateDecoderFromFileHandle(fileHandle,
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress()); ThrowFailed(hr);
	}
	BitmapDecoder::BitmapDecoder(std::filesystem::path fileName, DesiredAccess desiredAccess,
		BitmapDecoderOptions decoderOptions, std::optional<GUID> vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		auto hr = factory->CreateDecoderFromFilename(fileName.c_str(),
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<DWORD>(desiredAccess), 
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress()); ThrowFailed(hr);
	}
	BitmapDecoder::BitmapDecoder(ComPtr<IStream> stream, BitmapDecoderOptions decoderOptions, 
		std::optional<GUID> vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		auto hr = factory->CreateDecoderFromStream(stream.Get(),
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress()); ThrowFailed(hr);
	}

	void BitmapDecoder::Initialize(ComPtr<IStream> stream, BitmapDecoderOptions decoderOptions) const
	{
		auto hr = Get()->Initialize(stream.Get(), 
			static_cast<WICDecodeOptions>(decoderOptions)); ThrowFailed(hr);
	}

	auto BitmapDecoder::CopyPalette() -> Palette
	{
		Palette palette{ };
		auto hr = Get()->CopyPalette(palette.GetRaw()); ThrowFailed(hr);

		return palette;
	}

	auto BitmapDecoder::GetMetadataReader() const -> MetadataReader
	{
		ComPtr<IWICMetadataQueryReader> reader;
		auto hr = Get()->GetMetadataQueryReader(reader.GetAddressOf()); ThrowFailed(hr);

		return reader;
	}

	auto BitmapDecoder::GetPreview() const -> BitmapSource<>
	{
		ComPtr<IWICBitmapSource> preview;
		auto hr = Get()->GetPreview(preview.GetAddressOf()); ThrowFailed(hr);

		return preview;
	}

	auto BitmapDecoder::GetThumbnail() const -> BitmapSource<>
	{
		ComPtr<IWICBitmapSource> thumbnail;
		auto hr = Get()->GetThumbnail(thumbnail.GetAddressOf()); ThrowFailed(hr);
		
		return thumbnail;
	}

	auto BitmapDecoder::GetContainerFormat() const -> ContainerFormat
	{
		GUID format{ };
		auto hr = Get()->GetContainerFormat(&format); ThrowFailed(hr);
		
		return format;
	}

	auto BitmapDecoder::GetColorContexts(UINT count) const -> std::vector<ComPtr<IWICColorContext>>
	{
		std::vector<IWICColorContext*> contexts(count);
		auto hr = Get()->GetColorContexts(count, contexts.data(), nullptr); ThrowFailed(hr);
		
		return contexts | std::ranges::views::transform([](auto context)
		{
			return ComPtr<IWICColorContext>{ context };
		}) | std::ranges::to<std::vector<ComPtr<IWICColorContext>>>();
	}

	auto BitmapDecoder::GetFrameCount() const -> UINT
	{
		UINT count{ };
		auto hr = Get()->GetFrameCount(&count); ThrowFailed(hr);

		return count;
	}

	auto BitmapDecoder::GetFrame(UINT index) const -> BitmapFrameDecode
	{
		if (index > GetFrameCount())
		{
			throw std::out_of_range{ "index is out of range" };
		}

		ComPtr<IWICBitmapFrameDecode> frameDecode;

		auto hr = Get()->GetFrame(index, frameDecode.GetAddressOf()); ThrowFailed(hr);

		return frameDecode;
	}

	auto BitmapDecoder::GetAllFrames() const -> std::vector<BitmapFrameDecode>
	{
		const auto frameCount = GetFrameCount();

		std::vector<BitmapFrameDecode> frames(frameCount);

		for (auto i : std::views::iota(0U, frameCount))
		{
			frames[i] = GetFrame(i);
		}

		return frames;
	}

	auto BitmapDecoder::QueryCapabilities(ComPtr<IStream> stream) const -> WICBitmapDecoderCapabilities
	{
		DWORD capabilities{ };
		auto hr = Get()->QueryCapability(stream.Get(), &capabilities); ThrowFailed(hr);
		
		return static_cast<WICBitmapDecoderCapabilities>(capabilities);
	}
}

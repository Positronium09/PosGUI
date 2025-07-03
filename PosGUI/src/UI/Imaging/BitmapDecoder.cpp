module;
#include <ranges>
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.BitmapDecoder;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.Factories;

namespace PGUI::UI::Imaging
{
	BitmapDecoder::BitmapDecoder(const ComPtr<IWICBitmapDecoder>& decoder) noexcept :
		ComPtrHolder{ decoder }
	{ }

	BitmapDecoder::BitmapDecoder(
		const ContainerFormat& containerFormat,
		const std::optional<GUID>& vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		const auto hr = factory->CreateDecoder(
			containerFormat,
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr, GetAddress());
		ThrowFailed(hr);
	}

	BitmapDecoder::BitmapDecoder(
		const ULONG_PTR fileHandle,
		BitmapDecoderOptions decoderOptions,
		const std::optional<GUID>& vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		const auto hr = factory->CreateDecoderFromFileHandle(
			fileHandle,
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress());
		ThrowFailed(hr);
	}

	BitmapDecoder::BitmapDecoder(
		const std::filesystem::path& fileName, DesiredAccess desiredAccess,
		BitmapDecoderOptions decoderOptions, const std::optional<GUID>& vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		const auto hr = factory->CreateDecoderFromFilename(
			fileName.c_str(),
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<DWORD>(desiredAccess),
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress());
		ThrowFailed(hr);
	}

	BitmapDecoder::BitmapDecoder(
		const ComPtr<IStream>& stream, BitmapDecoderOptions decoderOptions,
		const std::optional<GUID>& vendorGUID)
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		const auto hr = factory->CreateDecoderFromStream(
			stream.Get(),
			vendorGUID.has_value() ? &vendorGUID.value() : nullptr,
			static_cast<WICDecodeOptions>(decoderOptions), GetAddress());
		ThrowFailed(hr);
	}

	auto BitmapDecoder::Initialize(const ComPtr<IStream>& stream, BitmapDecoderOptions decoderOptions) const -> void
	{
		const auto hr = Get()->Initialize(
			stream.Get(),
			static_cast<WICDecodeOptions>(decoderOptions));
		ThrowFailed(hr);
	}

	auto BitmapDecoder::CopyPalette() -> Palette
	{
		Palette palette{ };
		const auto hr = Get()->CopyPalette(palette.GetRaw());
		ThrowFailed(hr);

		return palette;
	}

	auto BitmapDecoder::GetMetadataReader() const -> MetadataReader
	{
		ComPtr<IWICMetadataQueryReader> reader;
		const auto hr = Get()->GetMetadataQueryReader(reader.GetAddressOf());
		ThrowFailed(hr);

		return reader;
	}

	auto BitmapDecoder::GetPreview() const -> BitmapSource<>
	{
		ComPtr<IWICBitmapSource> preview;
		const auto hr = Get()->GetPreview(preview.GetAddressOf());
		ThrowFailed(hr);

		return preview;
	}

	auto BitmapDecoder::GetThumbnail() const -> BitmapSource<>
	{
		ComPtr<IWICBitmapSource> thumbnail;
		const auto hr = Get()->GetThumbnail(thumbnail.GetAddressOf());
		ThrowFailed(hr);

		return thumbnail;
	}

	auto BitmapDecoder::GetContainerFormat() const -> ContainerFormat
	{
		GUID format{ };
		const auto hr = Get()->GetContainerFormat(&format);
		ThrowFailed(hr);

		return format;
	}

	auto BitmapDecoder::GetColorContexts(const UINT count) const -> std::vector<ComPtr<IWICColorContext>>
	{
		std::vector<IWICColorContext*> contexts(count);
		const auto hr = Get()->GetColorContexts(count, contexts.data(), nullptr);
		ThrowFailed(hr);

		return contexts | std::views::transform([](auto context)
		{
			return ComPtr<IWICColorContext>{ context };
		}) | std::ranges::to<std::vector<ComPtr<IWICColorContext>>>();
	}

	auto BitmapDecoder::GetFrameCount() const -> UINT
	{
		UINT count{ };
		const auto hr = Get()->GetFrameCount(&count);
		ThrowFailed(hr);

		return count;
	}

	auto BitmapDecoder::GetFrame(const UINT index) const -> BitmapFrameDecode
	{
		if (index > GetFrameCount())
		{
			throw std::out_of_range{ "index is out of range" };
		}

		ComPtr<IWICBitmapFrameDecode> frameDecode;

		const auto hr = Get()->GetFrame(index, frameDecode.GetAddressOf());
		ThrowFailed(hr);

		return frameDecode;
	}

	auto BitmapDecoder::GetAllFrames() const -> std::vector<BitmapFrameDecode>
	{
		const auto frameCount = GetFrameCount();

		std::vector<BitmapFrameDecode> frames(frameCount);

		for (const auto i : std::views::iota(0U, frameCount))
		{
			frames[i] = GetFrame(i);
		}

		return frames;
	}

	auto BitmapDecoder::QueryCapabilities(const ComPtr<IStream>& stream) const -> WICBitmapDecoderCapabilities
	{
		DWORD capabilities{ };
		const auto hr = Get()->QueryCapability(stream.Get(), &capabilities);
		ThrowFailed(hr);

		return static_cast<WICBitmapDecoderCapabilities>(capabilities);
	}
}

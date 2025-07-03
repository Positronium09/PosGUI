module;
#include <wincodec.h>
#include <wrl.h>

export module PGUI.UI.Imaging.BitmapDecoder;

import std;

import PGUI.ComPtr;
import PGUI.UI.Imaging.ContainerFormats;
import PGUI.UI.Imaging.Palette;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.BitmapFrameDecode;
import PGUI.UI.Imaging.MetadataReader;

export namespace PGUI::UI::Imaging
{
	enum class BitmapDecoderOptions : DWORD
	{
		DecodeMetadataCacheOnDemand = WICDecodeMetadataCacheOnDemand,
		DecodeMetadataCacheOnLoad = WICDecodeMetadataCacheOnLoad
	};

	enum class DesiredAccess : DWORD
	{
		Read = GENERIC_READ,
		Write = GENERIC_WRITE,
		ReadWrite = GENERIC_READ | GENERIC_WRITE
	};

	class BitmapDecoder : public ComPtrHolder<IWICBitmapDecoder>
	{
		public:
		BitmapDecoder() noexcept = default;

		explicit(false) BitmapDecoder(const ComPtr<IWICBitmapDecoder>& decoder) noexcept;

		explicit BitmapDecoder(
			const ContainerFormat& containerFormat,
			const std::optional<GUID>& vendorGUID = std::nullopt);

		explicit BitmapDecoder(
			ULONG_PTR fileHandle,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand,
			const std::optional<GUID>& vendorGUID = std::nullopt);

		explicit BitmapDecoder(
			const std::filesystem::path& fileName,
			DesiredAccess desiredAccess = DesiredAccess::Read,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand,
			const std::optional<GUID>& vendorGUID = std::nullopt);

		explicit BitmapDecoder(
			const ComPtr<IStream>& stream,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand,
			const std::optional<GUID>& vendorGUID = std::nullopt);

		auto Initialize(const ComPtr<IStream>& stream,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand) const
			-> void;

		[[nodiscard]] auto CopyPalette() -> Palette;

		//TODO GetDecoderInfo

		[[nodiscard]] auto GetMetadataReader() const -> MetadataReader;

		[[nodiscard]] auto GetPreview() const -> BitmapSource<>;

		[[nodiscard]] auto GetThumbnail() const -> BitmapSource<>;

		[[nodiscard]] auto GetContainerFormat() const -> ContainerFormat;

		[[nodiscard]] auto GetColorContexts(UINT count) const -> std::vector<ComPtr<IWICColorContext>>;

		[[nodiscard]] auto GetFrameCount() const -> UINT;

		[[nodiscard]] auto GetFrame(UINT index = 0U) const -> BitmapFrameDecode;

		[[nodiscard]] auto GetAllFrames() const -> std::vector<BitmapFrameDecode>;

		[[nodiscard]] auto QueryCapabilities(const ComPtr<IStream>& stream) const -> WICBitmapDecoderCapabilities;
	};
}

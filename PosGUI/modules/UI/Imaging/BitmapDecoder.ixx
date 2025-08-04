module;
#include <wincodec.h>
#include <wrl.h>

export module PGUI.UI.Imaging.BitmapDecoder;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
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
			const std::optional<GUID>& vendorGUID = std::nullopt) noexcept;

		explicit BitmapDecoder(
			ULONG_PTR fileHandle,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand,
			const std::optional<GUID>& vendorGUID = std::nullopt) noexcept;

		explicit BitmapDecoder(
			const std::filesystem::path& fileName,
			DesiredAccess desiredAccess = DesiredAccess::Read,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand,
			const std::optional<GUID>& vendorGUID = std::nullopt) noexcept;

		explicit BitmapDecoder(
			const ComPtr<IStream>& stream,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand,
			const std::optional<GUID>& vendorGUID = std::nullopt) noexcept;

		auto Initialize(const ComPtr<IStream>& stream,
			BitmapDecoderOptions decoderOptions = BitmapDecoderOptions::DecodeMetadataCacheOnDemand) const noexcept -> Error;

		//TODO GetDecoderInfo

		[[nodiscard]] auto CopyPalette() noexcept -> Result<Palette>;

		[[nodiscard]] auto GetMetadataReader() const noexcept -> Result<MetadataReader>;

		[[nodiscard]] auto GetPreview() const noexcept -> Result<BitmapSource<>>;

		[[nodiscard]] auto GetThumbnail() const noexcept -> Result<BitmapSource<>>;

		[[nodiscard]] auto GetContainerFormat() const noexcept -> Result<ContainerFormat>;

		[[nodiscard]] auto GetColorContexts(UINT count) const noexcept -> Result<std::vector<ComPtr<IWICColorContext>>>;

		[[nodiscard]] auto GetFrameCount() const noexcept -> Result<UINT>;

		[[nodiscard]] auto GetFrame(UINT index = 0U) const noexcept -> Result<BitmapFrameDecode>;

		[[nodiscard]] auto GetAllFrames() const noexcept -> Result<std::vector<BitmapFrameDecode>>;

		[[nodiscard]] auto QueryCapabilities(const ComPtr<IStream>& stream) const noexcept -> Result<WICBitmapDecoderCapabilities>;
	};
}

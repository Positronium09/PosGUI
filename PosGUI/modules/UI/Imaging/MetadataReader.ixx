module;
#include <wincodec.h>

export module PGUI.UI.Imaging.MetadataReader;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ComIterator;
import PGUI.PropVariant;
import PGUI.UI.Imaging.ContainerFormats;

export namespace PGUI::UI::Imaging
{
	class MetadataReader : public ComPtrHolder<IWICMetadataQueryReader>
	{
		public:
		MetadataReader() noexcept = default;

		explicit(false) MetadataReader(const ComPtr<IWICMetadataQueryReader>& reader) noexcept;

		[[nodiscard]] auto GetContainerFormat() const noexcept -> Result<ContainerFormat>;

		[[nodiscard]] auto GetMetadata(wzstring_view name) const noexcept -> Result<PropVariant>;

		[[nodiscard]] auto Location() const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetEnumerator() const noexcept -> Result<ComPtr<IEnumString>>;

		[[nodiscard]] auto operator[](wzstring_view name) const -> PropVariant;

		[[nodiscard]] auto cbegin() const -> EnumStringIterator;

		[[nodiscard]] auto cend() const -> EnumStringIterator;

		[[nodiscard]] auto begin() const -> EnumStringIterator;

		[[nodiscard]] auto end() const -> EnumStringIterator;
	};
}

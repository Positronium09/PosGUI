module;
#include <wincodec.h>
#include <wrl.h>

export module PGUI.UI.Imaging.MetadataReader;

import std;

import PGUI.ComPtr;
import PGUI.PropVariant;
import PGUI.IEnumStringIterator;
import PGUI.UI.Imaging.ContainerFormats;

export namespace PGUI::UI::Imaging
{
	class MetadataReader : public ComPtrHolder<IWICMetadataQueryReader>
	{
		public:
		MetadataReader() noexcept = default;

		explicit(false) MetadataReader(const ComPtr<IWICMetadataQueryReader>& reader) noexcept;

		[[nodiscard]] auto GetContainerFormat() const noexcept -> Result<ContainerFormat>;

		[[nodiscard]] auto GetMetadata(std::wstring_view name) const noexcept -> Result<PropVariant>;

		[[nodiscard]] auto Location() const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetEnumerator() const noexcept -> Result<ComPtr<IEnumString>>;

		[[nodiscard]] auto operator[](std::wstring_view name) const -> PropVariant;

		[[nodiscard]] auto cbegin() const -> IEnumStringIterator;

		[[nodiscard]] auto cend() const -> IEnumStringIterator;

		[[nodiscard]] auto begin() const -> IEnumStringIterator;

		[[nodiscard]] auto end() const -> IEnumStringIterator;
	};
}

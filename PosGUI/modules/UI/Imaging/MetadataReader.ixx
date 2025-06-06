module;
#include <string>
#include <wrl.h>
#include <wincodec.h>

export module PGUI.UI.Imaging.MetadataReader;

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
		MetadataReader(ComPtr<IWICMetadataQueryReader> reader) noexcept;

		[[nodiscard]] auto GetContainerFormat() const -> ContainerFormat;
		[[nodiscard]] auto GetMetadata(std::wstring_view name) const -> PropVariant;
		[[nodiscard]] auto Location() const -> std::wstring;

		[[nodiscard]] auto GetEnumerator() const -> ComPtr<IEnumString>;
		[[nodiscard]] auto operator[](std::wstring_view name) const noexcept -> PropVariant;

		[[nodiscard]] auto cbegin() const noexcept -> IEnumStringIterator;
		[[nodiscard]] auto cend() const noexcept -> IEnumStringIterator;
		[[nodiscard]] auto begin() const noexcept -> IEnumStringIterator;
		[[nodiscard]] auto end() const noexcept -> IEnumStringIterator;
	};
}

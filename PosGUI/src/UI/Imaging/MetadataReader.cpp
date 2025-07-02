module;
#include <wrl.h>
#include <wincodec.h>

module PGUI.UI.Imaging.MetadataReader;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.PropVariant;
import PGUI.IEnumStringIterator;
import PGUI.UI.Imaging.ContainerFormats;

namespace PGUI::UI::Imaging
{
	MetadataReader::MetadataReader(ComPtr<IWICMetadataQueryReader> reader) noexcept :
		ComPtrHolder{ reader }
	{
	}

	auto MetadataReader::GetContainerFormat() const -> ContainerFormat
	{
		ContainerFormat format;
		auto hr = Get()->GetContainerFormat(&format); ThrowFailed(hr);

		return format;
	}

	auto MetadataReader::GetMetadata(std::wstring_view name) const -> PropVariant
	{
		PropVariant value;
		auto hr = Get()->GetMetadataByName(name.data(), &value); ThrowFailed(hr);

		return value;
	}

	auto MetadataReader::Location() const -> std::wstring
	{
		UINT length = 0;
		auto hr = Get()->GetLocation(0, nullptr, &length); ThrowFailed(hr);

		std::wstring location(length, L'\0');

		hr = Get()->GetLocation(length, location.data(), &length); ThrowFailed(hr);
		return location;
	}

	auto MetadataReader::GetEnumerator() const -> ComPtr<IEnumString>
	{
		ComPtr<IEnumString> enumerator;
		auto hr = Get()->GetEnumerator(&enumerator); ThrowFailed(hr);

		return enumerator;
	}

	auto MetadataReader::operator[](std::wstring_view name) const noexcept -> PropVariant
	{
		return GetMetadata(name);
	}

	auto MetadataReader::cbegin() const noexcept -> IEnumStringIterator
	{
		return IEnumStringIterator{ GetEnumerator() };
	}
	auto MetadataReader::cend() const noexcept -> IEnumStringIterator
	{
		return IEnumStringIterator{ GetEnumerator(), true };
	}
	auto MetadataReader::begin() const noexcept -> IEnumStringIterator
	{
		return IEnumStringIterator{ GetEnumerator() };
	}
	auto MetadataReader::end() const noexcept -> IEnumStringIterator
	{
		return IEnumStringIterator{ GetEnumerator(), true };
	}
}

module;
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.MetadataReader;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.PropVariant;
import PGUI.IEnumStringIterator;
import PGUI.UI.Imaging.ContainerFormats;

namespace PGUI::UI::Imaging
{
	MetadataReader::MetadataReader(const ComPtr<IWICMetadataQueryReader>& reader) noexcept :
		ComPtrHolder{ reader }
	{ }

	auto MetadataReader::GetContainerFormat() const noexcept -> Result<ContainerFormat>
	{
		ContainerFormat format;
		if (const auto hr = Get()->GetContainerFormat(&format);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get container format");
			return Unexpected{ error };
		}

		return format;
	}

	auto MetadataReader::GetMetadata(const std::wstring_view name) const noexcept -> Result<PropVariant>
	{
		PropVariant value;
		if (const auto hr = Get()->GetMetadataByName(name.data(), &value);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Name", name)
				.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get metadata for name: {}");
			return Unexpected{ error };
		}

		return value;
	}

	auto MetadataReader::Location() const noexcept -> Result<std::wstring>
	{
		UINT length = 0;
		if (const auto hr = Get()->GetLocation(0, nullptr, &length);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get location length");
			return Unexpected{ error };
		}

		std::wstring location(length, L'\0');

		if (const auto hr = Get()->GetLocation(length, location.data(), &length);
			FAILED(hr))
		{ }

		return location;
	}

	auto MetadataReader::GetEnumerator() const noexcept -> Result<ComPtr<IEnumString>>
	{
		ComPtr<IEnumString> enumerator;
		if (const auto hr = Get()->GetEnumerator(&enumerator);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get metadata enumerator");
			return Unexpected{ error };
		}

		return enumerator;
	}

	auto MetadataReader::operator[](const std::wstring_view name) const -> PropVariant
	{
		auto metadataResult = GetMetadata(name);
		if (metadataResult.has_value())
		{
			return metadataResult.value();
		}

		throw Exception{
			metadataResult.error()
		}.SuggestFix(L"operator[] throws an exception; use GetMetadata if you want noexcept");
	}

	auto MetadataReader::cbegin() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value() };
		}

		throw Exception{ enumeratorResult.error() };
	}

	auto MetadataReader::cend() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value(), true };
		}

		throw Exception{ enumeratorResult.error() };
	}

	auto MetadataReader::begin() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value() };
		}

		throw Exception{ enumeratorResult.error() };
	}

	auto MetadataReader::end() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value(), true };
		}

		throw Exception{ enumeratorResult.error() };
	}
}

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
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return format;
	}

	auto MetadataReader::GetMetadata(const std::wstring_view name) const noexcept -> Result<PropVariant>
	{
		PropVariant value;
		if (const auto hr = Get()->GetMetadataByName(name.data(), &value);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return value;
	}

	auto MetadataReader::Location() const noexcept -> Result<std::wstring>
	{
		UINT length = 0;
		if (const auto hr = Get()->GetLocation(0, nullptr, &length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		std::wstring location(length, L'\0');

		if (const auto hr = Get()->GetLocation(length, location.data(), &length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return location;
	}

	auto MetadataReader::GetEnumerator() const noexcept -> Result<ComPtr<IEnumString>>
	{
		ComPtr<IEnumString> enumerator;
		if (const auto hr = Get()->GetEnumerator(&enumerator);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Imaging)
			};
		}

		return enumerator;
	}

	auto MetadataReader::operator[](const std::wstring_view name) const -> PropVariant
	{
		if (const auto metadataResult = GetMetadata(name);
			metadataResult.has_value())
		{
			return metadataResult.value();
		}

		throw Exception{
			Error{ WINCODEC_ERR_INVALIDPARAMETER }
			.AddTag(ErrorTags::Imaging)
		};
	}

	auto MetadataReader::cbegin() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value() };
		}

		throw Exception{
			enumeratorResult.error()
		};
	}

	auto MetadataReader::cend() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value(), true };
		}
		throw Exception{
			enumeratorResult.error()
		};
	}

	auto MetadataReader::begin() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value() };
		}
		throw Exception{
			enumeratorResult.error()
		};
	}

	auto MetadataReader::end() const -> IEnumStringIterator
	{
		const auto enumeratorResult = GetEnumerator();
		if (enumeratorResult.has_value())
		{
			return IEnumStringIterator{ enumeratorResult.value(), true };
		}
		throw Exception{
			enumeratorResult.error()
		};
	}
}

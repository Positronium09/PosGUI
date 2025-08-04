module;
#include <dwrite_1.h>
#include <wrl.h>

module PGUI.UI.Font.LocalizedStrings;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::UI::Font
{
	LocalizedStrings::LocalizedStrings(const ComPtr<IDWriteLocalizedStrings>& strings) noexcept :
		ComPtrHolder{ strings }
	{ }

	auto LocalizedStrings::FindLocaleName(
		const std::wstring_view name) const noexcept -> Result<UINT32>
	{
		UINT32 index = 0;
		auto found = FALSE;

		auto& ptr = Get();

		if (const auto hr = ptr->FindLocaleName(
			name.data(), &index,
			&found);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return index;
	}

	auto LocalizedStrings::GetLocaleName(const UINT32 index) const noexcept -> Result<std::wstring>
	{
		auto& ptr = Get();

		UINT32 length = 0;
		if (const auto hr = ptr->GetLocaleNameLength(index, &length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		std::wstring name(length, L'\0');

		if (const auto hr = ptr->GetLocaleName(index, name.data(), length + 1);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return name;
	}

	auto LocalizedStrings::GetString(const UINT32 index) const noexcept -> Result<std::wstring>
	{
		auto& ptr = Get();

		UINT32 length = 0;
		if (const auto hr = ptr->GetStringLength(index, &length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		std::wstring string(length, L'\0');

		if (const auto hr = ptr->GetString(index, string.data(), length + 1);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return string;
	}
}

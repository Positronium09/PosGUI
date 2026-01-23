module;
#include <dwrite_1.h>

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
			Error error{ hr };
			error
				.AddDetail(L"Locale Name", name);
			Logger::Error(error, L"Failed to find locale name in localized strings.");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get locale name length.");
			return Unexpected{ error };
		}

		std::wstring name(length, L'\0');

		if (const auto hr = ptr->GetLocaleName(index, name.data(), length + 1);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get locale name.");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get string length.");
			return Unexpected{ error };
		}

		std::wstring string(length, L'\0');

		if (const auto hr = ptr->GetString(index, string.data(), length + 1);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get string.");
			return Unexpected{ error };
		}

		return string;
	}
}

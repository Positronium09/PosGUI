module;
#include <dwrite_1.h>
#include <wrl.h>

module PGUI.UI.Font.LocalizedStrings;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.Logging;

namespace PGUI::UI::Font
{
	LocalizedStrings::LocalizedStrings(const ComPtr<IDWriteLocalizedStrings>& strings) noexcept :
		ComPtrHolder{ strings }
	{ }

	auto LocalizedStrings::FindLocaleName(const std::wstring_view name) const noexcept -> std::expected<UINT32, bool>
	{
		UINT32 index = 0;
		auto found = FALSE;

		auto& ptr = Get();

		const auto hr = ptr->FindLocaleName(
			name.data(), &index,
			&found);
		LogFailed(LogLevel::Error, hr);

		if (!found)
		{
			return std::unexpected{ false };
		}

		return index;
	}

	auto LocalizedStrings::GetLocaleName(const UINT32 index) const -> std::wstring
	{
		auto& ptr = Get();

		UINT32 length = 0;
		auto hr = ptr->GetLocaleNameLength(index, &length);
		ThrowFailed(hr);

		std::wstring name(length, L'\0');

		hr = ptr->GetLocaleName(index, name.data(), length + 1);
		ThrowFailed(hr);

		return name;
	}

	auto LocalizedStrings::GetString(const UINT32 index) const -> std::wstring
	{
		auto& ptr = Get();

		UINT32 length = 0;
		auto hr = ptr->GetStringLength(index, &length);
		ThrowFailed(hr);

		std::wstring string(length, L'\0');

		hr = ptr->GetString(index, string.data(), length + 1);
		ThrowFailed(hr);

		return string;
	}
}

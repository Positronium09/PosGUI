module;
#include <dwrite_3.h>

module PGUI.UI.Font.FontFamily;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.Font.LocalizedStrings;
import PGUI.UI.Font.FontSet;

namespace PGUI::UI::Font
{
	FontFamily::FontFamily(const ComPtr<IDWriteFontFamily2>& family) noexcept :
		ComPtrHolder{ family }
	{ }

	auto FontFamily::GetFamilyNames() const noexcept -> Result<LocalizedStrings>
	{
		ComPtr<IDWriteLocalizedStrings> strings;

		if (const auto hr = Get()->GetFamilyNames(strings.put());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"GetFamilyNames failed");
			return Unexpected{ error };
		}

		return strings;
	}

	auto FontFamily::GetFontSet() const noexcept -> Result<FontSet>
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontSet1> fontSet1;

		if (const auto hr = ptr->GetFontSet(fontSet1.put());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"GetFontSet failed");
			return Unexpected{ error };
		}

		auto fontSet = fontSet1.try_query<IDWriteFontSet4>();
		if (fontSet.get() == nullptr)
		{
			Error error{ E_NOINTERFACE };
			Logger::Error(error, L"Cannot get IDWriteFontSet4 interface");
			return Unexpected{ error };
		}

		return fontSet;
	}
}

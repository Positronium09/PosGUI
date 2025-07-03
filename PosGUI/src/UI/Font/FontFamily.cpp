module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.Font.FontFamily;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.Logging;
import PGUI.UI.Font.LocalizedStrings;
import PGUI.UI.Font.FontSet;

namespace PGUI::UI::Font
{
	FontFamily::FontFamily(const ComPtr<IDWriteFontFamily2>& family) noexcept :
		ComPtrHolder{ family }
	{ }

	auto FontFamily::GetFamilyNames() const -> LocalizedStrings
	{
		ComPtr<IDWriteLocalizedStrings> strings;

		const auto hr = Get()->GetFamilyNames(strings.GetAddressOf());
		ThrowFailed(hr);

		return strings;
	}

	auto FontFamily::GetFontSet() const -> FontSet
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontSet1> fontSet1;
		ComPtr<IDWriteFontSet4> fontSet;
		auto hr = ptr->GetFontSet(fontSet1.GetAddressOf());
		ThrowFailed(hr);

		hr = fontSet1.As(&fontSet);
		ThrowFailed(hr);

		return fontSet;
	}
}

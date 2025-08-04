module;
#include <dwrite_3.h>
#include <wrl.h>

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

		if (const auto hr = Get()->GetFamilyNames(strings.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return strings;
	}

	auto FontFamily::GetFontSet() const noexcept -> Result<FontSet>
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontSet1> fontSet1;
		ComPtr<IDWriteFontSet4> fontSet;

		if (const auto hr = ptr->GetFontSet(fontSet1.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		if (const auto& hr = fontSet1.As(&fontSet);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return fontSet;
	}
}

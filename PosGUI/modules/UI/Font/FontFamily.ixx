module;
#include <Windows.h>
#include <dwrite_3.h>
#include <wrl.h>

export module PGUI.UI.Font.FontFamily;

import PGUI.ComPtr;
import PGUI.UI.Font.LocalizedStrings;
import PGUI.UI.Font.FontSet;

export namespace PGUI::UI::Font
{
	class FontFamily : public ComPtrHolder<IDWriteFontFamily2>
	{
		public:
		FontFamily(ComPtr<IDWriteFontFamily2> family) noexcept;

		[[nodiscard]] auto GetFamilyNames() const -> LocalizedStrings;
		[[nodiscard]] auto GetFontCount() const noexcept { return Get()->GetFontCount(); }
		[[nodiscard]] auto GetFontSet() const -> FontSet;
	};
}

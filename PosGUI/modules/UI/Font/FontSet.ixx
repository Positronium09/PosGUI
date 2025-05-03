module;
#include <Windows.h>
#include <dwrite_3.h>
#include <vector>
#include <span>
#include <optional>
#include <wrl.h>

export module PGUI.UI.Font.FontSet;

import PGUI.ComPtr;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;

export namespace PGUI::UI::Font
{
	class FontSet : public ComPtrHolder<IDWriteFontSet4>
	{
		public:
		FontSet(ComPtr<IDWriteFontSet4> set) noexcept;
		[[nodiscard]] auto ConvertWeightStretchStyleToFontAxisValues(
			FontWeight weight, FontStretch stretch, FontStyle style,
			float fontSize,
			std::optional<std::span<const FontAxisValue>> inputValues = std::nullopt) const noexcept -> std::vector<FontAxisValue>;
	};
}

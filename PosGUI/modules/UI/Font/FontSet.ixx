module;
#include <dwrite_3.h>

export module PGUI.UI.Font.FontSet;

import std;

import PGUI.ComPtr;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;

export namespace PGUI::UI::Font
{
	class FontSet : public ComPtrHolder<IDWriteFontSet4>
	{
		public:
		explicit(false) FontSet(const ComPtr<IDWriteFontSet4>& set) noexcept;

		[[nodiscard]] auto ConvertWeightStretchStyleToFontAxisValues(
			FontWeight weight, FontStretch stretch, FontStyle style,
			float fontSize,
			const std::optional<std::span<const FontAxisValue>>& inputValues = std::nullopt) const noexcept -> std::vector<
			FontAxisValue>;
	};
}

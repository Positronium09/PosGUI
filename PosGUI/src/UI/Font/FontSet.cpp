module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.Font.FontSet;

import std;

namespace PGUI::UI::Font
{
	FontSet::FontSet(ComPtr<IDWriteFontSet4> set) noexcept : 
		ComPtrHolder{ set }
	{
	}
	auto FontSet::ConvertWeightStretchStyleToFontAxisValues(FontWeight weight, FontStretch stretch, FontStyle style, 
		float fontSize,
		std::optional<std::span<const FontAxisValue>> inputValues) const noexcept -> std::vector<FontAxisValue>
	{
		auto& ptr = Get();

		const DWRITE_FONT_AXIS_VALUE* inputValuesArr = nullptr;
		UINT32 inputValuesArrCount = 0;

		if (inputValues)
		{
			inputValuesArr = std::bit_cast<const DWRITE_FONT_AXIS_VALUE*>(inputValues->data());
			inputValuesArrCount = static_cast<UINT32>(inputValues->size());
		}
		std::vector<FontAxisValue> values(DWRITE_STANDARD_FONT_AXIS_COUNT + inputValuesArrCount);

		ptr->ConvertWeightStretchStyleToFontAxisValues(inputValuesArr, inputValuesArrCount, 
			weight, stretch, style, fontSize,
			values.data());

		for (auto i : std::views::iota(0UL, inputValuesArrCount))
		{
			values[DWRITE_STANDARD_FONT_AXIS_COUNT + i] = (*inputValues)[i];
		}

		return values;
	}
}

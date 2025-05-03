module;
#include <dwrite_3.h>
#include <string_view>
#include <wrl.h>

export module PGUI.UI.Font.FontStructs;

import PGUI.UI.Font.FontEnums;
import PGUI.ComPtr;


export namespace PGUI::UI::Font
{
	struct FontAxisValue : public DWRITE_FONT_AXIS_VALUE
	{
		constexpr FontAxisValue() noexcept = default;
		constexpr FontAxisValue(DWRITE_FONT_AXIS_VALUE value) noexcept :
			DWRITE_FONT_AXIS_VALUE{ value }
		{
		}
		constexpr FontAxisValue(FontAxisTag _axisTag, float _value) noexcept :
			DWRITE_FONT_AXIS_VALUE{ .axisTag = _axisTag, .value = _value }
		{
		}

		explicit(false) constexpr operator DWRITE_FONT_AXIS_VALUE() const noexcept { return *this; }
	};

	struct FontAxisRange : public DWRITE_FONT_AXIS_RANGE
	{
		constexpr FontAxisRange() noexcept = default;
		constexpr FontAxisRange(DWRITE_FONT_AXIS_RANGE range) noexcept :
			DWRITE_FONT_AXIS_RANGE{ range }
		{
		}
		constexpr FontAxisRange(FontAxisTag _axisTag, float _minValue, float _maxValue) noexcept :
			DWRITE_FONT_AXIS_RANGE{ .axisTag = _axisTag, .minValue = _minValue, .maxValue = _maxValue }
		{
		}

		explicit(false) constexpr operator DWRITE_FONT_AXIS_RANGE() const noexcept { return *this; }
	};

	struct TextRange : public DWRITE_TEXT_RANGE
	{
		constexpr TextRange() noexcept = default;
		explicit(false) constexpr TextRange(DWRITE_TEXT_RANGE range) noexcept :
			DWRITE_TEXT_RANGE{ range }
		{
		}
		constexpr TextRange(UINT32 _startPosition, UINT32 _length) noexcept :
			DWRITE_TEXT_RANGE{ .startPosition = _startPosition, .length = _length }
		{
		}

		explicit(false) constexpr operator DWRITE_TEXT_RANGE() const noexcept { return *this; }

		[[nodiscard]] auto GetEndPosition() const noexcept -> UINT32 { return startPosition + length; }
		[[nodiscard]] auto GetLength() const noexcept -> UINT32 { return length; }
		[[nodiscard]] auto GetStartPosition() const noexcept -> UINT32 { return startPosition; }
	};

	struct LineSpacing : public DWRITE_LINE_SPACING
	{
		constexpr LineSpacing() noexcept = default;
		constexpr LineSpacing(DWRITE_LINE_SPACING spacing) noexcept :
			DWRITE_LINE_SPACING{ spacing }
		{
		}
		constexpr LineSpacing(LineSpacingMethod method, float height, float baseline) noexcept :
			DWRITE_LINE_SPACING{ .method = method, .height = height, .baseline = baseline }
		{
		}

		explicit(false) constexpr operator DWRITE_LINE_SPACING() const noexcept { return *this; }
	};

	struct Trimming : public ComPtrHolder<IDWriteInlineObject>
	{
		constexpr Trimming() noexcept = default;

		Trimming(ComPtr<IDWriteInlineObject> trimmingSign) noexcept :
			ComPtrHolder<IDWriteInlineObject>{ trimmingSign },
			trimmingOptions{ .granularity = DWRITE_TRIMMING_GRANULARITY_NONE, .delimiter = 0, .delimiterCount = 0 }
		{
		}

		Trimming(DWRITE_TRIMMING trimmingOptions) noexcept :
			ComPtrHolder<IDWriteInlineObject>{ },
			trimmingOptions{ trimmingOptions }
		{
		}

		Trimming(ComPtr<IDWriteInlineObject> trimmingSign, DWRITE_TRIMMING trimmingOptions) noexcept : 
			ComPtrHolder<IDWriteInlineObject>{ trimmingSign },
			trimmingOptions{ trimmingOptions }
		{
		}

		DWRITE_TRIMMING trimmingOptions;
	};
}

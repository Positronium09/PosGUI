module;
#include <dwrite_3.h>
#include <wrl.h>

export module PGUI.UI.Font.FontStructs;

import std;

import PGUI.UI.Font.FontEnums;
import PGUI.ComPtr;

export namespace PGUI::UI::Font
{
	struct FontAxisValue : public DWRITE_FONT_AXIS_VALUE
	{
		constexpr FontAxisValue() noexcept = default;

		explicit(false) constexpr FontAxisValue(const DWRITE_FONT_AXIS_VALUE value) noexcept :
			DWRITE_FONT_AXIS_VALUE{ value }
		{ }

		constexpr FontAxisValue(const FontAxisTag _axisTag, const float _value) noexcept :
			DWRITE_FONT_AXIS_VALUE{ .axisTag = _axisTag, .value = _value }
		{ }

		explicit(false) constexpr operator DWRITE_FONT_AXIS_VALUE() const noexcept { return *this; }
	};

	struct FontAxisRange : public DWRITE_FONT_AXIS_RANGE
	{
		constexpr FontAxisRange() noexcept = default;

		explicit(false) constexpr FontAxisRange(const DWRITE_FONT_AXIS_RANGE range) noexcept :
			DWRITE_FONT_AXIS_RANGE{ range }
		{ }

		constexpr FontAxisRange(const FontAxisTag _axisTag, const float _minValue, const float _maxValue) noexcept :
			DWRITE_FONT_AXIS_RANGE{ .axisTag = _axisTag, .minValue = _minValue, .maxValue = _maxValue }
		{ }

		explicit(false) constexpr operator DWRITE_FONT_AXIS_RANGE() const noexcept { return *this; }
	};

	struct TextRange : public DWRITE_TEXT_RANGE
	{
		constexpr TextRange() noexcept = default;

		explicit(false) constexpr TextRange(const DWRITE_TEXT_RANGE range) noexcept :
			DWRITE_TEXT_RANGE{ range }
		{ }

		constexpr TextRange(const UINT32 _startPosition, const UINT32 _length) noexcept :
			DWRITE_TEXT_RANGE{ .startPosition = _startPosition, .length = _length }
		{ }

		explicit(false) constexpr operator DWRITE_TEXT_RANGE() const noexcept { return *this; }

		[[nodiscard]] auto GetEndPosition() const noexcept -> UINT32 { return startPosition + length; }
		[[nodiscard]] auto GetLength() const noexcept -> UINT32 { return length; }
		[[nodiscard]] auto GetStartPosition() const noexcept -> UINT32 { return startPosition; }
	};

	struct LineSpacing : public DWRITE_LINE_SPACING
	{
		constexpr LineSpacing() noexcept = default;

		explicit(false) constexpr LineSpacing(const DWRITE_LINE_SPACING& spacing) noexcept :
			DWRITE_LINE_SPACING{ spacing }
		{ }

		constexpr LineSpacing(const LineSpacingMethod method, const float height, const float baseline) noexcept :
			DWRITE_LINE_SPACING{ .method = method, .height = height, .baseline = baseline }
		{ }

		explicit(false) constexpr operator DWRITE_LINE_SPACING() const noexcept { return *this; }
	};

	struct Trimming : public ComPtrHolder<IDWriteInlineObject>
	{
		constexpr Trimming() noexcept = default;

		explicit(false) Trimming(const ComPtr<IDWriteInlineObject>& trimmingSign) noexcept :
			ComPtrHolder{ trimmingSign },
			trimmingOptions{ .granularity = DWRITE_TRIMMING_GRANULARITY_NONE, .delimiter = 0, .delimiterCount = 0 }
		{ }

		explicit(false) Trimming(const DWRITE_TRIMMING trimmingOptions) noexcept :
			ComPtrHolder{ },
			trimmingOptions{ trimmingOptions }
		{ }

		Trimming(const ComPtr<IDWriteInlineObject>& trimmingSign, const DWRITE_TRIMMING trimmingOptions) noexcept :
			ComPtrHolder{ trimmingSign },
			trimmingOptions{ trimmingOptions }
		{ }

		DWRITE_TRIMMING trimmingOptions;
	};
}

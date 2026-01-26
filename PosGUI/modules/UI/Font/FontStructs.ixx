module;
#include <dwrite_3.h>

export module PGUI.UI.Font.FontStructs;

import std;

import PGUI.UI.Font.FontEnums;
import PGUI.ComPtr;

export namespace PGUI::UI::Font
{
	struct FontAxisValue : DWRITE_FONT_AXIS_VALUE
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

	struct FontAxisRange : DWRITE_FONT_AXIS_RANGE
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

	struct TextRange : DWRITE_TEXT_RANGE
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

		[[nodiscard]] auto Contains(const UINT32 position) const noexcept -> bool
		{
			return position >= startPosition && position < startPosition + length;
		}
	};

	struct LineSpacing : DWRITE_LINE_SPACING
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

	struct Trimming : ComPtrHolder<IDWriteInlineObject>
	{
		constexpr Trimming() noexcept = default;

		explicit(false) Trimming(const ComPtr<IDWriteInlineObject>& trimmingSign) noexcept :
			ComPtrHolder{ trimmingSign },
			trimmingOptions{ .granularity = DWRITE_TRIMMING_GRANULARITY_NONE, .delimiter = 0, .delimiterCount = 0 }
		{ }

		explicit(false) Trimming(const DWRITE_TRIMMING trimmingOptions) noexcept :
			trimmingOptions{ trimmingOptions }
		{ }

		Trimming(const ComPtr<IDWriteInlineObject>& trimmingSign, const DWRITE_TRIMMING trimmingOptions) noexcept :
			ComPtrHolder{ trimmingSign },
			trimmingOptions{ trimmingOptions }
		{ }

		DWRITE_TRIMMING trimmingOptions;
	};

	struct TextMetrics : DWRITE_TEXT_METRICS
	{
		constexpr TextMetrics() noexcept = default;
		
		explicit(false) constexpr TextMetrics(const DWRITE_TEXT_METRICS& metrics) noexcept :
			DWRITE_TEXT_METRICS{ metrics }
		{ }

		constexpr TextMetrics(
			const FLOAT left, const FLOAT top, 
			const FLOAT width,
			const FLOAT widthIncludingTrailingWhitespace,
			const FLOAT height,
			const FLOAT layoutWidth,
			const FLOAT layoutHeight,
			const UINT32 maxBidiReorderingDepth,
			const UINT32 lineCount
		) noexcept : 
			DWRITE_TEXT_METRICS{
				.left = left,
				.top = top,
				.width = width,
				.widthIncludingTrailingWhitespace = widthIncludingTrailingWhitespace,
				.height = height,
				.layoutWidth = layoutWidth,
				.layoutHeight = layoutHeight,
				.maxBidiReorderingDepth = maxBidiReorderingDepth,
				.lineCount = lineCount
		}
		{ }

		explicit(false) constexpr operator DWRITE_TEXT_METRICS() const noexcept { return *this; }
	};

	struct alignas(alignof(DWRITE_LINE_METRICS1)) LineMetrics
	{
		UINT32 length;
		UINT32 trailingWhitespaceLength;
		UINT32 newLineLength;
		FLOAT height;
		FLOAT baseline;
		BOOL isTrimmed;
		FLOAT leadingBefore;
		FLOAT leadingAfter;

		constexpr LineMetrics() noexcept = default;
		
		constexpr LineMetrics(
			const UINT32 length,
			const UINT32 trailingWhitespaceLength,
			const UINT32 newLineLength,
			const FLOAT height,
			const FLOAT baseline,
			const BOOL isTrimmed,
			const FLOAT leadingBefore,
			const FLOAT leadingAfter
		) noexcept : 
			length{ length },
			trailingWhitespaceLength{ trailingWhitespaceLength },
			newLineLength{ newLineLength },
			height{ height },
			baseline{ baseline },
			isTrimmed{ isTrimmed },
			leadingBefore{ leadingBefore },
			leadingAfter{ leadingAfter }
		{ }

		explicit(false) constexpr LineMetrics(const DWRITE_LINE_METRICS& metrics) noexcept :
			length{ metrics.length },
			trailingWhitespaceLength{ metrics.trailingWhitespaceLength },
			newLineLength{ metrics.newlineLength },
			height{ metrics.height },
			baseline{ metrics.baseline },
			isTrimmed{ metrics.isTrimmed },
			leadingBefore{ 0.0F },
			leadingAfter{ 0.0F }
		{
		}

		explicit(false) constexpr LineMetrics(const DWRITE_LINE_METRICS1& metrics) noexcept :
			length{ metrics.length },
			trailingWhitespaceLength{ metrics.trailingWhitespaceLength },
			newLineLength{ metrics.newlineLength },
			height{ metrics.height },
			baseline{ metrics.baseline },
			isTrimmed{ metrics.isTrimmed },
			leadingBefore{ metrics.leadingBefore },
			leadingAfter{ metrics.leadingAfter }
		{
		}

		explicit(false) constexpr operator DWRITE_LINE_METRICS() const noexcept
		{
			return DWRITE_LINE_METRICS{
				.length = length,
				.trailingWhitespaceLength = trailingWhitespaceLength,
				.newlineLength = newLineLength,
				.height = height,
				.baseline = baseline,
				.isTrimmed = isTrimmed
			};
		}

		explicit(false) constexpr operator DWRITE_LINE_METRICS1() const noexcept
		{
			DWRITE_LINE_METRICS1 metrics{
				.leadingBefore = leadingBefore,
				.leadingAfter = leadingAfter
			};
			metrics.length = length;
			metrics.trailingWhitespaceLength = trailingWhitespaceLength;
			metrics.newlineLength = newLineLength;
			metrics.height = height;
			metrics.baseline = baseline;
			metrics.isTrimmed = isTrimmed;

			return metrics;
		}
	};
}

export template <typename Char>
struct std::formatter<PGUI::UI::Font::TextRange, Char>
{
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto iter = ctx.begin();
		const auto end = ctx.end();
		if (iter == end || *iter == '}')
		{
			return iter;
		}

		throw std::format_error{ "No format specifiers is supported" };
	}

	template <typename FormatContext>
	constexpr auto format(const PGUI::UI::Font::TextRange& textRange, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(),
			"Start {}, End {}, Length {}",
			textRange.GetStartPosition(),
			textRange.GetEndPosition(),
			textRange.GetLength());
	}
};

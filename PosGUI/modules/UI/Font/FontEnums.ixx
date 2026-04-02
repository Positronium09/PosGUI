module;
#include <dwrite.h>
#include <dwrite_3.h>

export module PGUI.UI.Font.FontEnums;

import std;

export namespace PGUI::UI::Font
{
	struct FontWeight
	{
		constexpr FontWeight() noexcept :
			fontWeight{ DWRITE_FONT_WEIGHT_NORMAL }
		{ }

		explicit(false) constexpr FontWeight(const DWRITE_FONT_WEIGHT fontWeight) noexcept :
			fontWeight{ fontWeight }
		{ }

		explicit(false) constexpr operator DWRITE_FONT_WEIGHT() const noexcept { return fontWeight; }

		constexpr auto operator==(const FontWeight&) const noexcept -> bool = default;
		constexpr auto operator<=>(const FontWeight& other) const noexcept
		{
			return std::to_underlying(fontWeight) <=> std::to_underlying(other.fontWeight);
		}

		private:
		DWRITE_FONT_WEIGHT fontWeight;
	};

	namespace FontWeights
	{
		constexpr FontWeight Thin = DWRITE_FONT_WEIGHT_THIN;
		constexpr FontWeight ExtraLight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
		constexpr FontWeight UltraLight = DWRITE_FONT_WEIGHT_ULTRA_LIGHT;
		constexpr FontWeight Light = DWRITE_FONT_WEIGHT_LIGHT;
		constexpr FontWeight SemiLight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
		constexpr FontWeight Normal = DWRITE_FONT_WEIGHT_NORMAL;
		constexpr FontWeight Regular = DWRITE_FONT_WEIGHT_REGULAR;
		constexpr FontWeight Medium = DWRITE_FONT_WEIGHT_MEDIUM;
		constexpr FontWeight DemiBold = DWRITE_FONT_WEIGHT_DEMI_BOLD;
		constexpr FontWeight SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD;
		constexpr FontWeight Bold = DWRITE_FONT_WEIGHT_BOLD;
		constexpr FontWeight ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD;
		constexpr FontWeight UltraBold = DWRITE_FONT_WEIGHT_ULTRA_BOLD;
		constexpr FontWeight Black = DWRITE_FONT_WEIGHT_BLACK;
		constexpr FontWeight Heavy = DWRITE_FONT_WEIGHT_HEAVY;
		constexpr FontWeight ExtraBlack = DWRITE_FONT_WEIGHT_EXTRA_BLACK;
		constexpr FontWeight UltraBlack = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	}

	struct FontStyle
	{
		constexpr FontStyle() noexcept :
			fontStyle{ DWRITE_FONT_STYLE_NORMAL }
		{ }

		explicit(false) constexpr FontStyle(const DWRITE_FONT_STYLE fontStyle) noexcept :
			fontStyle{ fontStyle }
		{ }

		explicit(false) constexpr operator DWRITE_FONT_STYLE() const noexcept { return fontStyle; }

		constexpr auto operator==(const FontStyle&) const noexcept -> bool = default;

		private:
		DWRITE_FONT_STYLE fontStyle;
	};

	namespace FontStyles
	{
		constexpr FontStyle Normal = DWRITE_FONT_STYLE_NORMAL;
		constexpr FontStyle Oblique = DWRITE_FONT_STYLE_OBLIQUE;
		constexpr FontStyle Italic = DWRITE_FONT_STYLE_ITALIC;
	}

	struct FontStretch
	{
		constexpr FontStretch() noexcept :
			fontStretch{ DWRITE_FONT_STRETCH_NORMAL }
		{ }

		explicit(false) constexpr FontStretch(const DWRITE_FONT_STRETCH fontStretch) noexcept :
			fontStretch{ fontStretch }
		{ }

		explicit(false) constexpr operator DWRITE_FONT_STRETCH() const noexcept { return fontStretch; }

		constexpr auto operator==(const FontStretch&) const noexcept -> bool = default;
		constexpr auto operator<=>(const FontStretch& other) const noexcept
		{
			return std::to_underlying(fontStretch) <=> std::to_underlying(other.fontStretch);
		}

		private:
		DWRITE_FONT_STRETCH fontStretch;
	};

	namespace FontStretches
	{
		constexpr FontStretch Undefined = DWRITE_FONT_STRETCH_UNDEFINED;
		constexpr FontStretch UltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED;
		constexpr FontStretch ExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED;
		constexpr FontStretch Condensed = DWRITE_FONT_STRETCH_CONDENSED;
		constexpr FontStretch SemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED;
		constexpr FontStretch Normal = DWRITE_FONT_STRETCH_NORMAL;
		constexpr FontStretch Medium = DWRITE_FONT_STRETCH_MEDIUM;
		constexpr FontStretch SemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED;
		constexpr FontStretch Expanded = DWRITE_FONT_STRETCH_EXPANDED;
		constexpr FontStretch ExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED;
		constexpr FontStretch UltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED;
	}

	struct FlowDirection
	{
		constexpr FlowDirection() noexcept :
			flowDirection{ DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT }
		{ }

		explicit(false) constexpr FlowDirection(const DWRITE_FLOW_DIRECTION flowDirection) noexcept :
			flowDirection{ flowDirection }
		{ }

		explicit(false) constexpr operator DWRITE_FLOW_DIRECTION() const noexcept { return flowDirection; }

		constexpr auto operator==(const FlowDirection&) const noexcept -> bool = default;

		private:
		DWRITE_FLOW_DIRECTION flowDirection;
	};

	namespace FlowDirections
	{
		constexpr FlowDirection LeftToRight = DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT;
		constexpr FlowDirection RightToLeft = DWRITE_FLOW_DIRECTION_RIGHT_TO_LEFT;
		constexpr FlowDirection TopToBottom = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM;
		constexpr FlowDirection BottomToTop = DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP;
	}

	struct ParagraphAlignment
	{
		constexpr ParagraphAlignment() noexcept :
			paragraphAlignment{ DWRITE_PARAGRAPH_ALIGNMENT_NEAR }
		{ }

		explicit(false) constexpr ParagraphAlignment(const DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment) noexcept :
			paragraphAlignment{ paragraphAlignment }
		{ }

		explicit(false) constexpr operator DWRITE_PARAGRAPH_ALIGNMENT() const noexcept { return paragraphAlignment; }

		constexpr auto operator==(const ParagraphAlignment&) const noexcept -> bool = default;

		private:
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;
	};

	namespace ParagraphAlignments
	{
		constexpr ParagraphAlignment Near = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		constexpr ParagraphAlignment Center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		constexpr ParagraphAlignment Far = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	}

	struct ReadingDirection
	{
		constexpr ReadingDirection() noexcept :
			readingDirection{ DWRITE_READING_DIRECTION_LEFT_TO_RIGHT }
		{ }

		explicit(false) constexpr ReadingDirection(const DWRITE_READING_DIRECTION readingDirection) noexcept :
			readingDirection{ readingDirection }
		{ }

		explicit(false) constexpr operator DWRITE_READING_DIRECTION() const noexcept { return readingDirection; }

		constexpr auto operator==(const ReadingDirection&) const noexcept -> bool = default;

		private:
		DWRITE_READING_DIRECTION readingDirection;
	};

	namespace ReadingDirections
	{
		constexpr ReadingDirection LeftToRight = DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;
		constexpr ReadingDirection RightToLeft = DWRITE_READING_DIRECTION_RIGHT_TO_LEFT;
		constexpr ReadingDirection TopToBottom = DWRITE_READING_DIRECTION_TOP_TO_BOTTOM;
		constexpr ReadingDirection BottomToTop = DWRITE_READING_DIRECTION_BOTTOM_TO_TOP;
	}

	struct TextAlignment
	{
		constexpr TextAlignment() noexcept :
			textAlignment{ DWRITE_TEXT_ALIGNMENT_LEADING }
		{ }

		explicit(false) constexpr TextAlignment(const DWRITE_TEXT_ALIGNMENT textAlignment) noexcept :
			textAlignment{ textAlignment }
		{ }

		explicit(false) constexpr operator DWRITE_TEXT_ALIGNMENT() const noexcept { return textAlignment; }

		constexpr auto operator==(const TextAlignment&) const noexcept -> bool = default;

		private:
		DWRITE_TEXT_ALIGNMENT textAlignment;
	};

	namespace TextAlignments
	{
		constexpr TextAlignment Center = DWRITE_TEXT_ALIGNMENT_CENTER;
		constexpr TextAlignment Justified = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
		constexpr TextAlignment Leading = DWRITE_TEXT_ALIGNMENT_LEADING;
		constexpr TextAlignment Trailing = DWRITE_TEXT_ALIGNMENT_TRAILING;
	}

	struct WordWrapping
	{
		constexpr WordWrapping() noexcept :
			wordWrapping{ DWRITE_WORD_WRAPPING_WHOLE_WORD }
		{ }

		explicit(false) constexpr WordWrapping(const DWRITE_WORD_WRAPPING wordWrapping) noexcept :
			wordWrapping{ wordWrapping }
		{ }

		explicit(false) constexpr operator DWRITE_WORD_WRAPPING() const noexcept { return wordWrapping; }

		constexpr auto operator==(const WordWrapping&) const noexcept -> bool = default;

		private:
		DWRITE_WORD_WRAPPING wordWrapping;
	};

	namespace WordWrappings
	{
		constexpr WordWrapping Character = DWRITE_WORD_WRAPPING_CHARACTER;
		constexpr WordWrapping EmergencyBreak = DWRITE_WORD_WRAPPING_EMERGENCY_BREAK;
		constexpr WordWrapping NoWrap = DWRITE_WORD_WRAPPING_NO_WRAP;
		constexpr WordWrapping WholeWord = DWRITE_WORD_WRAPPING_WHOLE_WORD;
		constexpr WordWrapping Wrap = DWRITE_WORD_WRAPPING_WRAP;
	}

	struct FontAxisTag
	{
		constexpr FontAxisTag() noexcept :
			fontAxisTag{ DWRITE_FONT_AXIS_TAG_WEIGHT }
		{ }

		explicit(false) constexpr FontAxisTag(const DWRITE_FONT_AXIS_TAG fontAxisTag) noexcept :
			fontAxisTag{ fontAxisTag }
		{ }

		explicit(false) constexpr operator DWRITE_FONT_AXIS_TAG() const noexcept { return fontAxisTag; }

		constexpr auto operator==(const FontAxisTag&) const noexcept -> bool = default;

		private:
		DWRITE_FONT_AXIS_TAG fontAxisTag;
	};

	namespace FontAxisTags
	{
		constexpr FontAxisTag Weight = DWRITE_FONT_AXIS_TAG_WEIGHT;
		constexpr FontAxisTag Width = DWRITE_FONT_AXIS_TAG_WIDTH;
		constexpr FontAxisTag Slant = DWRITE_FONT_AXIS_TAG_SLANT;
	}

	struct LineSpacingMethod
	{
		constexpr LineSpacingMethod() noexcept :
			lineSpacingMethod{ DWRITE_LINE_SPACING_METHOD_DEFAULT }
		{ }

		explicit(false) constexpr LineSpacingMethod(const DWRITE_LINE_SPACING_METHOD lineSpacingMethod) noexcept :
			lineSpacingMethod{ lineSpacingMethod }
		{ }

		explicit(false) constexpr operator DWRITE_LINE_SPACING_METHOD() const noexcept { return lineSpacingMethod; }

		constexpr auto operator==(const LineSpacingMethod&) const noexcept -> bool = default;

		private:
		DWRITE_LINE_SPACING_METHOD lineSpacingMethod;
	};

	namespace LineSpacingMethods
	{
		constexpr LineSpacingMethod Default = DWRITE_LINE_SPACING_METHOD_DEFAULT;
		constexpr LineSpacingMethod Uniform = DWRITE_LINE_SPACING_METHOD_UNIFORM;
		constexpr LineSpacingMethod Proportional = DWRITE_LINE_SPACING_METHOD_PROPORTIONAL;
	}

	struct MeasuringMode
	{
		constexpr MeasuringMode() noexcept :
			measuringMode{ DWRITE_MEASURING_MODE_NATURAL }
		{ }

		explicit(false) constexpr MeasuringMode(const DWRITE_MEASURING_MODE measuringMode) noexcept :
			measuringMode{ measuringMode }
		{ }

		explicit(false) constexpr operator DWRITE_MEASURING_MODE() const noexcept { return measuringMode; }

		constexpr auto operator==(const MeasuringMode&) const noexcept -> bool = default;

		private:
		DWRITE_MEASURING_MODE measuringMode;
	};

	namespace MeasuringModes
	{
		constexpr MeasuringMode GDIClassic = DWRITE_MEASURING_MODE_GDI_CLASSIC;
		constexpr MeasuringMode GDINatural = DWRITE_MEASURING_MODE_GDI_NATURAL;
		constexpr MeasuringMode Natural = DWRITE_MEASURING_MODE_NATURAL;
	}
}

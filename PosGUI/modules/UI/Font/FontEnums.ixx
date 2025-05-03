module;
#include <dwrite.h>
#include <dwrite_3.h>

export module PGUI.UI.Font.FontEnums;

export namespace PGUI::UI::Font
{
	class FontWeight
	{
		public:
		constexpr FontWeight() noexcept :
			fontWeight{ DWRITE_FONT_WEIGHT_NORMAL }
		{
		}
		explicit(false) constexpr FontWeight(DWRITE_FONT_WEIGHT fontWeight) noexcept :
			fontWeight{ fontWeight }
		{
		}

		explicit(false) constexpr operator DWRITE_FONT_WEIGHT() const noexcept { return fontWeight; }

		private:
		DWRITE_FONT_WEIGHT fontWeight;

	};
	namespace FontWeights
	{
		const FontWeight Thin = DWRITE_FONT_WEIGHT_THIN;
		const FontWeight ExtraLight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
		const FontWeight UltraLight = DWRITE_FONT_WEIGHT_ULTRA_LIGHT;
		const FontWeight Light = DWRITE_FONT_WEIGHT_LIGHT;
		const FontWeight SemiLight = DWRITE_FONT_WEIGHT_SEMI_LIGHT;
		const FontWeight Normal = DWRITE_FONT_WEIGHT_NORMAL;
		const FontWeight Regular = DWRITE_FONT_WEIGHT_REGULAR;
		const FontWeight Medium = DWRITE_FONT_WEIGHT_MEDIUM;
		const FontWeight DemiBold = DWRITE_FONT_WEIGHT_DEMI_BOLD;
		const FontWeight SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD;
		const FontWeight Bold = DWRITE_FONT_WEIGHT_BOLD;
		const FontWeight ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD;
		const FontWeight UltraBold = DWRITE_FONT_WEIGHT_ULTRA_BOLD;
		const FontWeight Black = DWRITE_FONT_WEIGHT_BLACK;
		const FontWeight Heavy = DWRITE_FONT_WEIGHT_HEAVY;
		const FontWeight ExtraBlack = DWRITE_FONT_WEIGHT_EXTRA_BLACK;
		const FontWeight UltraBlack = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	}

	class FontStyle
	{
		public:
		constexpr FontStyle() noexcept :
			fontStyle{ DWRITE_FONT_STYLE_NORMAL }
		{
		}
		explicit(false) constexpr FontStyle(DWRITE_FONT_STYLE fontStyle) noexcept :
			fontStyle{ fontStyle }
		{
		}

		explicit(false) constexpr operator DWRITE_FONT_STYLE() const noexcept { return fontStyle; }

		private:
		DWRITE_FONT_STYLE fontStyle;
	};

	namespace FontStyles
	{
		const FontStyle Normal = DWRITE_FONT_STYLE_NORMAL;
		const FontStyle Oblique = DWRITE_FONT_STYLE_OBLIQUE;
		const FontStyle Italic = DWRITE_FONT_STYLE_ITALIC;
	}

	class FontStretch
	{
		public:
		constexpr FontStretch() noexcept :
			fontStretch{ DWRITE_FONT_STRETCH_NORMAL }
		{
		}
		explicit(false) constexpr FontStretch(DWRITE_FONT_STRETCH fontStretch) noexcept :
			fontStretch{ fontStretch }
		{
		}

		explicit(false) constexpr operator DWRITE_FONT_STRETCH() const noexcept { return fontStretch; }

		private:
		DWRITE_FONT_STRETCH fontStretch;
	};

	namespace FontStretches
	{
		const FontStretch Undefined = DWRITE_FONT_STRETCH_UNDEFINED;
		const FontStretch UltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED;
		const FontStretch ExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED;
		const FontStretch Condensed = DWRITE_FONT_STRETCH_CONDENSED;
		const FontStretch SemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED;
		const FontStretch Normal = DWRITE_FONT_STRETCH_NORMAL;
		const FontStretch Medium = DWRITE_FONT_STRETCH_MEDIUM;
		const FontStretch SemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED;
		const FontStretch Expanded = DWRITE_FONT_STRETCH_EXPANDED;
		const FontStretch ExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED;
		const FontStretch UltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED;
	}

	class FlowDirection
	{
		public:
		constexpr FlowDirection() noexcept :
			flowDirection{ DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT }
		{
		}
		explicit(false) constexpr FlowDirection(DWRITE_FLOW_DIRECTION flowDirection) noexcept :
			flowDirection{ flowDirection }
		{
		}

		explicit(false) constexpr operator DWRITE_FLOW_DIRECTION() const noexcept { return flowDirection; }

		private:
		DWRITE_FLOW_DIRECTION flowDirection;
	};

	namespace FlowDirections
	{
		const FlowDirection LeftToRight = DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT;
		const FlowDirection RightToLeft = DWRITE_FLOW_DIRECTION_RIGHT_TO_LEFT;
		const FlowDirection TopToBottom = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM;
		const FlowDirection BottomToTop = DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP;
	}

	class ParagraphAlignment
	{
		public:
		constexpr ParagraphAlignment() noexcept :
			paragraphAlignment{ DWRITE_PARAGRAPH_ALIGNMENT_CENTER }
		{
		}
		explicit(false) constexpr ParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment) noexcept :
			paragraphAlignment{ paragraphAlignment }
		{
		}

		explicit(false) constexpr operator DWRITE_PARAGRAPH_ALIGNMENT() const noexcept { return paragraphAlignment; }

		private:
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;
	};

	namespace ParagraphAlignments
	{
		const ParagraphAlignment Near = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		const ParagraphAlignment Center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		const ParagraphAlignment Far = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	}

	class ReadingDirection
	{
		public:
		constexpr ReadingDirection() noexcept :
			readingDirection{ DWRITE_READING_DIRECTION_LEFT_TO_RIGHT }
		{
		}
		explicit(false) constexpr ReadingDirection(DWRITE_READING_DIRECTION readingDirection) noexcept :
			readingDirection{ readingDirection }
		{
		}

		explicit(false) constexpr operator DWRITE_READING_DIRECTION() const noexcept { return readingDirection; }

		private:
		DWRITE_READING_DIRECTION readingDirection;
	};

	namespace ReadingDirections
	{
		const ReadingDirection LeftToRight = DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;
		const ReadingDirection RightToLeft = DWRITE_READING_DIRECTION_RIGHT_TO_LEFT;
		const ReadingDirection TopToBottom = DWRITE_READING_DIRECTION_TOP_TO_BOTTOM;
		const ReadingDirection BottomToTop = DWRITE_READING_DIRECTION_BOTTOM_TO_TOP;
	}

	class TextAlignment
	{
		public:
		constexpr TextAlignment() noexcept :
			textAlignment{ DWRITE_TEXT_ALIGNMENT_CENTER }
		{
		}
		explicit(false) constexpr TextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment) noexcept :
			textAlignment{ textAlignment }
		{
		}

		explicit(false) constexpr operator DWRITE_TEXT_ALIGNMENT() const noexcept { return textAlignment; }

		private:
		DWRITE_TEXT_ALIGNMENT textAlignment;
	};

	namespace TextAlignments
	{
		const TextAlignment Center = DWRITE_TEXT_ALIGNMENT_CENTER;
		const TextAlignment Justified = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
		const TextAlignment Leading = DWRITE_TEXT_ALIGNMENT_LEADING;
		const TextAlignment Trailing = DWRITE_TEXT_ALIGNMENT_TRAILING;
	}

	class WordWrapping
	{
		public:
		constexpr WordWrapping() noexcept :
			wordWrapping{ DWRITE_WORD_WRAPPING_EMERGENCY_BREAK }
		{
		}
		explicit(false) constexpr WordWrapping(DWRITE_WORD_WRAPPING wordWrapping) noexcept :
			wordWrapping{ wordWrapping }
		{
		}

		explicit(false) constexpr operator DWRITE_WORD_WRAPPING() const noexcept { return wordWrapping; }

		private:
		DWRITE_WORD_WRAPPING wordWrapping;
	};

	namespace WordWrappings
	{
		const WordWrapping Character = DWRITE_WORD_WRAPPING_CHARACTER;
		const WordWrapping EmergencyBreak = DWRITE_WORD_WRAPPING_EMERGENCY_BREAK;
		const WordWrapping NoWrap = DWRITE_WORD_WRAPPING_NO_WRAP;
		const WordWrapping WholeWord = DWRITE_WORD_WRAPPING_WHOLE_WORD;
		const WordWrapping Wrap = DWRITE_WORD_WRAPPING_WRAP;
	}

	class FontAxisTag
	{
		public:
		constexpr FontAxisTag() noexcept :
			fontAxisTag{ DWRITE_FONT_AXIS_TAG_WEIGHT }
		{
		}
		explicit(false) constexpr FontAxisTag(DWRITE_FONT_AXIS_TAG fontAxisTag) noexcept :
			fontAxisTag{ fontAxisTag }
		{
		}
		explicit(false) constexpr operator DWRITE_FONT_AXIS_TAG() const noexcept { return fontAxisTag; }

		private:
		DWRITE_FONT_AXIS_TAG fontAxisTag;
	};

	namespace FontAxisTags
	{
		const FontAxisTag Weight = DWRITE_FONT_AXIS_TAG_WEIGHT;
		const FontAxisTag Width = DWRITE_FONT_AXIS_TAG_WIDTH;
		const FontAxisTag Slant = DWRITE_FONT_AXIS_TAG_SLANT;
	}

	class LineSpacingMethod
	{
		public:
		constexpr LineSpacingMethod() noexcept :
			lineSpacingMethod{ DWRITE_LINE_SPACING_METHOD_DEFAULT }
		{
		}
		explicit(false) constexpr LineSpacingMethod(DWRITE_LINE_SPACING_METHOD lineSpacingMethod) noexcept :
			lineSpacingMethod{ lineSpacingMethod }
		{
		}
		explicit(false) constexpr operator DWRITE_LINE_SPACING_METHOD() const noexcept { return lineSpacingMethod; }

		private:
		DWRITE_LINE_SPACING_METHOD lineSpacingMethod;
	};

	namespace LineSpacingMethods
	{
		const LineSpacingMethod Default = DWRITE_LINE_SPACING_METHOD_DEFAULT;
		const LineSpacingMethod Uniform = DWRITE_LINE_SPACING_METHOD_UNIFORM;
		const LineSpacingMethod Proportional = DWRITE_LINE_SPACING_METHOD_PROPORTIONAL;
	}

	class MeasuringMode
	{
		public:
		constexpr MeasuringMode() noexcept :
			measuringMode{ DWRITE_MEASURING_MODE_NATURAL }
		{
		}
		explicit(false) constexpr MeasuringMode(DWRITE_MEASURING_MODE measuringMode) noexcept :
			measuringMode{ measuringMode }
		{
		}
		explicit(false) constexpr operator DWRITE_MEASURING_MODE() const noexcept { return measuringMode; }

		private:
		DWRITE_MEASURING_MODE measuringMode;
	};
}

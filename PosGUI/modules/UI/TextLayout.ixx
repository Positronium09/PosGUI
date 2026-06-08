module;
#include <dwrite_3.h>

export module PGUI.UI.TextLayout;

import std;

import PGUI.UI.TextFormat;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Shape2D;
import PGUI.UI.Font.FontCollection;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;

export namespace PGUI::UI
{
	using namespace Font;

	class TextLayout : public ComPtrHolder<IDWriteTextLayout4>
	{
		public:
		explicit(false) TextLayout(const ComPtr<IDWriteTextLayout4>& textLayout) noexcept;

		TextLayout(wzstring_view text, const TextFormat& textFormat, SizeF maxSize) noexcept;

		[[nodiscard]] auto SetTextAlignment(TextAlignment textAlignment) const noexcept -> Result<void>;

		[[nodiscard]] auto SetParagraphAlignment(ParagraphAlignment paragraphAlignment) const noexcept -> Result<void>;

		[[nodiscard]] auto SetWordWrapping(WordWrapping wordWrapping) const noexcept -> Result<void>;

		[[nodiscard]] auto SetReadingDirection(ReadingDirection readingDirection) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFlowDirection(FlowDirection flowDirection) const noexcept -> Result<void>;

		[[nodiscard]] auto SetIncrementalTabStop(float incrementalTabStop) const noexcept -> Result<void>;

		[[nodiscard]] auto SetLineSpacing(const LineSpacing& lineSpacing) const noexcept -> Result<void>;

		[[nodiscard]] auto SetMaxWidth(float maxWidth) const noexcept -> Result<void>;

		[[nodiscard]] auto SetMaxHeight(float maxHeight) const noexcept -> Result<void>;

		[[nodiscard]] auto SetMaxSize(SizeF maxSize) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFontCollection(const FontCollection& fontCollection, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFontFamilyName(wzstring_view fontFamilyName, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFontWeight(FontWeight fontWeight, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFontStyle(FontStyle fontStyle, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFontStretch(FontStretch fontStretch, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetFontSize(float fontSize, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetUnderline(bool hasUnderline, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetStrikethrough(bool hasStrikethrough, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetDrawingEffect(const ComPtr<IUnknown>& drawingEffect, TextRange textRange) const noexcept -> Result<void>;

		//? Maybe write a wrapper for IDWriteInlineObject
		[[nodiscard]] auto SetInlineObject(const ComPtr<IDWriteInlineObject>& inlineObject, TextRange textRange) const noexcept -> Result<void>;

		//? Maybe write a wrapper for IDWriteTypography
		[[nodiscard]] auto SetTypography(const ComPtr<IDWriteTypography>& typography, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetLocaleName(wzstring_view localeName, TextRange textRange) const noexcept -> Result<void>;

		[[nodiscard]] auto SetTrimming(const Trimming& trimming) const noexcept -> Result<void>;

		[[nodiscard]] auto GetTrimming() const noexcept -> Result<Trimming>;

		[[nodiscard]] auto GetTextAlignment() const noexcept -> TextAlignment;

		[[nodiscard]] auto GetParagraphAlignment() const noexcept -> ParagraphAlignment;

		[[nodiscard]] auto GetWordWrapping() const noexcept -> WordWrapping;

		[[nodiscard]] auto GetReadingDirection() const noexcept -> ReadingDirection;

		[[nodiscard]] auto GetFlowDirection() const noexcept -> FlowDirection;

		[[nodiscard]] auto GetIncrementalTabStop() const noexcept -> float;

		[[nodiscard]] auto GetLineSpacing() const noexcept -> Result<LineSpacing>;

		[[nodiscard]] auto GetMaxWidth() const noexcept -> float;

		[[nodiscard]] auto GetMaxHeight() const noexcept -> float;

		[[nodiscard]] auto GetFontCollection() const noexcept -> Result<FontCollection>;

		[[nodiscard]] auto GetFontCollection(UINT32 position) const noexcept -> Result<FontCollection>;

		[[nodiscard]] auto GetFontCollection(UINT32 position, TextRange& textRange) const noexcept -> Result<FontCollection>;

		[[nodiscard]] auto GetFontFamilyName() const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetFontFamilyName(UINT32 position) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetFontFamilyName(UINT32 position, TextRange& textRange) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetFontWeight() const noexcept -> FontWeight;

		[[nodiscard]] auto GetFontWeight(UINT32 position) const noexcept -> Result<FontWeight>;

		[[nodiscard]] auto GetFontWeight(UINT32 position, TextRange& textRange) const noexcept -> Result<FontWeight>;

		[[nodiscard]] auto GetFontStyle() const noexcept -> FontStyle;

		[[nodiscard]] auto GetFontStyle(UINT32 position) const noexcept -> Result<FontStyle>;

		[[nodiscard]] auto GetFontStyle(UINT32 position, TextRange& textRange) const noexcept -> Result<FontStyle>;

		[[nodiscard]] auto GetFontStretch() const noexcept -> FontStretch;

		[[nodiscard]] auto GetFontStretch(UINT32 position) const noexcept -> Result<FontStretch>;

		[[nodiscard]] auto GetFontStretch(UINT32 position, TextRange& textRange) const noexcept -> Result<FontStretch>;

		[[nodiscard]] auto GetFontSize() const noexcept -> float;

		[[nodiscard]] auto GetFontSize(UINT32 position) const noexcept -> Result<float>;

		[[nodiscard]] auto GetFontSize(UINT32 position, TextRange& textRange) const noexcept -> Result<float>;

		[[nodiscard]] auto GetUnderline(UINT32 position) const noexcept -> Result<bool>;

		[[nodiscard]] auto GetUnderline(UINT32 position, TextRange& textRange) const noexcept -> Result<bool>;

		[[nodiscard]] auto GetStrikethrough(UINT32 position) const noexcept -> Result<bool>;

		[[nodiscard]] auto GetStrikethrough(UINT32 position, TextRange& textRange) const noexcept -> Result<bool>;

		[[nodiscard]] auto GetDrawingEffect(UINT32 position) const noexcept -> Result<ComPtr<IUnknown>>;

		[[nodiscard]] auto GetDrawingEffect(UINT32 position, TextRange& textRange) const noexcept -> Result<ComPtr<IUnknown>>;

		[[nodiscard]] auto GetInlineObject(UINT32 position) const noexcept -> Result<ComPtr<IDWriteInlineObject>>;

		[[nodiscard]] auto GetInlineObject(
			UINT32 position,
			TextRange& textRange) const noexcept -> Result<ComPtr<IDWriteInlineObject>>;

		[[nodiscard]] auto GetTypography(UINT32 position) const noexcept -> Result<ComPtr<IDWriteTypography>>;

		[[nodiscard]] auto GetTypography(
			UINT32 position,
			TextRange& textRange) const noexcept -> Result<ComPtr<IDWriteTypography>>;

		[[nodiscard]] auto GetLocaleName() const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetLocaleName(UINT32 position) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetLocaleName(UINT32 position, TextRange& textRange) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto DetermineMinWidth() const noexcept -> Result<float>;

		[[nodiscard]] auto GetMetrics() const noexcept -> Result<TextMetrics>;

		[[nodiscard]] auto GetLineMetrics() const noexcept -> Result<std::vector<LineMetrics>>;

		[[nodiscard]] auto GetTextLength() const noexcept -> Result<UINT32>;

		//! Wont probably implement
		//! HRESULT Draw(void* clientDrawingContext, IDWriteTextRenderer* renderer, FLOAT originX, FLOAT originY);

		//TODO Write wrappers for all
		//TODO dawg i mustve been so done w ts shi to leave em out
		/*
			HRESULT GetOverhangMetrics(DWRITE_OVERHANG_METRICS* overhangs);
			HRESULT GetClusterMetrics(DWRITE_CLUSTER_METRICS* clusterMetrics, UINT32 maxClusterCount, UINT32* actualClusterCount);
			HRESULT HitTestPoint(FLOAT pointX, FLOAT pointY, BOOL* isTrailingHit, BOOL* isInside, DWRITE_HIT_TEST_METRICS* hitTestMetrics);
			HRESULT HitTestTextPosition(UINT32 textPosition, BOOL isTrailingHit, FLOAT* pointX, FLOAT* pointY, DWRITE_HIT_TEST_METRICS* hitTestMetrics);
			HRESULT HitTestTextRange(UINT32 textPosition, UINT32 textLength, FLOAT originX, FLOAT originY, DWRITE_HIT_TEST_METRICS* hitTestMetrics, UINT32 maxHitTestMetricsCount, UINT32* actualHitTestMetricsCount);
		*/
	};
}

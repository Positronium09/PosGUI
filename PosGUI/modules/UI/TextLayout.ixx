module;
#include <dwrite_3.h>
#include <wrl.h>

export module PGUI.UI.TextLayout;

import std;

import PGUI.UI.TextFormat;
import PGUI.ComPtr;
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

		TextLayout(std::wstring_view text, const TextFormat& textFormat, SizeF maxSize) noexcept;

		auto SetTextAlignment(TextAlignment textAlignment) const noexcept -> Error;

		auto SetParagraphAlignment(ParagraphAlignment paragraphAlignment) const noexcept -> Error;

		auto SetWordWrapping(WordWrapping wordWrapping) const noexcept -> Error;

		auto SetReadingDirection(ReadingDirection readingDirection) const noexcept -> Error;

		auto SetFlowDirection(FlowDirection flowDirection) const noexcept -> Error;

		auto SetIncrementalTabStop(float incrementalTabStop) const noexcept -> Error;

		auto SetLineSpacing(const LineSpacing& lineSpacing) const noexcept -> Error;

		auto SetMaxWidth(float maxWidth) const noexcept -> Error;

		auto SetMaxHeight(float maxHeight) const noexcept -> Error;

		auto SetMaxSize(SizeF maxSize) const noexcept -> Error;

		auto SetFontCollection(const FontCollection& fontCollection, TextRange textRange) const noexcept -> Error;

		auto SetFontFamilyName(std::wstring_view fontFamilyName, TextRange textRange) const noexcept -> Error;

		auto SetFontWeight(FontWeight fontWeight, TextRange textRange) const noexcept -> Error;

		auto SetFontStyle(FontStyle fontStyle, TextRange textRange) const noexcept -> Error;

		auto SetFontStretch(FontStretch fontStretch, TextRange textRange) const noexcept -> Error;

		auto SetFontSize(float fontSize, TextRange textRange) const noexcept -> Error;

		auto SetUnderline(bool hasUnderline, TextRange textRange) const noexcept -> Error;

		auto SetStrikethrough(bool hasStrikethrough, TextRange textRange) const noexcept -> Error;

		auto SetDrawingEffect(const ComPtr<IUnknown>& drawingEffect, TextRange textRange) const noexcept -> Error;

		//? Maybe write a wrapper for IDWriteInlineObject
		auto SetInlineObject(const ComPtr<IDWriteInlineObject>& inlineObject, TextRange textRange) const noexcept -> Error;

		//? Maybe write a wrapper for IDWriteTypography
		auto SetTypography(const ComPtr<IDWriteTypography>& typography, TextRange textRange) const noexcept -> Error;

		auto SetLocaleName(std::wstring_view localeName, TextRange textRange) const noexcept -> Error;

		auto SetTrimming(const Trimming& trimming) const noexcept -> Error;

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

		//! Wont probably implement
		//! HRESULT Draw(void* clientDrawingContext, IDWriteTextRenderer* renderer, FLOAT originX, FLOAT originY);

		//TODO Write wrappers for all
		//TODO dawg i mustve been so done w ts shi to leave em out
		/*
			HRESULT GetLineMetrics(DWRITE_LINE_METRICS* lineMetrics, UINT32 maxLineCount, UINT32* actualLineCount);
			HRESULT GetMetrics(DWRITE_TEXT_METRICS* textMetrics);
			HRESULT GetOverhangMetrics(DWRITE_OVERHANG_METRICS* overhangs);
			HRESULT GetClusterMetrics(DWRITE_CLUSTER_METRICS* clusterMetrics, UINT32 maxClusterCount, UINT32* actualClusterCount);
			HRESULT HitTestPoint(FLOAT pointX, FLOAT pointY, BOOL* isTrailingHit, BOOL* isInside, DWRITE_HIT_TEST_METRICS* hitTestMetrics);
			HRESULT HitTestTextPosition(UINT32 textPosition, BOOL isTrailingHit, FLOAT* pointX, FLOAT* pointY, DWRITE_HIT_TEST_METRICS* hitTestMetrics);
			HRESULT HitTestTextRange(UINT32 textPosition, UINT32 textLength, FLOAT originX, FLOAT originY, DWRITE_HIT_TEST_METRICS* hitTestMetrics, UINT32 maxHitTestMetricsCount, UINT32* actualHitTestMetricsCount);
		*/
	};
}

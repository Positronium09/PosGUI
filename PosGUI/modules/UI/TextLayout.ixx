module;
#include <dwrite_3.h>
#include <wrl.h>
#include <string>

export module PGUI.UI.TextLayout;

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
		TextLayout(ComPtr<IDWriteTextLayout4> textLayout) noexcept;
		TextLayout(std::wstring_view text, TextFormat textFormat, SizeF maxSize) noexcept;

		void SetTextAlignment(TextAlignment textAlignment) const noexcept;
		void SetParagraphAlignment(ParagraphAlignment paragraphAlignment) const noexcept;
		void SetWordWrapping(WordWrapping wordWrapping) const noexcept;
		void SetReadingDirection(ReadingDirection readingDirection) const noexcept;
		void SetFlowDirection(FlowDirection flowDirection) const noexcept;
		void SetIncrementalTabStop(float incrementalTabStop) const noexcept;
		void SetLineSpacing(LineSpacing lineSpacing) const noexcept;
		void SetMaxWidth(float maxWidth) const noexcept;
		void SetMaxHeight(float maxHeight) const noexcept;
		void SetMaxSize(SizeF maxSize) const noexcept;
		void SetFontCollection(FontCollection fontCollection, TextRange textRange) const noexcept;
		void SetFontFamilyName(std::wstring_view fontFamilyName, TextRange textRange) const noexcept;
		void SetFontWeight(FontWeight fontWeight, TextRange textRange) const noexcept;
		void SetFontStyle(FontStyle fontStyle, TextRange textRange) const noexcept;
		void SetFontStretch(FontStretch fontStretch, TextRange textRange) const noexcept;
		void SetFontSize(float fontSize, TextRange textRange) const noexcept;
		void SetUnderline(bool hasUnderline, TextRange textRange) const noexcept;
		void SetStrikethrough(bool hasStrikethrough, TextRange textRange) const noexcept;
		void SetDrawingEffect(ComPtr<IUnknown> drawingEffect, TextRange textRange) const noexcept;
		//? Maybe write a wrapper for IDWriteInlineObject
		void SetInlineObject(ComPtr<IDWriteInlineObject> inlineObject, TextRange textRange) const noexcept;
		//? Maybe write a wrapper for IDWriteTypography
		void SetTypography(ComPtr<IDWriteTypography> typography, TextRange textRange) const noexcept;
		void SetLocaleName(std::wstring_view localeName, TextRange textRange) const noexcept;
		void SetTrimming(const Trimming& trimming) const noexcept;

		[[nodiscard]] auto GetTrimming() const noexcept -> Trimming;
		[[nodiscard]] auto GetTextAlignment() const noexcept -> TextAlignment;
		[[nodiscard]] auto GetParagraphAlignment() const noexcept -> ParagraphAlignment;
		[[nodiscard]] auto GetWordWrapping() const noexcept -> WordWrapping;
		[[nodiscard]] auto GetReadingDirection() const noexcept -> ReadingDirection;
		[[nodiscard]] auto GetFlowDirection() const noexcept -> FlowDirection;
		[[nodiscard]] auto GetIncrementalTabStop() const noexcept -> float;
		[[nodiscard]] auto GetLineSpacing() const noexcept -> LineSpacing;
		[[nodiscard]] auto GetMaxWidth() const noexcept -> float;
		[[nodiscard]] auto GetMaxHeight() const noexcept -> float;

		[[nodiscard]] auto GetFontCollection() const noexcept -> FontCollection;
		[[nodiscard]] auto GetFontCollection(UINT32 position) const noexcept -> FontCollection;
		[[nodiscard]] auto GetFontCollection(UINT32 position, TextRange& textRange) const noexcept -> FontCollection;
		[[nodiscard]] auto GetFontFamilyName() const noexcept -> std::wstring;
		[[nodiscard]] auto GetFontFamilyName(UINT32 position) const noexcept -> std::wstring;
		[[nodiscard]] auto GetFontFamilyName(UINT32 position, TextRange& textRange) const noexcept -> std::wstring;
		[[nodiscard]] auto GetFontWeight() const noexcept -> FontWeight;
		[[nodiscard]] auto GetFontWeight(UINT32 position) const noexcept -> FontWeight;
		[[nodiscard]] auto GetFontWeight(UINT32 position, TextRange& textRange) const noexcept -> FontWeight;
		[[nodiscard]] auto GetFontStyle() const noexcept -> FontStyle;
		[[nodiscard]] auto GetFontStyle(UINT32 position) const noexcept -> FontStyle;
		[[nodiscard]] auto GetFontStyle(UINT32 position, TextRange& textRange) const noexcept -> FontStyle;
		[[nodiscard]] auto GetFontStretch() const noexcept -> FontStretch;
		[[nodiscard]] auto GetFontStretch(UINT32 position) const noexcept -> FontStretch;
		[[nodiscard]] auto GetFontStretch(UINT32 position, TextRange& textRange) const noexcept -> FontStretch;
		[[nodiscard]] auto GetFontSize() const noexcept -> float;
		[[nodiscard]] auto GetFontSize(UINT32 position) const noexcept -> float;
		[[nodiscard]] auto GetFontSize(UINT32 position, TextRange& textRange) const noexcept -> float;
		[[nodiscard]] auto GetUnderline(UINT32 position) const noexcept -> bool;
		[[nodiscard]] auto GetUnderline(UINT32 position, TextRange& textRange) const noexcept -> bool;
		[[nodiscard]] auto GetStrikethrough(UINT32 position) const noexcept -> bool;
		[[nodiscard]] auto GetStrikethrough(UINT32 position, TextRange& textRange) const noexcept -> bool;
		[[nodiscard]] auto GetDrawingEffect(UINT32 position) const noexcept -> ComPtr<IUnknown>;
		[[nodiscard]] auto GetDrawingEffect(UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IUnknown>;
		[[nodiscard]] auto GetInlineObject(UINT32 position) const noexcept -> ComPtr<IDWriteInlineObject>;
		[[nodiscard]] auto GetInlineObject(UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IDWriteInlineObject>;
		[[nodiscard]] auto GetTypography(UINT32 position) const noexcept -> ComPtr<IDWriteTypography>;
		[[nodiscard]] auto GetTypography(UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IDWriteTypography>;
		[[nodiscard]] auto GetLocaleName() const noexcept -> std::wstring;
		[[nodiscard]] auto GetLocaleName(UINT32 position) const noexcept -> std::wstring;
		[[nodiscard]] auto GetLocaleName(UINT32 position, TextRange& textRange) const noexcept -> std::wstring;

		[[nodiscard]] auto DetermineMinWidth() const noexcept -> float;

		//! Wont probably implement
		//! HRESULT Draw(void* clientDrawingContext, IDWriteTextRenderer* renderer, FLOAT originX, FLOAT originY);

		//TODO Write wrappers for all
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

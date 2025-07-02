module;
#include <dwrite_3.h>

export module PGUI.UI.TextFormat;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.UI.Font.FontCollection;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;

export namespace PGUI::UI
{
	using namespace Font;

	class TextFormat : public ComPtrHolder<IDWriteTextFormat3>
	{
		public:
		TextFormat(ComPtr<IDWriteTextFormat3> textFormat) noexcept;
		TextFormat(std::wstring_view fontFamilyName, float fontSize,
			FontWeight fontWeight = FontWeights::Normal,
			FontStyle fontStyle = FontStyles::Normal,
			FontStretch fontStretch = FontStretches::Normal,
			FontCollection fontCollection = FontCollection::GetSystemFontCollection(),
			std::wstring_view localeName = GetUserLocaleName()) noexcept;
		TextFormat(std::wstring_view fontFamilyName, float fontSize,
			std::span<const FontAxisValue> fontAxisValues,
			FontCollection fontCollection = FontCollection::GetSystemFontCollection(),
			std::wstring_view localeName = GetUserLocaleName()) noexcept;

		void SetFlowDirection(FlowDirection flowDirection) noexcept;
		void SetIncrementalTabStop(float incrementalTabStop) noexcept;
		void SetParagraphAlignment(ParagraphAlignment paragraphAlignment) noexcept;
		void SetReadingDirection(ReadingDirection readingDirection) noexcept;
		void SetTextAlignment(TextAlignment textAlignment) noexcept;
		void SetWordWrapping(WordWrapping wordWrapping) noexcept;
		void SetTrimming(const Trimming& trimming) noexcept;
		void SetLineSpacing(LineSpacing lineSpacing) noexcept;

		[[nodiscard]] auto GetFlowDirection() const noexcept -> FlowDirection;
		[[nodiscard]] auto GetFontCollection() const noexcept -> FontCollection;
		[[nodiscard]] auto GetFontFamilyName() const noexcept -> std::wstring;
		[[nodiscard]] auto GetFontSize() const noexcept -> float;
		[[nodiscard]] auto GetFontStretch() const noexcept -> FontStretch;
		[[nodiscard]] auto GetFontStyle() const noexcept -> FontStyle;
		[[nodiscard]] auto GetFontWeight() const noexcept -> FontWeight;
		[[nodiscard]] auto GetIncrementalTabStop() const noexcept -> float;
		[[nodiscard]] auto GetLocaleName() const noexcept -> std::wstring;
		[[nodiscard]] auto GetParagraphAlignment() const noexcept -> ParagraphAlignment;
		[[nodiscard]] auto GetReadingDirection() const noexcept -> ReadingDirection;
		[[nodiscard]] auto GetTextAlignment() const noexcept -> TextAlignment;
		[[nodiscard]] auto GetWordWrapping() const noexcept -> WordWrapping;
		[[nodiscard]] auto GetTrimming() const noexcept -> Trimming;
		[[nodiscard]] auto GetLineSpacing() const noexcept -> LineSpacing;
	};
}

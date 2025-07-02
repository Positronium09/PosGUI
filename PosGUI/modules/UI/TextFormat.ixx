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
		explicit(false) TextFormat(ComPtr<IDWriteTextFormat3> textFormat) noexcept;

		TextFormat(
			std::wstring_view fontFamilyName, float fontSize,
			FontWeight fontWeight = FontWeights::Normal,
			FontStyle fontStyle = FontStyles::Normal,
			FontStretch fontStretch = FontStretches::Normal,
			FontCollection fontCollection = FontCollection::GetSystemFontCollection(),
			std::wstring_view localeName = GetUserLocaleName()) noexcept;

		TextFormat(std::wstring_view fontFamilyName, float fontSize,
		           std::span<const FontAxisValue> fontAxisValues,
		           FontCollection fontCollection = FontCollection::GetSystemFontCollection(),
		           std::wstring_view localeName = GetUserLocaleName()) noexcept;

		auto SetFlowDirection(FlowDirection flowDirection) noexcept -> void;

		auto SetIncrementalTabStop(float incrementalTabStop) noexcept -> void;

		auto SetParagraphAlignment(ParagraphAlignment paragraphAlignment) noexcept -> void;

		auto SetReadingDirection(ReadingDirection readingDirection) noexcept -> void;

		auto SetTextAlignment(TextAlignment textAlignment) noexcept -> void;

		auto SetWordWrapping(WordWrapping wordWrapping) noexcept -> void;

		auto SetTrimming(const Trimming& trimming) noexcept -> void;

		auto SetLineSpacing(LineSpacing lineSpacing) noexcept -> void;

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

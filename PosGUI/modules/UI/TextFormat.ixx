module;
#include <dwrite_3.h>

export module PGUI.UI.TextFormat;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.Font.FontCollection;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;

export namespace PGUI::UI
{
	using namespace Font;

	class TextFormat : public ComPtrHolder<IDWriteTextFormat3>
	{
		public:
		explicit(false) TextFormat(const ComPtr<IDWriteTextFormat3>& textFormat) noexcept;

		TextFormat(
			std::wstring_view fontFamilyName, float fontSize,
			FontWeight fontWeight = FontWeights::Normal,
			FontStyle fontStyle = FontStyles::Normal,
			FontStretch fontStretch = FontStretches::Normal,
			const FontCollection& fontCollection = FontCollection{ nullptr },
			std::wstring_view localeName = GetUserLocaleName()) noexcept;

		TextFormat(std::wstring_view fontFamilyName, float fontSize,
		           std::span<const FontAxisValue> fontAxisValues,
					const FontCollection& fontCollection = FontCollection{ nullptr },
		           std::wstring_view localeName = GetUserLocaleName()) noexcept;

		auto SetFlowDirection(FlowDirection flowDirection) noexcept -> Error;

		auto SetIncrementalTabStop(float incrementalTabStop) noexcept -> Error;

		auto SetParagraphAlignment(ParagraphAlignment paragraphAlignment) noexcept -> Error;

		auto SetReadingDirection(ReadingDirection readingDirection) noexcept -> Error;

		auto SetTextAlignment(TextAlignment textAlignment) noexcept -> Error;

		auto SetWordWrapping(WordWrapping wordWrapping) noexcept -> Error;

		auto SetTrimming(const Trimming& trimming) noexcept -> Error;

		auto SetLineSpacing(const LineSpacing& lineSpacing) noexcept -> Error;

		[[nodiscard]] auto GetFlowDirection() const noexcept -> FlowDirection;

		[[nodiscard]] auto GetFontCollection() const noexcept -> Result<FontCollection>;

		[[nodiscard]] auto GetFontFamilyName() const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetFontSize() const noexcept -> float;

		[[nodiscard]] auto GetFontStretch() const noexcept -> FontStretch;

		[[nodiscard]] auto GetFontStyle() const noexcept -> FontStyle;

		[[nodiscard]] auto GetFontWeight() const noexcept -> FontWeight;

		[[nodiscard]] auto GetIncrementalTabStop() const noexcept -> float;

		[[nodiscard]] auto GetLocaleName() const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetParagraphAlignment() const noexcept -> ParagraphAlignment;

		[[nodiscard]] auto GetReadingDirection() const noexcept -> ReadingDirection;

		[[nodiscard]] auto GetTextAlignment() const noexcept -> TextAlignment;

		[[nodiscard]] auto GetWordWrapping() const noexcept -> WordWrapping;

		[[nodiscard]] auto GetTrimming() const noexcept -> Result<Trimming>;

		[[nodiscard]] auto GetLineSpacing() const noexcept -> Result<LineSpacing>;
	};
}

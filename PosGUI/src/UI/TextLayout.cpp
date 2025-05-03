module;
#include <dwrite_3.h>
#include <wrl.h>
#include <bit>
#include <string>

module PGUI.UI.TextLayout;

import PGUI.UI.TextFormat;
import PGUI.ComPtr;
import PGUI.Logging;
import PGUI.Exceptions;
import PGUI.Factories;

namespace PGUI::UI
{
	using namespace Font;

	TextLayout::TextLayout(ComPtr<IDWriteTextLayout4> textLayout) noexcept : 
		ComPtrHolder{ textLayout }
	{
	}

	TextLayout::TextLayout(std::wstring_view text, TextFormat textFormat, SizeF maxSize) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();
		auto hr = factory->CreateTextLayout(text.data(), static_cast<UINT32>(text.size()),
			textFormat.GetRawAs<IDWriteTextFormat3, IDWriteTextFormat>(),
			maxSize.cx, maxSize.cy, 
			std::bit_cast<IDWriteTextLayout**>(GetAddress())); ThrowFailed(hr);
	}

	void TextLayout::SetTextAlignment(TextAlignment textAlignment) const noexcept
	{
		auto hr = Get()->SetTextAlignment(textAlignment); ThrowFailed(hr);
	}
	void TextLayout::SetParagraphAlignment(ParagraphAlignment paragraphAlignment) const noexcept
	{
		auto hr = Get()->SetParagraphAlignment(paragraphAlignment); ThrowFailed(hr);
	}
	void TextLayout::SetWordWrapping(WordWrapping wordWrapping) const noexcept
	{
		auto hr = Get()->SetWordWrapping(wordWrapping); ThrowFailed(hr);
	}
	void TextLayout::SetReadingDirection(ReadingDirection readingDirection) const noexcept
	{
		auto hr = Get()->SetReadingDirection(readingDirection); ThrowFailed(hr);
	}
	void TextLayout::SetFlowDirection(FlowDirection flowDirection) const noexcept
	{
		auto hr = Get()->SetFlowDirection(flowDirection); ThrowFailed(hr);
	}
	void TextLayout::SetIncrementalTabStop(float incrementalTabStop) const noexcept
	{
		auto hr = Get()->SetIncrementalTabStop(incrementalTabStop); ThrowFailed(hr);
	}
	void TextLayout::SetLineSpacing(LineSpacing lineSpacing) const noexcept
	{
		auto hr = Get()->SetLineSpacing(lineSpacing.method, 
			lineSpacing.height, lineSpacing.baseline); ThrowFailed(hr);
	}
	void TextLayout::SetMaxWidth(float maxWidth) const noexcept
	{
		auto hr = Get()->SetMaxWidth(maxWidth); ThrowFailed(hr);
	}
	void TextLayout::SetMaxHeight(float maxHeight) const noexcept
	{
		auto hr = Get()->SetMaxHeight(maxHeight); ThrowFailed(hr);
	}
	void TextLayout::SetMaxSize(SizeF maxSize) const noexcept
	{
		SetMaxWidth(maxSize.cx);
		SetMaxHeight(maxSize.cy);
	}
	void TextLayout::SetFontCollection(FontCollection fontCollection, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetFontCollection(
			fontCollection.GetRawAs<IDWriteFontCollection3, IDWriteFontCollection>(),
			textRange); ThrowFailed(hr);
	}
	void TextLayout::SetFontFamilyName(std::wstring_view fontFamilyName, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetFontFamilyName(fontFamilyName.data(), textRange); ThrowFailed(hr);
	}
	void TextLayout::SetFontWeight(FontWeight fontWeight, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetFontWeight(fontWeight, textRange); ThrowFailed(hr);
	}
	void TextLayout::SetFontStyle(FontStyle fontStyle, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetFontStyle(fontStyle, textRange); ThrowFailed(hr);
	}
	void TextLayout::SetFontStretch(FontStretch fontStretch, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetFontStretch(fontStretch, textRange); ThrowFailed(hr);
	}
	void TextLayout::SetFontSize(float fontSize, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetFontSize(fontSize, textRange); ThrowFailed(hr);
	}
	void TextLayout::SetUnderline(bool hasUnderline, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetUnderline(hasUnderline, textRange); ThrowFailed(hr);
	}
	void TextLayout::SetStrikethrough(bool hasStrikethrough, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetStrikethrough(hasStrikethrough, textRange); ThrowFailed(hr);
	}
	void TextLayout::SetDrawingEffect(ComPtr<IUnknown> drawingEffect, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetDrawingEffect(drawingEffect.Get(), textRange); ThrowFailed(hr);
	}
	void TextLayout::SetInlineObject(ComPtr<IDWriteInlineObject> inlineObject, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetInlineObject(inlineObject.Get(), textRange); ThrowFailed(hr);
	}
	void TextLayout::SetTypography(ComPtr<IDWriteTypography> typography, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetTypography(typography.Get(), textRange); ThrowFailed(hr);
	}
	void TextLayout::SetLocaleName(std::wstring_view localeName, TextRange textRange) const noexcept
	{
		auto hr = Get()->SetLocaleName(localeName.data(), textRange); ThrowFailed(hr);
	}
	void TextLayout::SetTrimming(const Trimming& trimming) const noexcept
	{
		auto& trimmingOptions = trimming.trimmingOptions;
		auto hr = Get()->SetTrimming(&trimmingOptions, trimming.GetRaw()); ThrowFailed(hr);
	}

	auto TextLayout::GetTrimming() const noexcept -> Trimming
	{
		Trimming trimming{ };
		auto hr = Get()->GetTrimming(&trimming.trimmingOptions, 
			trimming.GetAddress()); ThrowFailed(hr);

		return trimming;
	}
	auto TextLayout::GetTextAlignment() const noexcept -> TextAlignment
	{
		return Get()->GetTextAlignment();
	}
	auto TextLayout::GetParagraphAlignment() const noexcept -> ParagraphAlignment
	{
		return Get()->GetParagraphAlignment();
	}
	auto TextLayout::GetWordWrapping() const noexcept -> WordWrapping
	{
		return Get()->GetWordWrapping();
	}
	auto TextLayout::GetReadingDirection() const noexcept -> ReadingDirection
	{
		return Get()->GetReadingDirection();
	}
	auto TextLayout::GetFlowDirection() const noexcept -> FlowDirection
	{
		return Get()->GetFlowDirection();
	}
	auto TextLayout::GetIncrementalTabStop() const noexcept -> float
	{
		return Get()->GetIncrementalTabStop();
	}
	auto TextLayout::GetLineSpacing() const noexcept -> LineSpacing
	{
		LineSpacing lineSpacing{ };
		auto hr = Get()->GetLineSpacing(&lineSpacing.method, 
			&lineSpacing.height, &lineSpacing.baseline); ThrowFailed(hr);

		return lineSpacing;
	}
	auto TextLayout::GetMaxWidth() const noexcept -> float
	{
		return Get()->GetMaxWidth();
	}
	auto TextLayout::GetMaxHeight() const noexcept -> float
	{
		return Get()->GetMaxHeight();
	}
	auto TextLayout::GetFontCollection() const noexcept -> FontCollection
	{
		FontCollection fontCollection{ nullptr };
		auto hr = Get()->GetFontCollection(
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>()); ThrowFailed(hr);

		return fontCollection;
	}
	auto TextLayout::GetFontCollection(UINT32 position) const noexcept -> FontCollection
	{
		FontCollection fontCollection{ nullptr };

		auto hr = Get()->GetFontCollection(position, 
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>()); ThrowFailed(hr);
		
		return fontCollection;
	}
	auto TextLayout::GetFontCollection(UINT32 position, TextRange& textRange) const noexcept -> FontCollection
	{
		FontCollection fontCollection{ nullptr };
		auto hr = Get()->GetFontCollection(position,
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>(),
			&textRange); ThrowFailed(hr);

		return fontCollection;
	}
	auto TextLayout::GetFontFamilyName() const noexcept -> std::wstring
	{
		auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		auto hr = Get()->GetFontFamilyName(fontFamilyName.data(), length); ThrowFailed(hr);

		return fontFamilyName;
	}
	auto TextLayout::GetFontFamilyName(UINT32 position) const noexcept -> std::wstring
	{
		auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		auto hr = Get()->GetFontFamilyName(position, fontFamilyName.data(), length); ThrowFailed(hr);

		return fontFamilyName;
	}
	auto TextLayout::GetFontFamilyName(UINT32 position, TextRange& textRange) const noexcept -> std::wstring
	{
		auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		auto hr = Get()->GetFontFamilyName(position, fontFamilyName.data(), length, &textRange); ThrowFailed(hr);

		return fontFamilyName;
	}
	auto TextLayout::GetFontWeight() const noexcept -> FontWeight
	{
		return Get()->GetFontWeight();
	}
	auto TextLayout::GetFontWeight(UINT32 position) const noexcept -> FontWeight
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		auto hr = Get()->GetFontWeight(position, &fontWeight); ThrowFailed(hr);

		return fontWeight;
	}
	auto TextLayout::GetFontWeight(UINT32 position, TextRange& textRange) const noexcept -> FontWeight
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		auto hr = Get()->GetFontWeight(position, &fontWeight, &textRange); ThrowFailed(hr);

		return fontWeight;
	}
	auto TextLayout::GetFontStyle() const noexcept -> FontStyle
	{
		return Get()->GetFontStyle();
	}
	auto TextLayout::GetFontStyle(UINT32 position) const noexcept -> FontStyle
	{
		DWRITE_FONT_STYLE fontStyle{ };
		auto hr = Get()->GetFontStyle(position, &fontStyle); ThrowFailed(hr);

		return fontStyle;
	}
	auto TextLayout::GetFontStyle(UINT32 position, TextRange& textRange) const noexcept -> FontStyle
	{
		DWRITE_FONT_STYLE fontStyle{ };
		auto hr = Get()->GetFontStyle(position, &fontStyle, &textRange); ThrowFailed(hr);

		return fontStyle;
	}
	auto TextLayout::GetFontStretch() const noexcept -> FontStretch
	{
		return Get()->GetFontStretch();
	}
	auto TextLayout::GetFontStretch(UINT32 position) const noexcept -> FontStretch
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		auto hr = Get()->GetFontStretch(position, &fontStretch); ThrowFailed(hr);

		return fontStretch;
	}
	auto TextLayout::GetFontStretch(UINT32 position, TextRange& textRange) const noexcept -> FontStretch
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		auto hr = Get()->GetFontStretch(position, &fontStretch, &textRange); ThrowFailed(hr);

		return fontStretch;
	}
	auto TextLayout::GetFontSize() const noexcept -> float
	{
		return Get()->GetFontSize();
	}
	auto TextLayout::GetFontSize(UINT32 position) const noexcept -> float
	{
		float fontSize{ };
		auto hr = Get()->GetFontSize(position, &fontSize); ThrowFailed(hr);

		return fontSize;
	}
	auto TextLayout::GetFontSize(UINT32 position, TextRange& textRange) const noexcept -> float
	{
		float fontSize{ };
		auto hr = Get()->GetFontSize(position, &fontSize, &textRange); ThrowFailed(hr);

		return fontSize;
	}
	auto TextLayout::GetUnderline(UINT32 position) const noexcept -> bool
	{
		BOOL underline{ };
		auto hr = Get()->GetUnderline(position, &underline); ThrowFailed(hr);

		return underline;
	}
	auto TextLayout::GetUnderline(UINT32 position, TextRange& textRange) const noexcept -> bool
	{
		BOOL underline{ };
		auto hr = Get()->GetUnderline(position, &underline, &textRange); ThrowFailed(hr);

		return underline;
	}
	auto TextLayout::GetStrikethrough(UINT32 position) const noexcept -> bool
	{
		BOOL strikethrough{ };
		auto hr = Get()->GetStrikethrough(position, &strikethrough); ThrowFailed(hr);

		return strikethrough;
	}
	auto TextLayout::GetStrikethrough(UINT32 position, TextRange& textRange) const noexcept -> bool
	{
		BOOL strikethrough{ };
		auto hr = Get()->GetStrikethrough(position, &strikethrough, &textRange); ThrowFailed(hr);

		return strikethrough;
	}
	auto TextLayout::GetDrawingEffect(UINT32 position) const noexcept -> ComPtr<IUnknown>
	{
		ComPtr<IUnknown> drawingEffect;
		auto hr = Get()->GetDrawingEffect(position, drawingEffect.GetAddressOf(), nullptr); ThrowFailed(hr);
		
		return drawingEffect;
	}
	auto TextLayout::GetDrawingEffect(UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IUnknown>
	{
		ComPtr<IUnknown> drawingEffect;
		auto hr = Get()->GetDrawingEffect(position, drawingEffect.GetAddressOf(), &textRange); ThrowFailed(hr);

		return drawingEffect;
	}
	auto TextLayout::GetInlineObject(UINT32 position) const noexcept -> ComPtr<IDWriteInlineObject>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		auto hr = Get()->GetInlineObject(position, inlineObject.GetAddressOf(), nullptr); ThrowFailed(hr);

		return inlineObject;
	}
	auto TextLayout::GetInlineObject(UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IDWriteInlineObject>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		auto hr = Get()->GetInlineObject(position, inlineObject.GetAddressOf(), &textRange); ThrowFailed(hr);

		return inlineObject;
	}
	auto TextLayout::GetTypography(UINT32 position) const noexcept -> ComPtr<IDWriteTypography>
	{
		ComPtr<IDWriteTypography> typography;
		auto hr = Get()->GetTypography(position, typography.GetAddressOf(), nullptr); ThrowFailed(hr);

		return typography;
	}
	auto TextLayout::GetTypography(UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IDWriteTypography>
	{
		ComPtr<IDWriteTypography> typography;
		auto hr = Get()->GetTypography(position, typography.GetAddressOf(), &textRange); ThrowFailed(hr);

		return typography;
	}
	auto TextLayout::GetLocaleName() const noexcept -> std::wstring
	{
		auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');
		auto hr = Get()->GetLocaleName(localeName.data(), length); ThrowFailed(hr);

		return localeName;
	}
	auto TextLayout::GetLocaleName(UINT32 position) const noexcept -> std::wstring
	{
		auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');

		auto hr = Get()->GetLocaleName(position, localeName.data(), length); ThrowFailed(hr);

		return localeName;
	}
	auto TextLayout::GetLocaleName(UINT32 position, TextRange& textRange) const noexcept -> std::wstring
	{
		auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');
		auto hr = Get()->GetLocaleName(position, localeName.data(), length, &textRange); ThrowFailed(hr);

		return localeName;
	}
	auto TextLayout::DetermineMinWidth() const noexcept -> float
	{
		FLOAT minWidth = 0.0F;
		auto hr = Get()->DetermineMinWidth(&minWidth); ThrowFailed(hr);

		return minWidth;
	}
}

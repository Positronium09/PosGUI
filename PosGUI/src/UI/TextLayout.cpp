module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.TextLayout;

import std;

import PGUI.UI.TextFormat;
import PGUI.ComPtr;
import PGUI.Logging;
import PGUI.Exceptions;
import PGUI.Factories;

namespace PGUI::UI
{
	using namespace Font;

	TextLayout::TextLayout(const ComPtr<IDWriteTextLayout4>& textLayout) noexcept :
		ComPtrHolder{ textLayout }
	{ }

	TextLayout::TextLayout(const std::wstring_view text, const TextFormat& textFormat, const SizeF maxSize) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();
		const auto hr = factory->CreateTextLayout(
			text.data(), static_cast<UINT32>(text.size()),
			textFormat.GetRawAs<IDWriteTextFormat3, IDWriteTextFormat>(),
			maxSize.cx, maxSize.cy,
			std::bit_cast<IDWriteTextLayout**>(GetAddress()));
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetTextAlignment(const TextAlignment textAlignment) const noexcept -> void
	{
		const auto hr = Get()->SetTextAlignment(textAlignment);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetParagraphAlignment(const ParagraphAlignment paragraphAlignment) const noexcept -> void
	{
		const auto hr = Get()->SetParagraphAlignment(paragraphAlignment);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetWordWrapping(const WordWrapping wordWrapping) const noexcept -> void
	{
		const auto hr = Get()->SetWordWrapping(wordWrapping);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetReadingDirection(const ReadingDirection readingDirection) const noexcept -> void
	{
		const auto hr = Get()->SetReadingDirection(readingDirection);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetFlowDirection(const FlowDirection flowDirection) const noexcept -> void
	{
		const auto hr = Get()->SetFlowDirection(flowDirection);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetIncrementalTabStop(const float incrementalTabStop) const noexcept -> void
	{
		const auto hr = Get()->SetIncrementalTabStop(incrementalTabStop);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetLineSpacing(const LineSpacing& lineSpacing) const noexcept -> void
	{
		const auto hr = Get()->SetLineSpacing(
			lineSpacing.method, lineSpacing.height, lineSpacing.baseline);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetMaxWidth(const float maxWidth) const noexcept -> void
	{
		const auto hr = Get()->SetMaxWidth(maxWidth);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetMaxHeight(const float maxHeight) const noexcept -> void
	{
		const auto hr = Get()->SetMaxHeight(maxHeight);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetMaxSize(const SizeF maxSize) const noexcept -> void
	{
		SetMaxWidth(maxSize.cx);
		SetMaxHeight(maxSize.cy);
	}

	auto TextLayout::SetFontCollection(const FontCollection& fontCollection,
	                                   const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetFontCollection(
			fontCollection.GetRawAs<IDWriteFontCollection3, IDWriteFontCollection>(),
			textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetFontFamilyName(const std::wstring_view fontFamilyName,
	                                   const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetFontFamilyName(fontFamilyName.data(), textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetFontWeight(const FontWeight fontWeight, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetFontWeight(fontWeight, textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetFontStyle(const FontStyle fontStyle, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetFontStyle(fontStyle, textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetFontStretch(const FontStretch fontStretch, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetFontStretch(fontStretch, textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetFontSize(const float fontSize, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetFontSize(fontSize, textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetUnderline(const bool hasUnderline, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetUnderline(hasUnderline, textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetStrikethrough(const bool hasStrikethrough, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetStrikethrough(hasStrikethrough, textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetDrawingEffect(const ComPtr<IUnknown>& drawingEffect,
	                                  const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetDrawingEffect(drawingEffect.Get(), textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetInlineObject(const ComPtr<IDWriteInlineObject>& inlineObject,
	                                 const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetInlineObject(inlineObject.Get(), textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetTypography(const ComPtr<IDWriteTypography>& typography,
	                               const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetTypography(typography.Get(), textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetLocaleName(const std::wstring_view localeName, const TextRange textRange) const noexcept -> void
	{
		const auto hr = Get()->SetLocaleName(localeName.data(), textRange);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::SetTrimming(const Trimming& trimming) const noexcept -> void
	{
		auto& trimmingOptions = trimming.trimmingOptions;
		const auto hr = Get()->SetTrimming(&trimmingOptions, trimming.GetRaw());
		LogFailed(LogLevel::Error, hr);
	}

	auto TextLayout::GetTrimming() const noexcept -> Trimming
	{
		Trimming trimming{ };
		const auto hr = Get()->GetTrimming(
			&trimming.trimmingOptions,
			trimming.GetAddress());
		LogFailed(LogLevel::Error, hr);

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
		const auto hr = Get()->GetLineSpacing(
			&lineSpacing.method,
			&lineSpacing.height, &lineSpacing.baseline);
		LogFailed(LogLevel::Error, hr);

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
		const auto hr = Get()->GetFontCollection(
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>());
		LogFailed(LogLevel::Error, hr);

		return fontCollection;
	}

	auto TextLayout::GetFontCollection(const UINT32 position) const noexcept -> FontCollection
	{
		FontCollection fontCollection{ nullptr };

		const auto hr = Get()->GetFontCollection(
			position, fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>());
		LogFailed(LogLevel::Error, hr);

		return fontCollection;
	}

	auto TextLayout::GetFontCollection(const UINT32 position, TextRange& textRange) const noexcept -> FontCollection
	{
		FontCollection fontCollection{ nullptr };
		const auto hr = Get()->GetFontCollection(
			position,
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>(),
			&textRange);
		LogFailed(LogLevel::Error, hr);

		return fontCollection;
	}

	auto TextLayout::GetFontFamilyName() const noexcept -> std::wstring
	{
		const auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		const auto hr = Get()->GetFontFamilyName(fontFamilyName.data(), length);
		LogFailed(LogLevel::Error, hr);

		return fontFamilyName;
	}

	auto TextLayout::GetFontFamilyName(const UINT32 position) const noexcept -> std::wstring
	{
		const auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		const auto hr = Get()->GetFontFamilyName(position, fontFamilyName.data(), length);
		LogFailed(LogLevel::Error, hr);

		return fontFamilyName;
	}

	auto TextLayout::GetFontFamilyName(const UINT32 position, TextRange& textRange) const noexcept -> std::wstring
	{
		const auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		const auto hr = Get()->GetFontFamilyName(position, fontFamilyName.data(), length, &textRange);
		LogFailed(LogLevel::Error, hr);

		return fontFamilyName;
	}

	auto TextLayout::GetFontWeight() const noexcept -> FontWeight
	{
		return Get()->GetFontWeight();
	}

	auto TextLayout::GetFontWeight(const UINT32 position) const noexcept -> FontWeight
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		const auto hr = Get()->GetFontWeight(position, &fontWeight);
		LogFailed(LogLevel::Error, hr);

		return fontWeight;
	}

	auto TextLayout::GetFontWeight(const UINT32 position, TextRange& textRange) const noexcept -> FontWeight
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		const auto hr = Get()->GetFontWeight(position, &fontWeight, &textRange);
		LogFailed(LogLevel::Error, hr);

		return fontWeight;
	}

	auto TextLayout::GetFontStyle() const noexcept -> FontStyle
	{
		return Get()->GetFontStyle();
	}

	auto TextLayout::GetFontStyle(const UINT32 position) const noexcept -> FontStyle
	{
		DWRITE_FONT_STYLE fontStyle{ };
		const auto hr = Get()->GetFontStyle(position, &fontStyle);
		LogFailed(LogLevel::Error, hr);

		return fontStyle;
	}

	auto TextLayout::GetFontStyle(const UINT32 position, TextRange& textRange) const noexcept -> FontStyle
	{
		DWRITE_FONT_STYLE fontStyle{ };
		const auto hr = Get()->GetFontStyle(position, &fontStyle, &textRange);
		LogFailed(LogLevel::Error, hr);

		return fontStyle;
	}

	auto TextLayout::GetFontStretch() const noexcept -> FontStretch
	{
		return Get()->GetFontStretch();
	}

	auto TextLayout::GetFontStretch(const UINT32 position) const noexcept -> FontStretch
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		const auto hr = Get()->GetFontStretch(position, &fontStretch);
		LogFailed(LogLevel::Error, hr);

		return fontStretch;
	}

	auto TextLayout::GetFontStretch(const UINT32 position, TextRange& textRange) const noexcept -> FontStretch
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		const auto hr = Get()->GetFontStretch(position, &fontStretch, &textRange);
		LogFailed(LogLevel::Error, hr);

		return fontStretch;
	}

	auto TextLayout::GetFontSize() const noexcept -> float
	{
		return Get()->GetFontSize();
	}

	auto TextLayout::GetFontSize(const UINT32 position) const noexcept -> float
	{
		float fontSize{ };
		const auto hr = Get()->GetFontSize(position, &fontSize);
		LogFailed(LogLevel::Error, hr);

		return fontSize;
	}

	auto TextLayout::GetFontSize(const UINT32 position, TextRange& textRange) const noexcept -> float
	{
		float fontSize{ };
		const auto hr = Get()->GetFontSize(position, &fontSize, &textRange);
		LogFailed(LogLevel::Error, hr);

		return fontSize;
	}

	auto TextLayout::GetUnderline(const UINT32 position) const noexcept -> bool
	{
		BOOL underline{ };
		const auto hr = Get()->GetUnderline(position, &underline);
		LogFailed(LogLevel::Error, hr);

		return underline;
	}

	auto TextLayout::GetUnderline(const UINT32 position, TextRange& textRange) const noexcept -> bool
	{
		BOOL underline{ };
		const auto hr = Get()->GetUnderline(position, &underline, &textRange);
		LogFailed(LogLevel::Error, hr);

		return underline;
	}

	auto TextLayout::GetStrikethrough(const UINT32 position) const noexcept -> bool
	{
		BOOL strikethrough{ };
		const auto hr = Get()->GetStrikethrough(position, &strikethrough);
		LogFailed(LogLevel::Error, hr);

		return strikethrough;
	}

	auto TextLayout::GetStrikethrough(const UINT32 position, TextRange& textRange) const noexcept -> bool
	{
		BOOL strikethrough{ };
		const auto hr = Get()->GetStrikethrough(position, &strikethrough, &textRange);
		LogFailed(LogLevel::Error, hr);

		return strikethrough;
	}

	auto TextLayout::GetDrawingEffect(const UINT32 position) const noexcept -> ComPtr<IUnknown>
	{
		ComPtr<IUnknown> drawingEffect;
		const auto hr = Get()->GetDrawingEffect(position, drawingEffect.GetAddressOf(), nullptr);
		LogFailed(LogLevel::Error, hr);

		return drawingEffect;
	}

	auto TextLayout::GetDrawingEffect(const UINT32 position, TextRange& textRange) const noexcept -> ComPtr<IUnknown>
	{
		ComPtr<IUnknown> drawingEffect;
		const auto hr = Get()->GetDrawingEffect(position, drawingEffect.GetAddressOf(), &textRange);
		LogFailed(LogLevel::Error, hr);

		return drawingEffect;
	}

	auto TextLayout::GetInlineObject(const UINT32 position) const noexcept -> ComPtr<IDWriteInlineObject>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		const auto hr = Get()->GetInlineObject(position, inlineObject.GetAddressOf(), nullptr);
		LogFailed(LogLevel::Error, hr);

		return inlineObject;
	}

	auto TextLayout::GetInlineObject(
		const UINT32 position,
		TextRange& textRange) const noexcept -> ComPtr<IDWriteInlineObject>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		const auto hr = Get()->GetInlineObject(position, inlineObject.GetAddressOf(), &textRange);
		LogFailed(LogLevel::Error, hr);

		return inlineObject;
	}

	auto TextLayout::GetTypography(const UINT32 position) const noexcept -> ComPtr<IDWriteTypography>
	{
		ComPtr<IDWriteTypography> typography;
		const auto hr = Get()->GetTypography(position, typography.GetAddressOf(), nullptr);
		LogFailed(LogLevel::Error, hr);

		return typography;
	}

	auto TextLayout::GetTypography(const UINT32 position,
	                               TextRange& textRange) const noexcept -> ComPtr<IDWriteTypography>
	{
		ComPtr<IDWriteTypography> typography;
		const auto hr = Get()->GetTypography(position, typography.GetAddressOf(), &textRange);
		LogFailed(LogLevel::Error, hr);

		return typography;
	}

	auto TextLayout::GetLocaleName() const noexcept -> std::wstring
	{
		const auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');
		const auto hr = Get()->GetLocaleName(localeName.data(), length);
		LogFailed(LogLevel::Error, hr);

		return localeName;
	}

	auto TextLayout::GetLocaleName(const UINT32 position) const noexcept -> std::wstring
	{
		const auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');

		const auto hr = Get()->GetLocaleName(position, localeName.data(), length);
		LogFailed(LogLevel::Error, hr);

		return localeName;
	}

	auto TextLayout::GetLocaleName(const UINT32 position, TextRange& textRange) const noexcept -> std::wstring
	{
		const auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');
		const auto hr = Get()->GetLocaleName(position, localeName.data(), length, &textRange);
		LogFailed(LogLevel::Error, hr);

		return localeName;
	}

	auto TextLayout::DetermineMinWidth() const noexcept -> float
	{
		auto minWidth = 0.0F;
		const auto hr = Get()->DetermineMinWidth(&minWidth);
		LogFailed(LogLevel::Error, hr);

		return minWidth;
	}
}

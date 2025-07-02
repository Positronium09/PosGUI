module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.TextFormat;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.UI.Font.FontCollection;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;
import PGUI.Exceptions;

namespace PGUI::UI
{
	using namespace Font;

	TextFormat::TextFormat(ComPtr<IDWriteTextFormat3> textFormat) noexcept : 
		ComPtrHolder{ textFormat }
	{
	}
	
	TextFormat::TextFormat(std::wstring_view fontFamilyName, float fontSize, 
		FontWeight fontWeight, FontStyle fontStyle, FontStretch fontStretch, 
		FontCollection fontCollection,
		std::wstring_view localeName) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		auto fontSet = fontCollection.GetFontSet();
		auto fontAxisValues = 
			fontSet.ConvertWeightStretchStyleToFontAxisValues(fontWeight, fontStretch, fontStyle, fontSize);

		auto hr = factory->CreateTextFormat(fontFamilyName.data(),
			fontCollection.GetRawAs<IDWriteFontCollection>(),
			fontAxisValues.data(), static_cast<UINT32>(fontAxisValues.size()),
			fontSize, localeName.data(), GetAddress()); ThrowFailed(hr);
	}

	TextFormat::TextFormat(std::wstring_view fontFamilyName, float fontSize, 
		std::span<const FontAxisValue> fontAxisValues, 
		FontCollection fontCollection,
		std::wstring_view localeName) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		auto hr = factory->CreateTextFormat(fontFamilyName.data(),
			fontCollection.GetRawAs<IDWriteFontCollection>(),
			fontAxisValues.data(), static_cast<UINT32>(fontAxisValues.size()),
			fontSize, localeName.data(), GetAddress()); ThrowFailed(hr);
	}

	auto TextFormat::GetFlowDirection() const noexcept -> FlowDirection
	{
		return Get()->GetFlowDirection();
	}
	auto TextFormat::GetFontCollection() const noexcept -> FontCollection
	{
		ComPtr<IDWriteFontCollection> fontCollection;
		ComPtr<IDWriteFontCollection3> fontCollection3;
		auto hr = Get()->GetFontCollection(fontCollection.GetAddressOf()); ThrowFailed(hr);

		hr = fontCollection.As(&fontCollection3);

		return fontCollection3;
	}
	auto TextFormat::GetFontFamilyName() const noexcept -> std::wstring
	{
		auto& ptr = Get();

		auto length = ptr->GetFontFamilyNameLength();

		std::wstring fontFamilyName(length, L'\0');
		auto hr = ptr->GetFontFamilyName(fontFamilyName.data(), length + 1); ThrowFailed(hr);

		return fontFamilyName;
	}
	auto TextFormat::GetFontSize() const noexcept -> float
	{
		return Get()->GetFontSize();
	}
	auto TextFormat::GetFontStretch() const noexcept -> FontStretch
	{
		return Get()->GetFontStretch();
	}
	auto TextFormat::GetFontStyle() const noexcept -> FontStyle
	{
		return Get()->GetFontStyle();
	}
	auto TextFormat::GetFontWeight() const noexcept -> FontWeight
	{
		return Get()->GetFontWeight();
	}
	auto TextFormat::GetIncrementalTabStop() const noexcept -> float
	{
		return Get()->GetIncrementalTabStop();
	}
	auto TextFormat::GetLocaleName() const noexcept -> std::wstring
	{
		auto& ptr = Get();

		auto length = ptr->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');

		auto hr = ptr->GetLocaleName(localeName.data(), length + 1); ThrowFailed(hr);

		return localeName;
	}
	auto TextFormat::GetParagraphAlignment() const noexcept -> ParagraphAlignment
	{
		return Get()->GetParagraphAlignment();
	}
	auto TextFormat::GetReadingDirection() const noexcept -> ReadingDirection
	{
		return Get()->GetReadingDirection();
	}
	auto TextFormat::GetTextAlignment() const noexcept -> TextAlignment
	{
		return Get()->GetTextAlignment();
	}
	auto TextFormat::GetWordWrapping() const noexcept -> WordWrapping
	{
		return Get()->GetWordWrapping();
	}
	auto TextFormat::GetTrimming() const noexcept -> Trimming
	{
		Trimming trimming{ };
		auto hr = Get()->GetTrimming(&trimming.trimmingOptions, trimming.GetAddress()); ThrowFailed(hr);
		
		return trimming;
	}
	auto TextFormat::GetLineSpacing() const noexcept -> LineSpacing
	{
		LineSpacing lineSpacing{ };
		auto hr = Get()->GetLineSpacing(&lineSpacing); ThrowFailed(hr);

		return lineSpacing;
	}

	void TextFormat::SetFlowDirection(FlowDirection flowDirection) noexcept
	{
		auto hr = Get()->SetFlowDirection(flowDirection); ThrowFailed(hr);
	}
	void TextFormat::SetIncrementalTabStop(float incrementalTabStop) noexcept
	{
		auto hr = Get()->SetIncrementalTabStop(incrementalTabStop); ThrowFailed(hr);
	}
	void TextFormat::SetParagraphAlignment(ParagraphAlignment paragraphAlignment) noexcept
	{
		auto hr = Get()->SetParagraphAlignment(paragraphAlignment); ThrowFailed(hr);
	}
	void TextFormat::SetReadingDirection(ReadingDirection readingDirection) noexcept
	{
		auto hr = Get()->SetReadingDirection(readingDirection); ThrowFailed(hr);
	}
	void TextFormat::SetTextAlignment(TextAlignment textAlignment) noexcept
	{
		auto hr = Get()->SetTextAlignment(textAlignment); ThrowFailed(hr);
	}
	void TextFormat::SetWordWrapping(WordWrapping wordWrapping) noexcept
	{
		auto hr = Get()->SetWordWrapping(wordWrapping); ThrowFailed(hr);
	}
	void TextFormat::SetTrimming(const Trimming& trimming) noexcept
	{
		auto hr = Get()->SetTrimming(&trimming.trimmingOptions, trimming.GetRaw()); ThrowFailed(hr);
	}
	void TextFormat::SetLineSpacing(LineSpacing lineSpacing) noexcept
	{
		auto hr = Get()->SetLineSpacing(&lineSpacing); ThrowFailed(hr);
	}
}

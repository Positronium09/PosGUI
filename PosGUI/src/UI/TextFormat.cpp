module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.TextFormat;

import std;

import PGUI.ComPtr;
import PGUI.Logging;
import PGUI.Factories;
import PGUI.UI.Font.FontCollection;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;
import PGUI.Exceptions;

namespace PGUI::UI
{
	using namespace Font;

	TextFormat::TextFormat(const ComPtr<IDWriteTextFormat3>& textFormat) noexcept :
		ComPtrHolder{ textFormat }
	{ }

	TextFormat::TextFormat(
		const std::wstring_view fontFamilyName, const float fontSize,
		const FontWeight fontWeight, const FontStyle fontStyle, const FontStretch fontStretch,
		const FontCollection& fontCollection,
		const std::wstring_view localeName) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		const auto fontSet = fontCollection.GetFontSet();
		const auto fontAxisValues =
			fontSet.ConvertWeightStretchStyleToFontAxisValues(fontWeight, fontStretch, fontStyle, fontSize);

		const auto hr = factory->CreateTextFormat(
			fontFamilyName.data(),
			fontCollection.GetRawAs<IDWriteFontCollection>(),
			fontAxisValues.data(), static_cast<UINT32>(fontAxisValues.size()),
			fontSize, localeName.data(), GetAddress());
		LogFailed(LogLevel::Error, hr);
	}

	TextFormat::TextFormat(
		const std::wstring_view fontFamilyName, const float fontSize,
		const std::span<const FontAxisValue> fontAxisValues,
		const FontCollection& fontCollection,
		const std::wstring_view localeName) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		const auto hr = factory->CreateTextFormat(
			fontFamilyName.data(),
			fontCollection.GetRawAs<IDWriteFontCollection>(),
			fontAxisValues.data(), static_cast<UINT32>(fontAxisValues.size()),
			fontSize, localeName.data(), GetAddress());
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::GetFlowDirection() const noexcept -> FlowDirection
	{
		return Get()->GetFlowDirection();
	}

	auto TextFormat::GetFontCollection() const noexcept -> FontCollection
	{
		ComPtr<IDWriteFontCollection> fontCollection;
		ComPtr<IDWriteFontCollection3> fontCollection3;
		auto hr = Get()->GetFontCollection(fontCollection.GetAddressOf());
		LogFailed(LogLevel::Error, hr);

		if (FAILED(hr))
		{
			return FontCollection{ nullptr };
		}

		hr = fontCollection.As(&fontCollection3);
		LogFailed(LogLevel::Error, hr);

		return fontCollection3;
	}

	auto TextFormat::GetFontFamilyName() const noexcept -> std::wstring
	{
		auto& ptr = Get();

		const auto length = ptr->GetFontFamilyNameLength();

		std::wstring fontFamilyName(length, L'\0');
		const auto hr = ptr->GetFontFamilyName(fontFamilyName.data(), length + 1);
		LogFailed(LogLevel::Error, hr);

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

		const auto length = ptr->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');

		const auto hr = ptr->GetLocaleName(localeName.data(), length + 1);
		LogFailed(LogLevel::Error, hr);

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
		const auto hr = Get()->GetTrimming(&trimming.trimmingOptions, trimming.GetAddress());
		LogFailed(LogLevel::Error, hr);

		return trimming;
	}

	auto TextFormat::GetLineSpacing() const noexcept -> LineSpacing
	{
		LineSpacing lineSpacing{ };
		const auto hr = Get()->GetLineSpacing(&lineSpacing);
		LogFailed(LogLevel::Error, hr);

		return lineSpacing;
	}

	auto TextFormat::SetFlowDirection(const FlowDirection flowDirection) noexcept -> void
	{
		const auto hr = Get()->SetFlowDirection(flowDirection);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetIncrementalTabStop(const float incrementalTabStop) noexcept -> void
	{
		const auto hr = Get()->SetIncrementalTabStop(incrementalTabStop);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetParagraphAlignment(const ParagraphAlignment paragraphAlignment) noexcept -> void
	{
		const auto hr = Get()->SetParagraphAlignment(paragraphAlignment);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetReadingDirection(const ReadingDirection readingDirection) noexcept -> void
	{
		const auto hr = Get()->SetReadingDirection(readingDirection);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetTextAlignment(const TextAlignment textAlignment) noexcept -> void
	{
		const auto hr = Get()->SetTextAlignment(textAlignment);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetWordWrapping(const WordWrapping wordWrapping) noexcept -> void
	{
		const auto hr = Get()->SetWordWrapping(wordWrapping);
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetTrimming(const Trimming& trimming) noexcept -> void
	{
		const auto hr = Get()->SetTrimming(&trimming.trimmingOptions, trimming.GetRaw());
		LogFailed(LogLevel::Error, hr);
	}

	auto TextFormat::SetLineSpacing(const LineSpacing& lineSpacing) noexcept -> void
	{
		const auto hr = Get()->SetLineSpacing(&lineSpacing);
		LogFailed(LogLevel::Error, hr);
	}
}

module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.TextFormat;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Factories;
import PGUI.UI.Font.FontCollection;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.Font.FontStructs;

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

		FontCollection collection = fontCollection;

		if (fontCollection.IsInitialized())
		{
			auto fontCollectionResult = FontCollection::GetSystemFontCollection();
			if (!fontCollectionResult.has_value())
			{
				LogIfFailed(Error{ fontCollectionResult.error() });
				return;
			}
			collection = fontCollectionResult.value();
		}

		const auto fontSetResult = collection.GetFontSet();
		if (!fontSetResult.has_value())
		{
			LogIfFailed(Error{ fontSetResult.error() });
			return;
		}
		const auto& fontSet = fontSetResult.value();
		const auto fontAxisValues =
			fontSet.ConvertWeightStretchStyleToFontAxisValues(fontWeight, fontStretch, fontStyle, fontSize);

		const auto hr = factory->CreateTextFormat(
			fontFamilyName.data(),
			collection.GetRawAs<IDWriteFontCollection>(),
			fontAxisValues.data(), static_cast<UINT32>(fontAxisValues.size()),
			fontSize, localeName.data(), GetAddress());
		LogIfFailed(
			Error{ hr }
			.AddDetail(L"FontFamilyName", fontFamilyName)
			.AddDetail(L"FontSize", std::to_wstring(fontSize))
			.AddDetail(L"FontWeight", std::to_wstring(fontWeight))
			.AddDetail(L"FontStyle", std::to_wstring(fontStyle))
			.AddDetail(L"FontStretch", std::to_wstring(fontStretch))
			.AddDetail(L"FontCollection", std::format(L"{:p}", static_cast<void*>(fontCollection.GetRaw())))
			.AddDetail(L"LocaleName", localeName)
		);
	}

	TextFormat::TextFormat(
		const std::wstring_view fontFamilyName, const float fontSize,
		const std::span<const FontAxisValue> fontAxisValues,
		const FontCollection& fontCollection,
		const std::wstring_view localeName) noexcept
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		FontCollection collection = fontCollection;

		if (fontCollection.IsInitialized())
		{
			auto fontCollectionResult = FontCollection::GetSystemFontCollection();
			if (!fontCollectionResult.has_value())
			{
				LogIfFailed(Error{ fontCollectionResult.error() });
				return;
			}
			collection = fontCollectionResult.value();
		}

		const auto hr = factory->CreateTextFormat(
			fontFamilyName.data(),
			collection.GetRawAs<IDWriteFontCollection>(),
			fontAxisValues.data(), static_cast<UINT32>(fontAxisValues.size()),
			fontSize, localeName.data(), GetAddress());

		LogIfFailed(
			Error{ hr }
			.AddDetail(L"FontFamilyName", fontFamilyName)
			.AddDetail(L"FontSize", std::to_wstring(fontSize))
			.AddDetail(L"FontCollection", std::format(L"{:p}", static_cast<void*>(fontCollection.GetRaw())))
			.AddDetail(L"LocaleName", localeName)
		);
	}

	auto TextFormat::SetFlowDirection(const FlowDirection flowDirection) noexcept -> Error
	{
		return Error{
			Get()->SetFlowDirection(flowDirection)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetIncrementalTabStop(const float incrementalTabStop) noexcept -> Error
	{
		return Error{
			Get()->SetIncrementalTabStop(incrementalTabStop)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetParagraphAlignment(const ParagraphAlignment paragraphAlignment) noexcept -> Error
	{
		return Error{
			Get()->SetParagraphAlignment(paragraphAlignment)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetReadingDirection(const ReadingDirection readingDirection) noexcept -> Error
	{
		return Error{
			Get()->SetReadingDirection(readingDirection)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetTextAlignment(const TextAlignment textAlignment) noexcept -> Error
	{
		return Error{
			Get()->SetTextAlignment(textAlignment)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetWordWrapping(const WordWrapping wordWrapping) noexcept -> Error
	{
		return Error{
			Get()->SetWordWrapping(wordWrapping)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetTrimming(const Trimming& trimming) noexcept -> Error
	{
		return Error{
			Get()->SetTrimming(&trimming.trimmingOptions, trimming.GetRaw())
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::SetLineSpacing(const LineSpacing& lineSpacing) noexcept -> Error
	{
		return Error{
			Get()->SetLineSpacing(&lineSpacing)
		}.AddTag(ErrorTags::Font);
	}

	auto TextFormat::GetFlowDirection() const noexcept -> FlowDirection
	{
		return Get()->GetFlowDirection();
	}

	auto TextFormat::GetFontCollection() const noexcept -> Result<FontCollection>
	{
		ComPtr<IDWriteFontCollection> fontCollection;
		ComPtr<IDWriteFontCollection3> fontCollection3;

		if (const auto hr = Get()->GetFontCollection(fontCollection.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		if (const auto hr = fontCollection.As(&fontCollection3);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return fontCollection3;
	}

	auto TextFormat::GetFontFamilyName() const noexcept -> Result<std::wstring>
	{
		auto& ptr = Get();

		const auto length = ptr->GetFontFamilyNameLength();

		std::wstring fontFamilyName(length, L'\0');
		if (const auto hr = ptr->GetFontFamilyName(fontFamilyName.data(), length + 1);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

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

	auto TextFormat::GetLocaleName() const noexcept -> Result<std::wstring>
	{
		auto& ptr = Get();

		const auto length = ptr->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');

		if (const auto hr = ptr->GetLocaleName(localeName.data(), length + 1);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

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

	auto TextFormat::GetTrimming() const noexcept -> Result<Trimming>
	{
		Trimming trimming{ };
		if (const auto hr = Get()->GetTrimming(&trimming.trimmingOptions, trimming.GetAddress());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return trimming;
	}

	auto TextFormat::GetLineSpacing() const noexcept -> Result<LineSpacing>
	{
		LineSpacing lineSpacing{ };
		if (const auto hr = Get()->GetLineSpacing(&lineSpacing);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return lineSpacing;
	}
}

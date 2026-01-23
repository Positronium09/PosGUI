module;
#include <dwrite_3.h>

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
			fontSize, localeName.data(), Put());
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
			fontSize, localeName.data(), Put());

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
		Error error{
			Get()->SetFlowDirection(flowDirection)
		};
		LogIfFailed(error, L"Cannot set flow direction");
		return error;
	}

	auto TextFormat::SetIncrementalTabStop(const float incrementalTabStop) noexcept -> Error
	{
		Error error{
			Get()->SetIncrementalTabStop(incrementalTabStop)
		};
		LogIfFailed(error, L"Cannot set incremental tab stop");
		return error;
	}

	auto TextFormat::SetParagraphAlignment(const ParagraphAlignment paragraphAlignment) noexcept -> Error
	{
		Error error{
			Get()->SetParagraphAlignment(paragraphAlignment)
		};
		LogIfFailed(error, L"Cannot set paragraph alignment");
		return error;
	}

	auto TextFormat::SetReadingDirection(const ReadingDirection readingDirection) noexcept -> Error
	{
		Error error{
			Get()->SetReadingDirection(readingDirection)
		};
		LogIfFailed(error, L"Cannot set reading direction");
		return error;
	}

	auto TextFormat::SetTextAlignment(const TextAlignment textAlignment) noexcept -> Error
	{
		Error error{
			Get()->SetTextAlignment(textAlignment)
		};
		LogIfFailed(error, L"Cannot set text alignment");
		return error;
	}

	auto TextFormat::SetWordWrapping(const WordWrapping wordWrapping) noexcept -> Error
	{
		Error error{
			Get()->SetWordWrapping(wordWrapping)
		};
		LogIfFailed(error, L"Cannot set word wrapping");
		return error;
	}

	auto TextFormat::SetTrimming(const Trimming& trimming) noexcept -> Error
	{
		Error error{
			Get()->SetTrimming(&trimming.trimmingOptions, trimming.GetRaw())
		};
		LogIfFailed(error, L"Cannot set trimming");
		return error;
	}

	auto TextFormat::SetLineSpacing(const LineSpacing& lineSpacing) noexcept -> Error
	{
		Error error{
			Get()->SetLineSpacing(&lineSpacing)
		};
		LogIfFailed(error, L"Cannot set line spacing");
		return error;
	}

	auto TextFormat::GetFlowDirection() const noexcept -> FlowDirection
	{
		return Get()->GetFlowDirection();
	}

	auto TextFormat::GetFontCollection() const noexcept -> Result<FontCollection>
	{
		ComPtr<IDWriteFontCollection> fontCollection;

		if (const auto hr = Get()->GetFontCollection(fontCollection.put());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get font collection");
			return Unexpected{ error };
		}

		auto fontCollection3 = fontCollection.try_query<IDWriteFontCollection3>();
		if (fontCollection3.get() == nullptr)
		{
			Error error{ E_NOINTERFACE };
			Logger::Error(error, L"Failed to cast font collection to IDWriteFontCollection3");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Failed to get font family name");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Failed to get locale name");
			return Unexpected{ error };
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
		if (const auto hr = Get()->GetTrimming(&trimming.trimmingOptions, trimming.Put());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get trimming");
			return Unexpected{ error };
		}

		return trimming;
	}

	auto TextFormat::GetLineSpacing() const noexcept -> Result<LineSpacing>
	{
		LineSpacing lineSpacing{ };
		if (const auto hr = Get()->GetLineSpacing(&lineSpacing);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get line spacing");
			return Unexpected{ error };
		}

		return lineSpacing;
	}
}

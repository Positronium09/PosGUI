module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.TextLayout;

import std;

import PGUI.UI.TextFormat;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
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

		LogIfFailed(
			Error{
				hr
			}
			.AddTag(ErrorTags::Initialization)
			.AddDetail(L"Text", text)
			.AddDetail(L"MaxSize", std::format(L"{}", maxSize))
		);
	}

	auto TextLayout::SetTextAlignment(const TextAlignment textAlignment) const noexcept -> Error
	{
		return Error{
			Get()->SetTextAlignment(textAlignment)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetParagraphAlignment(const ParagraphAlignment paragraphAlignment) const noexcept -> Error
	{
		return Error{
			Get()->SetParagraphAlignment(paragraphAlignment)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetWordWrapping(const WordWrapping wordWrapping) const noexcept -> Error
	{
		return Error{
			Get()->SetWordWrapping(wordWrapping)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetReadingDirection(const ReadingDirection readingDirection) const noexcept -> Error
	{
		return Error{
			Get()->SetReadingDirection(readingDirection)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetFlowDirection(const FlowDirection flowDirection) const noexcept -> Error
	{
		return Error{
			Get()->SetFlowDirection(flowDirection)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetIncrementalTabStop(const float incrementalTabStop) const noexcept -> Error
	{
		return Error{
			Get()->SetIncrementalTabStop(incrementalTabStop)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetLineSpacing(const LineSpacing& lineSpacing) const noexcept -> Error
	{
		return Error{
			Get()->SetLineSpacing(
				lineSpacing.method, lineSpacing.height, lineSpacing.baseline)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetMaxWidth(const float maxWidth) const noexcept -> Error
	{
		return Error{
			Get()->SetMaxWidth(maxWidth)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetMaxHeight(const float maxHeight) const noexcept -> Error
	{
		return Error{
			Get()->SetMaxHeight(maxHeight)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetMaxSize(const SizeF maxSize) const noexcept -> Error
	{
		if (const auto error = SetMaxWidth(maxSize.cx);
			error.IsFailure())
		{
			return error;
		}
		return SetMaxHeight(maxSize.cy);
	}

	auto TextLayout::SetFontCollection(const FontCollection& fontCollection,
	                                   const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetFontCollection(
				fontCollection.GetRawAs<IDWriteFontCollection3, IDWriteFontCollection>(),
				textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetFontFamilyName(const std::wstring_view fontFamilyName,
	                                   const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetFontFamilyName(fontFamilyName.data(), textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetFontWeight(const FontWeight fontWeight, const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetFontWeight(fontWeight, textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetFontStyle(const FontStyle fontStyle, const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetFontStyle(fontStyle, textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetFontStretch(const FontStretch fontStretch, const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetFontStretch(fontStretch, textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetFontSize(const float fontSize, const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetFontSize(fontSize, textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetUnderline(const bool hasUnderline, const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetUnderline(hasUnderline, textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetStrikethrough(const bool hasStrikethrough, const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetStrikethrough(hasStrikethrough, textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetDrawingEffect(const ComPtr<IUnknown>& drawingEffect,
	                                  const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetDrawingEffect(drawingEffect.Get(), textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetInlineObject(const ComPtr<IDWriteInlineObject>& inlineObject,
	                                 const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetInlineObject(inlineObject.Get(), textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetTypography(const ComPtr<IDWriteTypography>& typography,
	                               const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetTypography(typography.Get(), textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetLocaleName(const std::wstring_view localeName,
	                               const TextRange textRange) const noexcept -> Error
	{
		return Error{
			Get()->SetLocaleName(localeName.data(), textRange)
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::SetTrimming(const Trimming& trimming) const noexcept -> Error
	{
		auto& trimmingOptions = trimming.trimmingOptions;
		return Error{
			Get()->SetTrimming(&trimmingOptions, trimming.GetRaw())
		}.AddTag(ErrorTags::Font);
	}

	auto TextLayout::GetTrimming() const noexcept -> Result<Trimming>
	{
		Trimming trimming{ };
		if (const auto hr = Get()->GetTrimming(
			&trimming.trimmingOptions,
			trimming.GetAddress());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

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

	auto TextLayout::GetLineSpacing() const noexcept -> Result<LineSpacing>
	{
		LineSpacing lineSpacing{ };
		if (const auto hr = Get()->GetLineSpacing(
			&lineSpacing.method,
			&lineSpacing.height, &lineSpacing.baseline);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

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

	auto TextLayout::GetFontCollection() const noexcept -> Result<FontCollection>
	{
		FontCollection fontCollection{ nullptr };
		if (const auto hr = Get()->GetFontCollection(
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return fontCollection;
	}

	auto TextLayout::GetFontCollection(const UINT32 position) const noexcept -> Result<FontCollection>
	{
		FontCollection fontCollection{ nullptr };
		if (const auto hr = Get()->GetFontCollection(
			position, fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return fontCollection;
	}

	auto TextLayout::GetFontCollection(const UINT32 position,
	                                   TextRange& textRange) const noexcept -> Result<FontCollection>
	{
		FontCollection fontCollection{ nullptr };
		if (const auto hr = Get()->GetFontCollection(
			position,
			fontCollection.GetAddressAs<IDWriteFontCollection3, IDWriteFontCollection>(),
			&textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return fontCollection;
	}

	auto TextLayout::GetFontFamilyName() const noexcept -> Result<std::wstring>
	{
		const auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		if (const auto hr = Get()->GetFontFamilyName(
				fontFamilyName.data(), length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return fontFamilyName;
	}

	auto TextLayout::GetFontFamilyName(const UINT32 position) const noexcept -> Result<std::wstring>
	{
		const auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		if (const auto hr = Get()->GetFontFamilyName(
				position, fontFamilyName.data(), length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return fontFamilyName;
	}

	auto TextLayout::GetFontFamilyName(const UINT32 position,
	                                   TextRange& textRange) const noexcept -> Result<std::wstring>
	{
		const auto length = Get()->GetFontFamilyNameLength();
		std::wstring fontFamilyName(length, L'\0');

		if (const auto hr = Get()->GetFontFamilyName(
				position, fontFamilyName.data(), length, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return fontFamilyName;
	}

	auto TextLayout::GetFontWeight() const noexcept -> FontWeight
	{
		return Get()->GetFontWeight();
	}

	auto TextLayout::GetFontWeight(const UINT32 position) const noexcept -> Result<FontWeight>
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		if (const auto hr = Get()->GetFontWeight(position, &fontWeight);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return fontWeight;
	}

	auto TextLayout::GetFontWeight(const UINT32 position, TextRange& textRange) const noexcept -> Result<FontWeight>
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		if (const auto hr = Get()->GetFontWeight(position, &fontWeight, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return fontWeight;
	}

	auto TextLayout::GetFontStyle() const noexcept -> FontStyle
	{
		return Get()->GetFontStyle();
	}

	auto TextLayout::GetFontStyle(const UINT32 position) const noexcept -> Result<FontStyle>
	{
		DWRITE_FONT_STYLE fontStyle{ };
		if (const auto hr = Get()->GetFontStyle(position, &fontStyle);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return fontStyle;
	}

	auto TextLayout::GetFontStyle(const UINT32 position, TextRange& textRange) const noexcept -> Result<FontStyle>
	{
		DWRITE_FONT_STYLE fontStyle{ };
		if (const auto hr = Get()->GetFontStyle(position, &fontStyle, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return fontStyle;
	}

	auto TextLayout::GetFontStretch() const noexcept -> FontStretch
	{
		return Get()->GetFontStretch();
	}

	auto TextLayout::GetFontStretch(const UINT32 position) const noexcept -> Result<FontStretch>
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		if (const auto hr = Get()->GetFontStretch(position, &fontStretch);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return fontStretch;
	}

	auto TextLayout::GetFontStretch(const UINT32 position, TextRange& textRange) const noexcept -> Result<FontStretch>
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		if (const auto hr = Get()->GetFontStretch(position, &fontStretch, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return fontStretch;
	}

	auto TextLayout::GetFontSize() const noexcept -> float
	{
		return Get()->GetFontSize();
	}

	auto TextLayout::GetFontSize(const UINT32 position) const noexcept -> Result<float>
	{
		float fontSize{ };
		if (const auto hr = Get()->GetFontSize(position, &fontSize);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return fontSize;
	}

	auto TextLayout::GetFontSize(const UINT32 position, TextRange& textRange) const noexcept -> Result<float>
	{
		float fontSize{ };
		if (const auto hr = Get()->GetFontSize(position, &fontSize, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return fontSize;
	}

	auto TextLayout::GetUnderline(const UINT32 position) const noexcept -> Result<bool>
	{
		BOOL underline{ };
		if (const auto hr = Get()->GetUnderline(position, &underline);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return underline;
	}

	auto TextLayout::GetUnderline(const UINT32 position, TextRange& textRange) const noexcept -> Result<bool>
	{
		BOOL underline{ };
		if (const auto hr = Get()->GetUnderline(position, &underline, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return underline;
	}

	auto TextLayout::GetStrikethrough(const UINT32 position) const noexcept -> Result<bool>
	{
		BOOL strikethrough{ };
		if (const auto hr = Get()->GetStrikethrough(position, &strikethrough);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return strikethrough;
	}

	auto TextLayout::GetStrikethrough(const UINT32 position, TextRange& textRange) const noexcept -> Result<bool>
	{
		BOOL strikethrough{ };
		if (const auto hr = Get()->GetStrikethrough(position, &strikethrough, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return strikethrough;
	}

	auto TextLayout::GetDrawingEffect(const UINT32 position) const noexcept -> Result<ComPtr<IUnknown>>
	{
		ComPtr<IUnknown> drawingEffect;
		if (const auto hr = Get()->GetDrawingEffect(
				position, drawingEffect.GetAddressOf(), nullptr);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return drawingEffect;
	}

	auto TextLayout::GetDrawingEffect(const UINT32 position,
	                                  TextRange& textRange) const noexcept -> Result<ComPtr<IUnknown>>
	{
		ComPtr<IUnknown> drawingEffect;
		if (const auto hr = Get()->GetDrawingEffect(
				position, drawingEffect.GetAddressOf(), &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return drawingEffect;
	}

	auto TextLayout::GetInlineObject(const UINT32 position) const noexcept -> Result<ComPtr<IDWriteInlineObject>>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		if (const auto hr = Get()->GetInlineObject(
				position, inlineObject.GetAddressOf(), nullptr);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return inlineObject;
	}

	auto TextLayout::GetInlineObject(
		const UINT32 position,
		TextRange& textRange) const noexcept -> Result<ComPtr<IDWriteInlineObject>>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		if (const auto hr = Get()->GetInlineObject(
				position, inlineObject.GetAddressOf(), &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return inlineObject;
	}

	auto TextLayout::GetTypography(const UINT32 position) const noexcept -> Result<ComPtr<IDWriteTypography>>
	{
		ComPtr<IDWriteTypography> typography;
		if (const auto hr = Get()->GetTypography(
				position, typography.GetAddressOf(), nullptr);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return typography;
	}

	auto TextLayout::GetTypography(const UINT32 position,
	                               TextRange& textRange) const noexcept -> Result<ComPtr<IDWriteTypography>>
	{
		ComPtr<IDWriteTypography> typography;
		if (const auto hr = Get()->GetTypography(
				position, typography.GetAddressOf(), &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return typography;
	}

	auto TextLayout::GetLocaleName() const noexcept -> Result<std::wstring>
	{
		const auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');
		if (const auto hr = Get()->GetLocaleName(localeName.data(), length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return localeName;
	}

	auto TextLayout::GetLocaleName(const UINT32 position) const noexcept -> Result<std::wstring>
	{
		const auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');

		if (const auto hr = Get()->GetLocaleName(
				position, localeName.data(), length);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
			};
		}

		return localeName;
	}

	auto TextLayout::GetLocaleName(const UINT32 position, TextRange& textRange) const noexcept -> Result<std::wstring>
	{
		const auto length = Get()->GetLocaleNameLength();
		std::wstring localeName(length, L'\0');
		if (const auto hr = Get()->GetLocaleName(
				position, localeName.data(), length, &textRange);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range",
				           std::format(L"{} {}",
				                       textRange.GetStartPosition(), textRange.GetEndPosition()))
			};
		}

		return localeName;
	}

	auto TextLayout::DetermineMinWidth() const noexcept -> Result<float>
	{
		auto minWidth = 0.0F;
		if (const auto hr = Get()->DetermineMinWidth(&minWidth);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return minWidth;
	}
}

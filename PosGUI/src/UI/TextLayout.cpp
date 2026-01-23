module;
#include <dwrite_3.h>

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
			std::bit_cast<IDWriteTextLayout**>(Put()));

		LogIfFailed(
			Error{
				hr
			}
			.AddDetail(L"Text", text)
			.AddDetail(L"MaxSize", std::format(L"{}", maxSize)),
			L"Cannot create text layout"
		);
	}

	auto TextLayout::SetTextAlignment(const TextAlignment textAlignment) const noexcept -> Error
	{
		Error error{
			Get()->SetTextAlignment(textAlignment)
		};
		LogIfFailed(error, L"Cannot set text alignment");
		return error;
	}

	auto TextLayout::SetParagraphAlignment(const ParagraphAlignment paragraphAlignment) const noexcept -> Error
	{
		Error error{
			Get()->SetParagraphAlignment(paragraphAlignment)
		};
		LogIfFailed(error, L"Cannot set paragraph alignment");
		return error;
	}

	auto TextLayout::SetWordWrapping(const WordWrapping wordWrapping) const noexcept -> Error
	{
		Error error{
			Get()->SetWordWrapping(wordWrapping)
		};
		LogIfFailed(error, L"Cannot set word wrapping");
		return error;
	}

	auto TextLayout::SetReadingDirection(const ReadingDirection readingDirection) const noexcept -> Error
	{
		Error error{
			Get()->SetReadingDirection(readingDirection)
		};
		LogIfFailed(error, L"Cannot set reading direction");
		return error;
	}

	auto TextLayout::SetFlowDirection(const FlowDirection flowDirection) const noexcept -> Error
	{
		Error error{
			Get()->SetFlowDirection(flowDirection)
		};
		LogIfFailed(error, L"Cannot set flow direction");
		return error;
	}

	auto TextLayout::SetIncrementalTabStop(const float incrementalTabStop) const noexcept -> Error
	{
		Error error{
			Get()->SetIncrementalTabStop(incrementalTabStop)
		};
		error
			.AddDetail(L"Incremental Tab Stop", std::format(L"{:.5F}", incrementalTabStop));
		LogIfFailed(error, L"Cannot set incremental tab stop");
		return error;
	}

	auto TextLayout::SetLineSpacing(const LineSpacing& lineSpacing) const noexcept -> Error
	{
		Error error{
			Get()->SetLineSpacing(
				lineSpacing.method, lineSpacing.height, lineSpacing.baseline)
		};
		LogIfFailed(error, L"Cannot set line spacing");
		return error;
	}

	auto TextLayout::SetMaxWidth(const float maxWidth) const noexcept -> Error
	{
		Error error{
			Get()->SetMaxWidth(maxWidth)
		};
		error
			.AddDetail(L"Max Width", std::format(L"{:.5F}", maxWidth));
		LogIfFailed(error, L"Cannot set max width");
		return error;
	}

	auto TextLayout::SetMaxHeight(const float maxHeight) const noexcept -> Error
	{
		Error error{
			Get()->SetMaxHeight(maxHeight)
		};
		error
			.AddDetail(L"Max Width", std::format(L"{:.5F}", maxHeight));
		LogIfFailed(error, L"Cannot set max height");
		return error;
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
		Error error{
			Get()->SetFontCollection(
				fontCollection.GetRawAs<IDWriteFontCollection3, IDWriteFontCollection>(),
				textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set font collection");
		return error;
	}

	auto TextLayout::SetFontFamilyName(const std::wstring_view fontFamilyName,
	                                   const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetFontFamilyName(fontFamilyName.data(), textRange)
		};
		error
			.AddDetail(L"Font Family Name", fontFamilyName)
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set font family name");
		return error;
	}

	auto TextLayout::SetFontWeight(const FontWeight fontWeight, const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetFontWeight(fontWeight, textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set font weight");
		return error;
	}

	auto TextLayout::SetFontStyle(const FontStyle fontStyle, const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetFontStyle(fontStyle, textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set font style");
		return error;
	}

	auto TextLayout::SetFontStretch(const FontStretch fontStretch, const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetFontStretch(fontStretch, textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set font stretch");
		return error;
	}

	auto TextLayout::SetFontSize(const float fontSize, const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetFontSize(fontSize, textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set font size");
		return error;
	}

	auto TextLayout::SetUnderline(const bool hasUnderline, const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetUnderline(hasUnderline, textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set underline");
		return error;
	}

	auto TextLayout::SetStrikethrough(const bool hasStrikethrough, const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetStrikethrough(hasStrikethrough, textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set strikethrough");
		return error;
	}

	auto TextLayout::SetDrawingEffect(const ComPtr<IUnknown>& drawingEffect,
	                                  const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetDrawingEffect(drawingEffect.get(), textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set drawing effect");
		return error;
	}

	auto TextLayout::SetInlineObject(const ComPtr<IDWriteInlineObject>& inlineObject,
	                                 const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetInlineObject(inlineObject.get(), textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set inline object");
		return error;
	}

	auto TextLayout::SetTypography(const ComPtr<IDWriteTypography>& typography,
	                               const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetTypography(typography.get(), textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set typography");
		return error;
	}

	auto TextLayout::SetLocaleName(const std::wstring_view localeName,
	                               const TextRange textRange) const noexcept -> Error
	{
		Error error{
			Get()->SetLocaleName(localeName.data(), textRange)
		};
		error
			.AddDetail(L"Text Range", std::format(L"{}", textRange));
		LogIfFailed(error, L"Cannot set locale name");
		return error;
	}

	auto TextLayout::SetTrimming(const Trimming& trimming) const noexcept -> Error
	{
		auto& trimmingOptions = trimming.trimmingOptions;
		Error error{
			Get()->SetTrimming(&trimmingOptions, trimming.GetRaw())
		};
		LogIfFailed(error, L"Cannot set trimming");
		return error;
	}

	auto TextLayout::GetTrimming() const noexcept -> Result<Trimming>
	{
		Trimming trimming{ };
		if (const auto hr = Get()->GetTrimming(
			&trimming.trimmingOptions,
			trimming.Put());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Cannot get trimming");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Cannot get line spacing");
			return Unexpected{ error };
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
				fontCollection.PutAs<IDWriteFontCollection3, IDWriteFontCollection>());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Cannot get font collection");
			return Unexpected{ error };
		}

		return fontCollection;
	}

	auto TextLayout::GetFontCollection(const UINT32 position) const noexcept -> Result<FontCollection>
	{
		FontCollection fontCollection{ nullptr };
		if (const auto hr = Get()->GetFontCollection(
				position, fontCollection.PutAs<IDWriteFontCollection3, IDWriteFontCollection>());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get font collection");
			return Unexpected{ error };
		}

		return fontCollection;
	}

	auto TextLayout::GetFontCollection(const UINT32 position,
	                                   TextRange& textRange) const noexcept -> Result<FontCollection>
	{
		FontCollection fontCollection{ nullptr };
		if (const auto hr = Get()->GetFontCollection(
			position,
			fontCollection.PutAs<IDWriteFontCollection3, IDWriteFontCollection>(),
			&textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get font collection");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Cannot get font family name");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get font family name");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get font family name");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get font weight");
			return Unexpected{ error };
		}

		return fontWeight;
	}

	auto TextLayout::GetFontWeight(const UINT32 position, TextRange& textRange) const noexcept -> Result<FontWeight>
	{
		DWRITE_FONT_WEIGHT fontWeight{ };
		if (const auto hr = Get()->GetFontWeight(position, &fontWeight, &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get font weight");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get font style");
			return Unexpected{ error };
		}

		return fontStyle;
	}

	auto TextLayout::GetFontStyle(const UINT32 position, TextRange& textRange) const noexcept -> Result<FontStyle>
	{
		DWRITE_FONT_STYLE fontStyle{ };
		if (const auto hr = Get()->GetFontStyle(position, &fontStyle, &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get font style");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get font stretch");
			return Unexpected{ error };
		}

		return fontStretch;
	}

	auto TextLayout::GetFontStretch(const UINT32 position, TextRange& textRange) const noexcept -> Result<FontStretch>
	{
		DWRITE_FONT_STRETCH fontStretch{ };
		if (const auto hr = Get()->GetFontStretch(position, &fontStretch, &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get font stretch");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get font size");
			return Unexpected{ error };
		}

		return fontSize;
	}

	auto TextLayout::GetFontSize(const UINT32 position, TextRange& textRange) const noexcept -> Result<float>
	{
		float fontSize{ };
		if (const auto hr = Get()->GetFontSize(position, &fontSize, &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get font size");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get underline");
			return Unexpected{ error };
		}

		return underline;
	}

	auto TextLayout::GetStrikethrough(const UINT32 position) const noexcept -> Result<bool>
	{
		BOOL strikethrough{ };
		if (const auto hr = Get()->GetStrikethrough(position, &strikethrough);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get strikethrough");
			return Unexpected{ error };
		}

		return strikethrough;
	}

	auto TextLayout::GetStrikethrough(const UINT32 position, TextRange& textRange) const noexcept -> Result<bool>
	{
		BOOL strikethrough{ };
		if (const auto hr = Get()->GetStrikethrough(position, &strikethrough, &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get strikethrough");
			return Unexpected{ error };
		}

		return strikethrough;
	}

	auto TextLayout::GetDrawingEffect(const UINT32 position) const noexcept -> Result<ComPtr<IUnknown>>
	{
		ComPtr<IUnknown> drawingEffect;
		if (const auto hr = Get()->GetDrawingEffect(
				position, drawingEffect.put(), nullptr);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get drawing effect");
			return Unexpected{ error };
		}

		return drawingEffect;
	}

	auto TextLayout::GetDrawingEffect(const UINT32 position,
	                                  TextRange& textRange) const noexcept -> Result<ComPtr<IUnknown>>
	{
		ComPtr<IUnknown> drawingEffect;
		if (const auto hr = Get()->GetDrawingEffect(
				position, drawingEffect.put(), &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get drawing effect");
			return Unexpected{ error };
		}

		return drawingEffect;
	}

	auto TextLayout::GetInlineObject(const UINT32 position) const noexcept -> Result<ComPtr<IDWriteInlineObject>>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		if (const auto hr = Get()->GetInlineObject(
				position, inlineObject.put(), nullptr);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get inline object");
			return Unexpected{ error };
		}

		return inlineObject;
	}

	auto TextLayout::GetInlineObject(
		const UINT32 position,
		TextRange& textRange) const noexcept -> Result<ComPtr<IDWriteInlineObject>>
	{
		ComPtr<IDWriteInlineObject> inlineObject;
		if (const auto hr = Get()->GetInlineObject(
				position, inlineObject.put(), &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get inline object");
			return Unexpected{ error };
		}

		return inlineObject;
	}

	auto TextLayout::GetTypography(const UINT32 position) const noexcept -> Result<ComPtr<IDWriteTypography>>
	{
		ComPtr<IDWriteTypography> typography;
		if (const auto hr = Get()->GetTypography(
				position, typography.put(), nullptr);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get typography");
			return Unexpected{ error };
		}

		return typography;
	}

	auto TextLayout::GetTypography(const UINT32 position,
	                               TextRange& textRange) const noexcept -> Result<ComPtr<IDWriteTypography>>
	{
		ComPtr<IDWriteTypography> typography;
		if (const auto hr = Get()->GetTypography(
				position, typography.put(), &textRange);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get typography");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Cannot get locale name");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position));
			Logger::Error(error, L"Cannot get locale name");
			return Unexpected{ error };
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
			Error error{ hr };
			error
				.AddDetail(L"Position", std::to_wstring(position))
				.AddDetail(L"Text Range", std::format(L"{}", textRange));
			Logger::Error(error, L"Cannot get locale name");
			return Unexpected{ error };
		}

		return localeName;
	}

	auto TextLayout::DetermineMinWidth() const noexcept -> Result<float>
	{
		auto minWidth = 0.0F;
		if (const auto hr = Get()->DetermineMinWidth(&minWidth);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Cannot determine minimum width");
			return Unexpected{ error };
		}

		return minWidth;
	}
}

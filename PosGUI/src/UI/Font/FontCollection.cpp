module;
#include <dwrite_3.h>
#include <wrl.h>

module PGUI.UI.Font.FontCollection;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.ErrorHandling;

namespace PGUI::UI::Font
{
	auto FontCollection::GetSystemFontCollection() noexcept -> Result<FontCollection>
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		ComPtr<IDWriteFontCollection3> fontCollection;
		if (const auto hr = factory->GetSystemFontCollection(
			false, DWRITE_FONT_FAMILY_MODEL_TYPOGRAPHIC,
			fontCollection.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddTag(ErrorTags::System)
			};
		}

		return fontCollection;
	}

	auto FontCollection::LoadFontFile(const std::filesystem::path& filePath) noexcept -> Result<FontCollection>
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();
		ComPtr<IDWriteFontSetBuilder2> fontSetBuilder;

		auto hr = factory->CreateFontSetBuilder(&fontSetBuilder);
		if (FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddTag(ErrorTags::System)
			};
		}

		ComPtr<IDWriteFontFile> fontFile;
		hr = factory->CreateFontFileReference(filePath.c_str(), nullptr, &fontFile);
		if (FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
				.AddTag(ErrorTags::System)
			};
		}

		hr = fontSetBuilder->AddFontFile(fontFile.Get());
		if (FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		ComPtr<IDWriteFontSet> fontSet;
		hr = fontSetBuilder->CreateFontSet(&fontSet);
		if (FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		ComPtr<IDWriteFontCollection1> fontCollection;
		hr = factory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);
		if (FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		ComPtr<IDWriteFontCollection3> fontCollection3;
		hr = fontCollection.As(&fontCollection3);
		if (FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return fontCollection3;
	}

	FontCollection::FontCollection(const ComPtr<IDWriteFontCollection3>& collection) noexcept :
		ComPtrHolder{ collection }
	{ }

	auto FontCollection::FindFontFamilyByName(
		const std::wstring_view fontFamilyName) const noexcept -> Result<UINT32>
	{
		auto& ptr = Get();

		UINT32 result;
		BOOL exists;
		const auto hr = ptr->FindFamilyName(fontFamilyName.data(), &result, &exists);
		LogIfFailed(
			Error{ hr }
			.AddDetail(L"fontFamilName", fontFamilyName)
		);

		if (!exists)
		{
			return Unexpected{
				Error{ E_FAIL }
				.AddTag(ErrorTags::Font)
			};
		}

		return result;
	}

	auto FontCollection::GetFontFamily(
		const std::wstring_view fontFamilyName) const noexcept -> Result<FontFamily>
	{
		return FindFontFamilyByName(fontFamilyName).and_then(
			[this](const auto& index) -> Result<FontFamily>
		{
			return GetFontFamily(index);
		});
	}

	auto FontCollection::GetFontFamily(const UINT32 index) const noexcept -> Result<FontFamily>
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontFamily2> family;
		if (const auto hr = ptr->GetFontFamily(index, family.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}

		return family;
	}

	auto FontCollection::GetFontSet() const -> Result<FontSet>
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontSet1> fontSet1;
		ComPtr<IDWriteFontSet4> fontSet;
		if (const auto hr = ptr->GetFontSet(fontSet1.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}


		if (const auto hr = fontSet1.As(&fontSet);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Font)
			};
		}


		return fontSet;
	}

	auto FontCollection::GetFontFamilyCount() const noexcept -> UINT32
	{
		return Get()->GetFontFamilyCount();
	}

	auto FontCollection::GetExpirationEvent() const noexcept -> HANDLE
	{
		return Get()->GetExpirationEvent();
	}
}

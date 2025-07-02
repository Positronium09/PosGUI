module;
#include <wrl.h>
#include <dwrite_3.h>

module PGUI.UI.Font.FontCollection;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.Logging;

namespace PGUI::UI::Font
{
	auto FontCollection::GetSystemFontCollection() -> FontCollection
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();

		ComPtr<IDWriteFontCollection3> fontCollection;
		auto hr = factory->GetSystemFontCollection(false,  DWRITE_FONT_FAMILY_MODEL_TYPOGRAPHIC, 
			fontCollection.GetAddressOf()); ThrowFailed(hr);

		return fontCollection;
	}

	auto FontCollection::LoadFontFile(std::filesystem::path filePath) -> FontCollection
	{
		const auto& factory = Factories::DWriteFactory::GetFactory();
		ComPtr<IDWriteFontSetBuilder2> fontSetBuilder;

		auto hr = factory->CreateFontSetBuilder(&fontSetBuilder); ThrowFailed(hr);

		ComPtr<IDWriteFontFile> fontFile;
		hr = factory->CreateFontFileReference(filePath.c_str(), nullptr, &fontFile); ThrowFailed(hr);

		fontSetBuilder->AddFontFile(fontFile.Get());

		ComPtr<IDWriteFontSet> fontSet;
		hr = fontSetBuilder->CreateFontSet(&fontSet); ThrowFailed(hr);

		ComPtr<IDWriteFontCollection1> fontCollection;
		hr = factory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection); ThrowFailed(hr);

		ComPtr<IDWriteFontCollection3> fontCollection3;
		hr = fontCollection.As(&fontCollection3); ThrowFailed(hr);

		return fontCollection3;
	}

	FontCollection::FontCollection(ComPtr<IDWriteFontCollection3> collection) noexcept :
		ComPtrHolder{ collection }
	{
	}

	auto FontCollection::FindFontFamilyByName(std::wstring_view fontFamilyName) const noexcept -> std::expected<UINT32, bool>
	{
		auto& ptr = Get();

		UINT32 result;
		BOOL exists;
		auto hr = ptr->FindFamilyName(fontFamilyName.data(), &result, &exists); LogFailed(LogLevel::Error, hr);

		if (!exists)
		{
			return std::unexpected{ false };
		}

		return result;
	}

	auto FontCollection::GetFontFamily(std::wstring_view fontFamilyName) const noexcept -> FontFamily
	{
		auto found = FindFontFamilyByName(fontFamilyName);
		if (found)
		{
			return GetFontFamily(*found);
		}

		return FontFamily{ nullptr };
	}
	
	auto FontCollection::GetFontFamily(UINT32 index) const noexcept -> FontFamily
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontFamily2> family;
		auto hr = ptr->GetFontFamily(index, family.GetAddressOf()); ThrowFailed(hr);

		return family;
	}
	
	auto FontCollection::GetFontSet() const -> FontSet
	{
		auto& ptr = Get();

		ComPtr<IDWriteFontSet1> fontSet1;
		ComPtr<IDWriteFontSet4> fontSet;
		auto hr = ptr->GetFontSet(fontSet1.GetAddressOf()); ThrowFailed(hr);

		hr = fontSet1.As(&fontSet);  ThrowFailed(hr);

		return fontSet;
	}
	
	auto FontCollection::GetFontFamilyCount() const noexcept -> UINT32
	{
		auto& ptr = Get();

		return ptr->GetFontFamilyCount();
	}
	auto FontCollection::GetExpirationEvent() const noexcept -> HANDLE
	{
		auto& ptr = Get();

		return ptr->GetExpirationEvent();
	}
}

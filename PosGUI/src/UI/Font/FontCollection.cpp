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
			Error error{ hr };
			Logger::Error(error, L"Failed to get system font collection.");
			return Unexpected{ error };
		}

		return fontCollection;
	}

	auto FontCollection::LoadFontFile(const std::filesystem::path& filePath) noexcept -> Result<FontCollection>
	{
		if (!std::filesystem::exists(filePath))
		{
			Error error{ ErrorCode::InvalidArgument };
			error
				.AddDetail(L"File Path", filePath.wstring())
				.SuggestFix(L"Ensure the file path is correct and the file exists");
			Logger::Error(error, L"Font file does not exist.");
			return Unexpected{ error };
		}

		const auto& factory = Factories::DWriteFactory::GetFactory();
		ComPtr<IDWriteFontSetBuilder2> fontSetBuilder;

		auto hr = factory->CreateFontSetBuilder(&fontSetBuilder);
		if (FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"File Path", filePath.wstring());
			Logger::Error(error, L"Failed to create font set builder.");
			return Unexpected{ error };
		}

		ComPtr<IDWriteFontFile> fontFile;
		hr = factory->CreateFontFileReference(filePath.c_str(), nullptr, &fontFile);
		if (FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"File Path", filePath.wstring());
			Logger::Error(error, L"Failed to create font file reference.");
			return Unexpected{ error };
		}

		hr = fontSetBuilder->AddFontFile(fontFile.Get());
		if (FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"File Path", filePath.wstring());
			Logger::Error(error, L"Failed to add font file to font set builder.");
			return Unexpected{ error };
		}

		ComPtr<IDWriteFontSet> fontSet;
		hr = fontSetBuilder->CreateFontSet(&fontSet);
		if (FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"File Path", filePath.wstring());
			Logger::Error(error, L"Failed to create font set from builder.");
			return Unexpected{ error };
		}

		ComPtr<IDWriteFontCollection1> fontCollection;
		hr = factory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);
		if (FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"File Path", filePath.wstring());
			Logger::Error(error, L"Failed to create font collection from font set.");
			return Unexpected{ error };
		}

		ComPtr<IDWriteFontCollection3> fontCollection3;
		hr = fontCollection.As(&fontCollection3);
		if (FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"File Path", filePath.wstring());
			Logger::Error(error, L"Failed to cast font collection to IDWriteFontCollection3.");
			return Unexpected{ error };
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
			.AddDetail(L"fontFamilName", fontFamilyName),
			L"An error occurred while trying to find a font family by name."
		);

		if (!exists)
		{
			return Unexpected{
				Error{ ErrorCode::Failure }
				.AddDetail(L"Font Family Name", fontFamilyName)
				.SuggestFix(L"Ensure the font family name is correct and try again")
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
			Error error{ hr };
			error
				.AddDetail(L"Index", std::to_wstring(index));
			Logger::Error(error, L"Failed to get font family by index.");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Failed to get font set.");
			return Unexpected{ error };
		}


		if (const auto hr = fontSet1.As(&fontSet);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to cast font set to IDWriteFontSet4.");
			return Unexpected{ error };
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

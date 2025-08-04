module;
#include <dwrite_3.h>
#include <Windows.h>

export module PGUI.UI.Font.FontCollection;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.Font.FontFamily;
import PGUI.UI.Font.FontSet;

export namespace PGUI::UI::Font
{
	class FontCollection final : public ComPtrHolder<IDWriteFontCollection3>
	{
		public:
		[[nodiscard]] static auto GetSystemFontCollection() noexcept -> Result<FontCollection>;

		[[nodiscard]] static auto LoadFontFile(
			const std::filesystem::path& filePath) noexcept -> Result<FontCollection>;

		FontCollection() noexcept = default;

		explicit(false) FontCollection(const ComPtr<IDWriteFontCollection3>& collection) noexcept;

		[[nodiscard]] auto FindFontFamilyByName(
			std::wstring_view fontFamilyName) const noexcept -> Result<UINT32>;

		[[nodiscard]] auto GetFontFamily(
			std::wstring_view fontFamilyName) const noexcept -> Result<FontFamily>;

		[[nodiscard]] auto GetFontFamily(UINT32 index) const noexcept -> Result<FontFamily>;

		[[nodiscard]] auto GetFontSet() const -> Result<FontSet>;

		[[nodiscard]] auto GetFontFamilyCount() const noexcept -> UINT32;

		[[nodiscard]] auto GetExpirationEvent() const noexcept -> HANDLE;

		//TODO GetFontFromFontFace
		//TODO GetFontFamilyModel
		//TODO GetMatchingFontsByFamilyName
	};
}

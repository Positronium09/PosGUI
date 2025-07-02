module;
#include <Windows.h>
#include <dwrite_3.h>

export module PGUI.UI.Font.FontCollection;

import std;

import PGUI.ComPtr;
import PGUI.UI.Font.FontFamily;
import PGUI.UI.Font.FontSet;

export namespace PGUI::UI::Font
{
	class FontCollection final : public ComPtrHolder<IDWriteFontCollection3>
	{
		public:
		[[nodiscard]] static auto GetSystemFontCollection() -> FontCollection;
		[[nodiscard]] static auto LoadFontFile(std::filesystem::path filePath) -> FontCollection;

		FontCollection() noexcept = default;
		FontCollection(ComPtr<IDWriteFontCollection3> collection) noexcept;

		[[nodiscard]] auto FindFontFamilyByName(std::wstring_view fontFamilyName) const noexcept -> std::expected<UINT32, bool>;
		[[nodiscard]] auto GetFontFamily(std::wstring_view fontFamilyName) const noexcept -> FontFamily;
		[[nodiscard]] auto GetFontFamily(UINT32 index) const noexcept -> FontFamily;
		[[nodiscard]] auto GetFontSet() const -> FontSet;
		[[nodiscard]] auto GetFontFamilyCount() const noexcept -> UINT32;
		[[nodiscard]] auto GetExpirationEvent() const noexcept -> HANDLE;
		//TODO GetFontFromFontFace
		//TODO GetFontFamilyModel
		//TODO GetMatchingFontsByFamilyName
	};
}

module;
#include <dwrite_1.h>
#include <wrl.h>

export module PGUI.UI.Font.LocalizedStrings;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::UI::Font
{
	class LocalizedStrings : public ComPtrHolder<IDWriteLocalizedStrings>
	{
		public:
		explicit(false) LocalizedStrings(const ComPtr<IDWriteLocalizedStrings>& strings) noexcept;

		[[nodiscard]] auto FindLocaleName(std::wstring_view name) const noexcept -> Result<UINT32>;

		[[nodiscard]] auto GetCount() const noexcept { return Get()->GetCount(); }

		[[nodiscard]] auto GetLocaleName(UINT32 index) const noexcept -> Result<std::wstring>;

		[[nodiscard]] auto GetString(UINT32 index) const noexcept -> Result<std::wstring>;
	};
}

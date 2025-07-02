module;
#include <wincodec.h>

export module PGUI.UI.Imaging.BitmapFrameDecode;

import std;

import PGUI.ComPtr;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.MetadataReader;

export namespace PGUI::UI::Imaging
{
	class BitmapFrameDecode : public BitmapSource<IWICBitmapFrameDecode>
	{
		public:
		BitmapFrameDecode() noexcept = default;

		explicit(false) BitmapFrameDecode(ComPtr<IWICBitmapFrameDecode> frame) noexcept;

		[[nodiscard]] auto GetMetadataReader() const -> MetadataReader;

		[[nodiscard]] auto GetThumbnail() const -> BitmapSource<>;

		//TODO Maybe wrap IWICColorContext
		[[nodiscard]] auto GetColorContexts(UINT count) const -> std::vector<ComPtr<IWICColorContext>>;
	};
}

module;
#include <wrl.h>
#include <wincodec.h>

export module PGUI.UI.Imaging.WICBitmap;

import PGUI.ComPtr;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;
import PGUI.UI.Imaging.WICBitmapLock;

export namespace PGUI::UI::Imaging
{
	class WICBitmap : public BitmapSource<IWICBitmap>
	{
		public:
		WICBitmap() noexcept = default;
		WICBitmap(ComPtr<IWICBitmap> bitmap) noexcept;
		//TODO Wrapper for IWICBitmapLock
		[[nodiscard]] auto Lock(RectI rect, WICBitmapLockFlags flags) const noexcept -> WICBitmapLock;

		void SetPalette(Palette palette) noexcept;

		void SetResolution(double dpiX, double dpiY) noexcept;
	};
}

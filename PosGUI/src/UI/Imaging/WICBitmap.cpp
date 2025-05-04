module;
#include <wrl.h>
#include <wincodec.h>
#include <stacktrace>

module PGUI.UI.Imaging.WICBitmap;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;

namespace PGUI::UI::Imaging
{
	WICBitmap::WICBitmap(ComPtr<IWICBitmap> bitmap) noexcept :
		BitmapSource<IWICBitmap>{ bitmap }
	{
	}

	auto WICBitmap::Lock(RectI rect, WICBitmapLockFlags flags) const noexcept -> WICBitmapLock
	{
		WICRect wicRect = rect;
		ComPtr<IWICBitmapLock> lock;

		auto hr = Get()->Lock(&wicRect, static_cast<DWORD>(flags), &lock); ThrowFailed(hr);

		return lock;
	}

	void WICBitmap::SetPalette(Palette palette) noexcept
	{
		auto hr = Get()->SetPalette(palette.GetRaw()); ThrowFailed(hr);
	}

	void WICBitmap::SetResolution(double dpiX, double dpiY) noexcept
	{
		auto hr = this->Get()->SetResolution(dpiX, dpiY); ThrowFailed(hr);
	}
}

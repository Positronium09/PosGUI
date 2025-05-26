module;
#include <wrl.h>
#include <wincodec.h>
#include <stacktrace>

module PGUI.UI.Imaging.WICBitmap;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;

namespace PGUI::UI::Imaging
{
	WICBitmap::WICBitmap(ComPtr<IWICBitmap> bitmap) noexcept :
		BitmapSource<IWICBitmap>{ bitmap }
	{
	}
	WICBitmap::WICBitmap(SizeU size, 
		const WICPixelFormatGUID& pixelFormat, CreateCacheOption cacheOption)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto hr = factory->CreateBitmap(size.cx, size.cy, pixelFormat,
			static_cast<WICBitmapCreateCacheOption>(cacheOption), 
			GetAddress()); ThrowFailed(hr);
	}
	WICBitmap::WICBitmap(HBITMAP hBitmap, AlphaChannelOption alphaOption, HPALETTE hPalette)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto hr = factory->CreateBitmapFromHBITMAP(hBitmap, hPalette,
			static_cast<WICBitmapAlphaChannelOption>(alphaOption), 
			GetAddress()); ThrowFailed(hr);
	}
	WICBitmap::WICBitmap(HICON hIcon)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto hr = factory->CreateBitmapFromHICON(hIcon, GetAddress()); ThrowFailed(hr);
	}
	WICBitmap::WICBitmap(BitmapSource<> source, CreateCacheOption cacheOption)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto hr = factory->CreateBitmapFromSource(source.GetRaw(),
			static_cast<WICBitmapCreateCacheOption>(cacheOption), 
			GetAddress()); ThrowFailed(hr);
	}
	WICBitmap::WICBitmap(BitmapSource<> source, RectU rect)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto size = rect.Size();

		auto hr = factory->CreateBitmapFromSourceRect(source.GetRaw(), 
			rect.left, rect.right, size.cx, size.cy, 
			GetAddress()); ThrowFailed(hr);
	}

	auto WICBitmap::Lock(RectI rect, LockFlags flags) const noexcept -> WICBitmapLock
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

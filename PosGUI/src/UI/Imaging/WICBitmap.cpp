module;
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.WICBitmap;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;

namespace PGUI::UI::Imaging
{
	WICBitmap::WICBitmap(const ComPtr<IWICBitmap>& bitmap) noexcept :
		BitmapSource{ bitmap }
	{ }

	WICBitmap::WICBitmap(
		const SizeU size,
		const WICPixelFormatGUID& pixelFormat,
		CreateCacheOption cacheOption)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto hr = factory->CreateBitmap(
			size.cx, size.cy, pixelFormat,
			static_cast<WICBitmapCreateCacheOption>(cacheOption),
			GetAddress());
		ThrowFailed(hr);
	}

	WICBitmap::WICBitmap(const HBITMAP hBitmap, AlphaChannelOption alphaOption, const HPALETTE hPalette)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto hr = factory->CreateBitmapFromHBITMAP(
			hBitmap, hPalette,
			static_cast<WICBitmapAlphaChannelOption>(alphaOption),
			GetAddress());
		ThrowFailed(hr);
	}

	WICBitmap::WICBitmap(const HICON hIcon)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto hr = factory->CreateBitmapFromHICON(hIcon, GetAddress());
		ThrowFailed(hr);
	}

	WICBitmap::WICBitmap(BitmapSource<> source, CreateCacheOption cacheOption)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto hr = factory->CreateBitmapFromSource(
			source.GetRaw(),
			static_cast<WICBitmapCreateCacheOption>(cacheOption),
			GetAddress());
		ThrowFailed(hr);
	}

	WICBitmap::WICBitmap(BitmapSource<> source, RectU rect)
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		auto size = rect.Size();

		const auto hr = factory->CreateBitmapFromSourceRect(
			source.GetRaw(),
			rect.left, rect.right, size.cx, size.cy,
			GetAddress());
		ThrowFailed(hr);
	}

	auto WICBitmap::Lock(const RectI rect, LockFlags flags) const noexcept -> WICBitmapLock
	{
		const WICRect wicRect = rect;
		ComPtr<IWICBitmapLock> lock;

		const auto hr = Get()->Lock(&wicRect, static_cast<DWORD>(flags), &lock);
		ThrowFailed(hr);

		return lock;
	}

	auto WICBitmap::SetPalette(Palette palette) noexcept -> void
	{
		const auto hr = Get()->SetPalette(palette.GetRaw());
		ThrowFailed(hr);
	}

	auto WICBitmap::SetResolution(const double dpiX, const double dpiY) noexcept -> void
	{
		const auto hr = this->Get()->SetResolution(dpiX, dpiY);
		ThrowFailed(hr);
	}
}

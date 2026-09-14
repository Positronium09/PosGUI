module;
#include <wincodec.h>

module PGUI.UI.Imaging.WICBitmap;

import std;

import PGUI.ComPtr;
import PGUI.Factories;
import PGUI.ErrorHandling;
import PGUI.UI.Imaging.BitmapSource;

namespace PGUI::UI::Imaging
{
	WICBitmap::WICBitmap(const ComPtr<IWICBitmap>& bitmap) noexcept :
		BitmapSource{ bitmap }
	{ }

	auto WICBitmap::CreateFromHIcon(const HICON hIcon) noexcept -> Result<WICBitmap>
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		ComPtr<IWICBitmap> bitmap;
		if (const auto error = Error{ factory->CreateBitmapFromHICON(hIcon, bitmap.put()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return WICBitmap{ bitmap };
	}

	auto WICBitmap::CreateFromSource(
		const BitmapSource<>& source,
		const CreateCacheOption cacheOption) noexcept -> Result<WICBitmap>
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		ComPtr<IWICBitmap> bitmap;
		if (const auto error = Error{
				factory->CreateBitmapFromSource(
					source.GetRaw(),
					static_cast<WICBitmapCreateCacheOption>(cacheOption),
					bitmap.put())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return WICBitmap{ bitmap };
	}

	auto WICBitmap::CreateFromSource(
		const BitmapSource<>& source,
		const RectU rect) noexcept -> Result<WICBitmap>
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto size = rect.Size();

		ComPtr<IWICBitmap> bitmap;
		if (auto error = Error{
				factory->CreateBitmapFromSourceRect(
					source.GetRaw(),
					rect.left, rect.top, size.cx, size.cy,
					bitmap.put())
			};
			error.IsFailure())
		{
			error.AddDetail(L"rect", std::format(L"{}", rect));
			return Unexpected{ error };
		}

		return WICBitmap{ bitmap };
	}

	auto WICBitmap::Create(
		const SizeU size,
		const WICPixelFormatGUID& pixelFormat,
		const CreateCacheOption cacheOption) noexcept -> Result<WICBitmap>
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		ComPtr<IWICBitmap> bitmap;
		if (auto error = Error{
				factory->CreateBitmap(
					size.cx, size.cy, pixelFormat,
					static_cast<WICBitmapCreateCacheOption>(cacheOption),
					bitmap.put())
			};
			error.IsFailure())
		{
			error.AddDetail(L"size", std::format(L"{}", size));
			return Unexpected{ error };
		}

		return WICBitmap{ bitmap };
	}

	auto WICBitmap::CreateFromHBitmap(
		const HBITMAP hBitmap,
		const AlphaChannelOption alphaOption,
		const HPALETTE hPalette) noexcept -> Result<WICBitmap>
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		ComPtr<IWICBitmap> bitmap;
		if (const auto error = Error{
				factory->CreateBitmapFromHBITMAP(
					hBitmap, hPalette,
					static_cast<WICBitmapAlphaChannelOption>(alphaOption),
					bitmap.put())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return WICBitmap{ bitmap };
	}

	auto WICBitmap::Lock(const RectI rect, LockFlags flags) const noexcept -> Result<WICBitmapLock>
	{
		const WICRect wicRect = rect;
		ComPtr<IWICBitmapLock> lock;

		if (const auto hr = Get()->Lock(&wicRect, static_cast<DWORD>(flags), &lock);
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Rect", std::format(L"{}", rect));
			Logger::Error(error, L"Failed to lock WICBitmap");
			return Unexpected{ error };
		}

		return lock;
	}

	auto WICBitmap::SetPalette(Palette palette) noexcept -> Result<void>
	{
		Error error{
			Get()->SetPalette(palette.GetRaw())
		};
		LogIfFailed(error, L"Failed to set palette for WICBitmap");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto WICBitmap::SetResolution(const double dpiX, const double dpiY) noexcept -> Result<void>
	{
		Error error{
			Get()->SetResolution(dpiX, dpiY)
		};
		if (error.IsFailure())
		{
			error
				.AddDetail(L"DPI X", std::to_wstring(dpiX))
				.AddDetail(L"DPI Y", std::to_wstring(dpiY));
			Logger::Error(error, L"Failed to set resolution for WICBitmap");
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto WICBitmap::SetResolution(const Size<double> dpi) noexcept -> Result<void>
	{
		return SetResolution(dpi.cx, dpi.cy);
	}
}

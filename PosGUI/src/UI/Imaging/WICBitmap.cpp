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

	WICBitmap::WICBitmap(
		const SizeU size,
		const WICPixelFormatGUID& pixelFormat,
		CreateCacheOption cacheOption) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		if (const auto hr = factory->CreateBitmap(
			size.cx, size.cy, pixelFormat,
			static_cast<WICBitmapCreateCacheOption>(cacheOption),
			Put());
			FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              .AddDetail(L"Size", std::format(L"{}", size)),
			              L"Failed to create WICBitmap");
		}
	}

	WICBitmap::WICBitmap(
		const HBITMAP hBitmap,
		AlphaChannelOption alphaOption,
		const HPALETTE hPalette) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		if (const auto hr = factory->CreateBitmapFromHBITMAP(
			hBitmap, hPalette,
			static_cast<WICBitmapAlphaChannelOption>(alphaOption),
			Put());
			FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create WICBitmap from HBITMAP");
		}
	}

	WICBitmap::WICBitmap(const HICON hIcon) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreateBitmapFromHICON(hIcon, Put());
			FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create WICBitmap from HICON");
		}
	}

	WICBitmap::WICBitmap(BitmapSource<> source, CreateCacheOption cacheOption) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		if (const auto hr = factory->CreateBitmapFromSource(
			source.GetRaw(),
			static_cast<WICBitmapCreateCacheOption>(cacheOption),
			Put());
			FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create WICBitmap from BitmapSource");
		}
	}

	WICBitmap::WICBitmap(BitmapSource<> source, const RectU rect) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto size = rect.Size();

		if (const auto hr = factory->CreateBitmapFromSourceRect(
			source.GetRaw(),
			rect.left, rect.right, size.cx, size.cy,
			Put());
			FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create WICBitmap from BitmapSource");
		}
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

	auto WICBitmap::SetPalette(Palette palette) noexcept -> Error
	{
		Error error{
			Get()->SetPalette(palette.GetRaw())
		};
		LogIfFailed(error, L"Failed to set palette for WICBitmap");
		return error;
	}

	auto WICBitmap::SetResolution(const double dpiX, const double dpiY) noexcept -> Error
	{
		Error error{
			Get()->SetResolution(dpiX, dpiY)
		};
		error
			.AddDetail(L"DPI X", std::to_wstring(dpiX))
			.AddDetail(L"DPI Y", std::to_wstring(dpiY));
		LogIfFailed(error, L"Failed to set resolution for WICBitmap");
		return error;
	}

	auto WICBitmap::SetResolution(const Size<double> dpi) noexcept -> Error
	{
		return SetResolution(dpi.cx, dpi.cy);
	}
}

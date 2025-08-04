module;
#include <wincodec.h>
#include <wrl.h>

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
			GetAddress());
			FAILED(hr))
		{
			Logger::Error(
				L"Failed to create WICBitmap {}",
				Error{ hr }
				.AddDetail(L"Size", std::format(L"{}", size))
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
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
			GetAddress());
			FAILED(hr))
		{
			Logger::Error(
				L"Failed to create WICBitmap from HBITMAP {}",
				Error{ hr }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
		}
	}

	WICBitmap::WICBitmap(const HICON hIcon) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();

		if (const auto hr = factory->CreateBitmapFromHICON(hIcon, GetAddress());
			FAILED(hr))
		{
			Logger::Error(
				L"Failed to create WICBitmap from HICON {}",
				Error{ hr }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
		}
	}

	WICBitmap::WICBitmap(BitmapSource<> source, CreateCacheOption cacheOption) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		if (const auto hr = factory->CreateBitmapFromSource(
			source.GetRaw(),
			static_cast<WICBitmapCreateCacheOption>(cacheOption),
			GetAddress());
			FAILED(hr))
		{
			Logger::Error(
				L"Failed to create WICBitmap from BitmapSource {}",
				Error{ hr }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
		}
	}

	WICBitmap::WICBitmap(BitmapSource<> source, const RectU rect) noexcept
	{
		const auto& factory = Factories::WICFactory::GetFactory();
		const auto size = rect.Size();

		if (const auto hr = factory->CreateBitmapFromSourceRect(
			source.GetRaw(),
			rect.left, rect.right, size.cx, size.cy,
			GetAddress());
			FAILED(hr))
		{
			Logger::Error(
				L"Failed to create WICBitmap from BitmapSource {}",
				Error{ hr }
				.AddTag(ErrorTags::Creation)
				.AddTag(ErrorTags::Imaging));
		}
	}

	auto WICBitmap::Lock(const RectI rect, LockFlags flags) const noexcept -> Result<WICBitmapLock>
	{
		const WICRect wicRect = rect;
		ComPtr<IWICBitmapLock> lock;

		if (const auto hr = Get()->Lock(&wicRect, static_cast<DWORD>(flags), &lock);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddDetail(L"Rect", std::format(L"{}", rect))
				.AddTag(ErrorTags::Imaging)
			};
		}

		return lock;
	}

	auto WICBitmap::SetPalette(Palette palette) noexcept -> Error
	{
		return Error{
			Get()->SetPalette(palette.GetRaw())
		}.AddTag(ErrorTags::Imaging);
	}

	auto WICBitmap::SetResolution(const double dpiX, const double dpiY) noexcept -> Error
	{
		return Error{
			Get()->SetResolution(dpiX, dpiY)
		}.AddTag(ErrorTags::Imaging);
	}
}

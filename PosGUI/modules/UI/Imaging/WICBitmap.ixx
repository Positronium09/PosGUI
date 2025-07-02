module;
#include <wincodec.h>

export module PGUI.UI.Imaging.WICBitmap;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;
import PGUI.UI.Imaging.WICBitmapLock;

export namespace PGUI::UI::Imaging
{
	enum class CreateCacheOption
	{
		CacheOnDemand = WICBitmapCacheOnDemand,
		CacheOnLoad = WICBitmapCacheOnLoad,
		NoCache = WICBitmapNoCache
	};

	enum class AlphaChannelOption
	{
		UseAlpha = WICBitmapUseAlpha,
		UsePremultipliedAlpha = WICBitmapUsePremultipliedAlpha,
		Ignore = WICBitmapIgnoreAlpha
	};

	enum class LockFlags
	{
		Read = WICBitmapLockRead,
		Write = WICBitmapLockWrite,
		ReadWrite = WICBitmapLockRead | WICBitmapLockWrite
	};

	class WICBitmap : public BitmapSource<IWICBitmap>
	{
		public:
		WICBitmap() noexcept = default;

		explicit(false) WICBitmap(ComPtr<IWICBitmap> bitmap) noexcept;

		WICBitmap(SizeU size, const WICPixelFormatGUID& pixelFormat, CreateCacheOption cacheOption);

		WICBitmap(HBITMAP hBitmap, AlphaChannelOption alphaOption, HPALETTE hPalette = nullptr);

		explicit WICBitmap(HICON hIcon);

		WICBitmap(BitmapSource<> source, CreateCacheOption cacheOption);

		WICBitmap(BitmapSource<> source, RectU rect);

		[[nodiscard]] auto Lock(RectI rect, LockFlags flags) const noexcept -> WICBitmapLock;

		auto SetPalette(Palette palette) noexcept -> void;

		auto SetResolution(double dpiX, double dpiY) noexcept -> void;
	};
}

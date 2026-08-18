module;
#include <wincodec.h>

export module PGUI.UI.Imaging.WICBitmap;

import PGUI.Shape;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.Imaging.Palette;
import PGUI.UI.Imaging.BitmapSource;
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

		explicit(false) WICBitmap(const ComPtr<IWICBitmap>& bitmap) noexcept;

		WICBitmap(SizeU size, 
			const WICPixelFormatGUID& pixelFormat, 
			CreateCacheOption cacheOption) noexcept;

		WICBitmap(HBITMAP hBitmap, AlphaChannelOption alphaOption, HPALETTE hPalette = nullptr) noexcept;

		explicit WICBitmap(HICON hIcon) noexcept;

		WICBitmap(BitmapSource<> source, CreateCacheOption cacheOption) noexcept;

		WICBitmap(BitmapSource<> source, RectU rect) noexcept;

		[[nodiscard]] auto Lock(RectI rect, LockFlags flags) const noexcept -> Result<WICBitmapLock>;

		[[nodiscard]] auto SetPalette(Palette palette) noexcept -> Result<void>;

		[[nodiscard]] auto SetResolution(double dpiX, double dpiY) noexcept -> Result<void>;
		[[nodiscard]] auto SetResolution(Size<double> dpi) noexcept -> Result<void>;
	};
}

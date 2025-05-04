module;
#include <span>
#include <wrl.h>
#include <wincodec.h>

export module PGUI.UI.Imaging.WICBitmapLock;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;

export namespace PGUI::UI::Imaging
{
	class WICBitmapLock : public ComPtrHolder<IWICBitmapLock>
	{
		public:
		WICBitmapLock() noexcept = default;
		WICBitmapLock(ComPtr<IWICBitmapLock> bitmapLock) noexcept;

		[[nodiscard]] auto GetSize() const -> SizeU;
		[[nodiscard]] auto GetStride() const -> UINT;
		[[nodiscard]] auto GetDataPointer() const -> std::span<BYTE>;
		[[nodiscard]] auto GetPixelFormat() const -> WICPixelFormatGUID;
	};
}

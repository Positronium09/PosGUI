module;
#include <wincodec.h>

export module PGUI.UI.Imaging.WICBitmapLock;

import std;

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

		explicit(false) WICBitmapLock(const ComPtr<IWICBitmapLock>& bitmapLock) noexcept;

		[[nodiscard]] auto GetSize() const -> SizeU;

		[[nodiscard]] auto GetStride() const -> UINT;

		[[nodiscard]] auto GetDataPointer() const -> std::span<BYTE>;

		[[nodiscard]] auto GetPixelFormat() const -> WICPixelFormatGUID;
	};
}

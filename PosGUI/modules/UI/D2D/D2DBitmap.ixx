module;
#include <wrl.h>
#include <d2d1_3.h>

export module PGUI.UI.D2D.D2DBitmap;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.D2D.D2DImage;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	class D2DBitmap : public D2DImage<ID2D1Bitmap1>
	{
		public:
		D2DBitmap() noexcept = default;
		D2DBitmap(ComPtr<ID2D1Bitmap1> bitmap) noexcept;

		[[nodiscard]] auto GetSize() const noexcept -> SizeF;
		[[nodiscard]] auto GetPixelSize() const noexcept -> SizeU;
		[[nodiscard]] auto GetDpi() const noexcept -> std::pair<float, float>;
		[[nodiscard]] auto GetPixelFormat() const noexcept -> D2D1_PIXEL_FORMAT;
		[[nodiscard]] auto GetBitmapOptions() const noexcept -> D2D1_BITMAP_OPTIONS;

		[[nodiscard]] auto GetSurface() const noexcept -> ComPtr<IDXGISurface>;

		[[nodiscard]] auto Map(D2D1_MAP_OPTIONS options) const -> MappedRect;
		void Unmap() const;

		void CopyFromBitmap(D2DBitmap bitmap, 
			std::optional<PointU> destPoint = std::nullopt, 
			std::optional<RectU> srcRect = std::nullopt);
		void CopyFromMemory(const void* source, UINT32 pitch, 
			std::optional<RectU> destRect = std::nullopt);

		//TODO GetColorContext
	};
}

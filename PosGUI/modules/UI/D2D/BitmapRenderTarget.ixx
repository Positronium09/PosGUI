module;
#include <optional>
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.BitmapRenderTarget;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Exceptions;
import PGUI.UI.D2D.RenderTarget;
import PGUI.UI.D2D.D2DBitmap;

export namespace PGUI::UI::D2D
{
	class BitmapRenderTarget : public RenderTarget<ID2D1BitmapRenderTarget>
	{
		public:
		constexpr BitmapRenderTarget() noexcept = default;
		BitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> renderTarget) noexcept :
			RenderTarget<ID2D1BitmapRenderTarget>{ renderTarget }
		{
		}
		explicit BitmapRenderTarget(RenderTarget<> renderTarget)
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(&bitmapRenderTarget); ThrowFailed(hr);

			Set(bitmapRenderTarget);
		}
		BitmapRenderTarget(RenderTarget<> renderTarget, SizeF size, std::optional<SizeU> pixelSize = std::nullopt)
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;

			if (pixelSize.has_value())
			{
				auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(size, pixelSize.value(), &bitmapRenderTarget); ThrowFailed(hr);
				Set(bitmapRenderTarget);
				return;
			}

			auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(size, &bitmapRenderTarget); ThrowFailed(hr);

			Set(bitmapRenderTarget);
		}
		BitmapRenderTarget(RenderTarget<> renderTarget, SizeF size, SizeU pixelSize,
			D2D1_PIXEL_FORMAT pixelFormat = { .format = DXGI_FORMAT_UNKNOWN, .alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED },
			D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS compatibleRenderTargetOptions = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(size, pixelSize, 
				pixelFormat, compatibleRenderTargetOptions, &bitmapRenderTarget); ThrowFailed(hr);
			Set(bitmapRenderTarget);
		}
		[[nodiscard]] auto GetBitmap() const noexcept -> D2DBitmap
		{
			ComPtr<ID2D1Bitmap> bitmap;
			auto hr = Get()->GetBitmap(&bitmap); ThrowFailed(hr);
			
			ComPtr<ID2D1Bitmap1> bitmap1;
			hr = bitmap.As(&bitmap1); ThrowFailed(hr);

			return D2DBitmap{ bitmap1 };
		}
	};
}

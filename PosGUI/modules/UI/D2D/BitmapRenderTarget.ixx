module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.BitmapRenderTarget;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.RenderTarget;
import PGUI.UI.D2D.D2DBitmap;

export namespace PGUI::UI::D2D
{
	class BitmapRenderTarget : public RenderTarget<ID2D1BitmapRenderTarget>
	{
		public:
		constexpr BitmapRenderTarget() noexcept = default;

		explicit(false) BitmapRenderTarget(const ComPtr<ID2D1BitmapRenderTarget>& renderTarget) noexcept :
			RenderTarget{ renderTarget }
		{
		}

		explicit BitmapRenderTarget(RenderTarget<> renderTarget) noexcept
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			if (const auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(&bitmapRenderTarget);
				FAILED(hr))
			{
				Logger::Error(
					Error{ hr },
					L"Failed to create compatible render target");
				return;
			}

			Set(bitmapRenderTarget);
		}

		BitmapRenderTarget(RenderTarget<> renderTarget, const SizeF size, const std::optional<SizeU> pixelSize = std::nullopt) noexcept
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;

			if (pixelSize.has_value())
			{
				const auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(
					size, pixelSize.value(), &bitmapRenderTarget);

				if (FAILED(hr))
				{
					Logger::Error(
						Error{ hr }
						.AddDetail(L"Size", std::format(L"{}", size)),
						L"Failed to create compatible render target");
					return;
				}

				Set(bitmapRenderTarget);
				return;
			}


			if (const auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(size, &bitmapRenderTarget);
				FAILED(hr))
			{
				Logger::Error(
					Error{ hr },
					L"Failed to create compatible render target");
				return;
			}

			Set(bitmapRenderTarget);
		}

		BitmapRenderTarget(RenderTarget<> renderTarget, const SizeF size, const SizeU pixelSize,
			const D2D1_PIXEL_FORMAT pixelFormat = {
				.format = DXGI_FORMAT_UNKNOWN,
				.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED
			},
			const D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS compatibleRenderTargetOptions =
			D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE) noexcept
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			const auto hr = renderTarget.Get()->CreateCompatibleRenderTarget(
				size, pixelSize,
				pixelFormat, compatibleRenderTargetOptions,
				&bitmapRenderTarget);
			if (FAILED(hr))
			{
				Logger::Error(
					Error{ hr }
					.AddDetail(L"Size", std::format(L"{}", size))
					.AddDetail(L"PixelSize", std::format(L"{}", pixelSize)),
					L"Failed to create compatible render target");
				return;
			}
			Set(bitmapRenderTarget);
		}

		[[nodiscard]] auto GetBitmap() const noexcept -> Result<D2DBitmap>
		{
			ComPtr<ID2D1Bitmap> bitmap;
			
			if (auto hr = Get()->GetBitmap(&bitmap); 
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to get bitmap");
				return Unexpected{ error };
			}

			auto bitmap1 = bitmap.try_query<ID2D1Bitmap1>();
			if (bitmap1.get() == nullptr)
			{
				Error error{ E_NOINTERFACE };
				Logger::Error(error, L"Failed to cast bitmap to ID2D1Bitmap1 interface");
				return Unexpected{ error };
			}

			return D2DBitmap{ bitmap1 };
		}
	};
}

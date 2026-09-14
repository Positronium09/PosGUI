module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.BitmapRenderTarget;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
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

		[[nodiscard]] static auto Create(
			const RenderTarget<>& renderTarget) noexcept -> Result<BitmapRenderTarget>
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			if (const auto error = Error{
					renderTarget.Get()->CreateCompatibleRenderTarget(bitmapRenderTarget.put())
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return BitmapRenderTarget{ bitmapRenderTarget };
		}

		[[nodiscard]] static auto Create(
			const RenderTarget<>& renderTarget,
			const SizeF size,
			const std::optional<SizeU> pixelSize = std::nullopt) noexcept -> Result<BitmapRenderTarget>
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			if (auto error = Error{
					pixelSize.has_value()
						? renderTarget.Get()->CreateCompatibleRenderTarget(
							size, pixelSize.value(), bitmapRenderTarget.put())
						: renderTarget.Get()->CreateCompatibleRenderTarget(
							size, bitmapRenderTarget.put())
				};
				error.IsFailure())
			{
				error.AddDetail(L"size", std::format(L"{}", size));
				return Unexpected{ error };
			}

			return BitmapRenderTarget{ bitmapRenderTarget };
		}

		[[nodiscard]] static auto Create(
			const RenderTarget<>& renderTarget,
			const SizeF size,
			const SizeU pixelSize,
			const D2D1_PIXEL_FORMAT pixelFormat = {
				.format = DXGI_FORMAT_UNKNOWN,
				.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED
			},
			const D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS compatibleRenderTargetOptions =
			D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE) noexcept -> Result<BitmapRenderTarget>
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;
			if (auto error = Error{
					renderTarget.Get()->CreateCompatibleRenderTarget(
						size, pixelSize,
						pixelFormat, compatibleRenderTargetOptions,
						bitmapRenderTarget.put())
				};
				error.IsFailure())
			{
				error
					.AddDetail(L"size", std::format(L"{}", size))
					.AddDetail(L"pixelSize", std::format(L"{}", pixelSize));
				return Unexpected{ error };
			}

			return BitmapRenderTarget{ bitmapRenderTarget };
		}

		[[nodiscard]] auto GetBitmap() const noexcept -> Result<D2DBitmap>
		{
			ComPtr<ID2D1Bitmap> bitmap;
			
			if (const auto hr = Get()->GetBitmap(bitmap.put()); 
				FAILED(hr))
			{
				return Unexpected{ Error{ hr } };
			}

			const auto bitmap1 = bitmap.try_query<ID2D1Bitmap1>();
			if (bitmap1.get() == nullptr)
			{
				return Unexpected{
					Error{
						SystemErrorCode::InterfaceNotSupported
					}.SetCustomMessage(L"Failed to cast to ID2D1Bitmap1")
				};
			}

			return D2DBitmap{ bitmap1 };
		}
	};
}

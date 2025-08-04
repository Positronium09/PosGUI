module;
#include <d2d1_3.h>
#include <wrl.h>

module PGUI.UI.D2D.D2DBitmap;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DImage;
import PGUI.UI.D2D.RenderTarget;

namespace PGUI::UI::D2D
{
	D2DBitmap::D2DBitmap(const ComPtr<ID2D1Bitmap1>& bitmap) noexcept :
		D2DImage{ bitmap }
	{ }

	auto D2DBitmap::GetSize() const noexcept -> SizeF
	{
		return Get()->GetSize();
	}

	auto D2DBitmap::GetPixelSize() const noexcept -> SizeU
	{
		return Get()->GetPixelSize();
	}

	auto D2DBitmap::GetDpi() const noexcept -> std::pair<float, float>
	{
		auto dpiX = 0.0F;
		auto dpiY = 0.0F;
		Get()->GetDpi(&dpiX, &dpiY);

		return std::pair{ dpiX, dpiY };
	}

	auto D2DBitmap::GetPixelFormat() const noexcept -> D2D1_PIXEL_FORMAT
	{
		return Get()->GetPixelFormat();
	}

	auto D2DBitmap::GetBitmapOptions() const noexcept -> D2D1_BITMAP_OPTIONS
	{
		return Get()->GetOptions();
	}

	auto D2DBitmap::GetSurface() const noexcept -> Result<ComPtr<IDXGISurface>>
	{
		ComPtr<IDXGISurface> surface;
		if (const auto hr = Get()->GetSurface(surface.GetAddressOf());
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::D2D)
			};
		}

		return surface;
	}

	auto D2DBitmap::Map(const MapOptions options) const noexcept -> Result<MappedRect>
	{
		D2D1_MAPPED_RECT mappedRect;

		if (const auto hr = Get()->Map(
				static_cast<D2D1_MAP_OPTIONS>(options), &mappedRect);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::D2D)
			};
		}

		return MappedRect{ mappedRect, GetPixelSize().cy };
	}

	auto D2DBitmap::Unmap() const noexcept -> Error
	{
		return Error{
			Get()->Unmap()
		}.AddTag(ErrorTags::D2D);
	}

	auto D2DBitmap::CopyFromBitmap(
		D2DBitmap bitmap,
		std::optional<PointU> destPoint,
		std::optional<RectU> srcRect) noexcept -> Error
	{
		const D2D1_RECT_U* srcRectPtr = nullptr;
		if (srcRect.has_value())
		{
			srcRectPtr = std::bit_cast<D2D1_RECT_U*>(&srcRect.value());
		}
		const D2D1_POINT_2U* destPointPtr = nullptr;
		if (destPoint.has_value())
		{
			destPointPtr = std::bit_cast<D2D1_POINT_2U*>(&destPoint.value());
		}

		return Error{
			Get()->CopyFromBitmap(
				destPointPtr, bitmap.GetRaw(), srcRectPtr)
		}.AddTag(ErrorTags::D2D);
	}

	auto D2DBitmap::CopyFromMemory(
		const void* source, const UINT32 pitch,
		std::optional<RectU> destRect) noexcept -> Error
	{
		const D2D1_RECT_U* destRectPtr = nullptr;
		if (destRect.has_value())
		{
			destRectPtr = std::bit_cast<D2D1_RECT_U*>(&destRect.value());
		}

		return Error{
			Get()->CopyFromMemory(destRectPtr, source, pitch)
		}.AddTag(ErrorTags::D2D);
	}
}

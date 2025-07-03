module;
#include <d2d1_3.h>
#include <wrl.h>

module PGUI.UI.D2D.D2DBitmap;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Exceptions;
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

	auto D2DBitmap::GetSurface() const noexcept -> ComPtr<IDXGISurface>
	{
		ComPtr<IDXGISurface> surface;
		const auto hr = Get()->GetSurface(surface.GetAddressOf());
		ThrowFailed(hr);

		return surface;
	}

	auto D2DBitmap::Map(const D2D1_MAP_OPTIONS options) const -> MappedRect
	{
		D2D1_MAPPED_RECT mappedRect;

		const auto hr = Get()->Map(options, &mappedRect);
		ThrowFailed(hr);

		return MappedRect{ mappedRect, GetPixelSize().cy };
	}

	auto D2DBitmap::Unmap() const -> void
	{
		const auto hr = Get()->Unmap();
		ThrowFailed(hr);
	}

	auto D2DBitmap::CopyFromBitmap(
		D2DBitmap bitmap, std::optional<PointU> destPoint, std::optional<RectU> srcRect) -> void
	{
		D2D1_RECT_U* srcRectPtr = nullptr;
		if (srcRect.has_value())
		{
			srcRectPtr = std::bit_cast<D2D1_RECT_U*>(&srcRect.value());
		}
		D2D1_POINT_2U* destPointPtr = nullptr;
		if (destPoint.has_value())
		{
			destPointPtr = std::bit_cast<D2D1_POINT_2U*>(&destPoint.value());
		}

		const auto hr = Get()->CopyFromBitmap(
			destPointPtr, bitmap.GetRaw(), srcRectPtr);
		ThrowFailed(hr);
	}

	auto D2DBitmap::CopyFromMemory(
		const void* source, const UINT32 pitch, std::optional<RectU> destRect) -> void
	{
		D2D1_RECT_U* destRectPtr = nullptr;
		if (destRect.has_value())
		{
			destRectPtr = std::bit_cast<D2D1_RECT_U*>(&destRect.value());
		}

		const auto hr = Get()->CopyFromMemory(
			destRectPtr, source, pitch);
		ThrowFailed(hr);
	}
}

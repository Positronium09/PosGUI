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

		return std::make_pair(dpiX, dpiY);
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
			Error error{ hr };
			Logger::Error(error, L"Cannot get surface");
			return Unexpected{ error };
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
			Error error{ hr };
			Logger::Error(error, L"Cannot map bitmap");
			return Unexpected{ error };
		}

		return MappedRect{ mappedRect, GetPixelSize().cy };
	}

	auto D2DBitmap::Unmap() const noexcept -> Error
	{
		const auto hr = Get()->Unmap();
		Error error{ hr };
		LogIfFailed(error, L"Cannot Unmap");
		return error;
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

		Error error{
			Get()->CopyFromBitmap(
				destPointPtr, bitmap.GetRaw(), srcRectPtr)
		};
		error
			.AddDetail(L"Destination Point",
			           std::format(L"{}",
			                       destPoint.value_or(PointU{
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1)
			                       })))
			.AddDetail(L"Source Rect",
			           std::format(L"{}",
			                       srcRect.value_or(RectU{
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1)
			                       })));
		LogIfFailed(error, L"Cannot copy from bitmap");
		return error;
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

		Error error{
			Get()->CopyFromMemory(destRectPtr, source, pitch)
		};
		error
			.AddDetail(L"Source pointer", std::format(L"{:p}", source))
			.AddDetail(L"Pitch", std::to_wstring(pitch))
			.AddDetail(L"Destination Rect",
			           std::format(L"{}", destRect.value_or(RectU{
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1),
				                       static_cast<std::uint32_t>(-1)
			                       })));
		LogIfFailed(error, L"Cannot copy from memory");
		return error;
	}
}

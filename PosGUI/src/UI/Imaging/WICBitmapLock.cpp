module;
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.WICBitmapLock;

import std;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;

namespace PGUI::UI::Imaging
{
	WICBitmapLock::WICBitmapLock(const ComPtr<IWICBitmapLock>& bitmapLock) noexcept :
		ComPtrHolder{ bitmapLock }
	{ }

	auto WICBitmapLock::GetSize() const noexcept -> Result<SizeU>
	{
		SizeU size;

		if (const auto hr = Get()->GetSize(&size.cx, &size.cy);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get size");
			return Unexpected{ error };
		}

		return size;
	}

	auto WICBitmapLock::GetStride() const noexcept -> Result<UINT>
	{
		UINT stride;

		if (const auto hr = Get()->GetStride(&stride);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get stride");
			return Unexpected{ error };
		}

		return stride;
	}

	auto WICBitmapLock::GetDataPointer() const noexcept -> Result<std::span<BYTE>>
	{
		UINT bufferSize;
		BYTE* buffer = nullptr;

		if (const auto hr = Get()->GetDataPointer(&bufferSize, &buffer);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get data pointer");
			return Unexpected{ error };
		}

		return std::span(buffer, bufferSize);
	}

	auto WICBitmapLock::GetPixelFormat() const noexcept -> Result<WICPixelFormatGUID>
	{
		WICPixelFormatGUID pixelFormat;

		if (const auto hr = Get()->GetPixelFormat(&pixelFormat);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get pixel format");
			return Unexpected{ error };
		}

		return pixelFormat;
	}
}

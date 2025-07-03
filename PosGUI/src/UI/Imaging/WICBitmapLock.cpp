module;
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.WICBitmapLock;

import std;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.Palette;

namespace PGUI::UI::Imaging
{
	WICBitmapLock::WICBitmapLock(const ComPtr<IWICBitmapLock>& bitmapLock) noexcept :
		ComPtrHolder{ bitmapLock }
	{ }

	auto WICBitmapLock::GetSize() const -> SizeU
	{
		SizeU size;
		const auto hr = Get()->GetSize(&size.cx, &size.cy);
		ThrowFailed(hr);

		return size;
	}

	auto WICBitmapLock::GetStride() const -> UINT
	{
		UINT stride;
		const auto hr = Get()->GetStride(&stride);
		ThrowFailed(hr);

		return stride;
	}

	auto WICBitmapLock::GetDataPointer() const -> std::span<BYTE>
	{
		UINT bufferSize;
		BYTE* buffer = nullptr;
		const auto hr = Get()->GetDataPointer(&bufferSize, &buffer);
		ThrowFailed(hr);

		return std::span<BYTE>(buffer, bufferSize);
	}

	auto WICBitmapLock::GetPixelFormat() const -> WICPixelFormatGUID
	{
		WICPixelFormatGUID pixelFormat;
		const auto hr = Get()->GetPixelFormat(&pixelFormat);
		ThrowFailed(hr);
		return pixelFormat;
	}
}

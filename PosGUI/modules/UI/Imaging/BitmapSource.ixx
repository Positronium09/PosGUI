module;
#include <concepts>
#include <wrl.h>
#include <wincodec.h>
#include <vector>
#include <utility>


export module PGUI.UI.Imaging.BitmapSource;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Logging;
import PGUI.Exceptions;

export namespace PGUI::UI::Imaging
{
	template <typename Interface = IWICBitmapSource>
	class BitmapSource : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, IWICBitmapSource>, "Interface must be derived from IWICBitmapSource");

		public:
		BitmapSource() noexcept = default;
		BitmapSource(ComPtr<Interface> bitmapSource) noexcept : 
			ComPtrHolder<Interface>{ bitmapSource }
		{
		}

		[[nodiscard]] auto GetSize() const
		{
			UINT width{ };
			UINT height{ };
			auto hr = this->Get()->GetSize(&width, &height); ThrowFailed(hr);

			return SizeU{ width, height };
		}
		[[nodiscard]] auto GetPixelFormat() const
		{
			WICPixelFormatGUID format{ };
			auto hr =this->Get()->GetPixelFormat(&format); ThrowFailed(hr);

			return format;
		}
		[[nodiscard]] auto GetResolution() const
		{
			double dpiX{ };
			double dpiY{ };
			auto hr =this->Get()->GetResolution(&dpiX, &dpiY); ThrowFailed(hr);

			return std::pair{ dpiX, dpiY };
		}
		[[nodiscard]] auto CopyPixels(UINT stride) const
		{
			auto size = this->GetSize();
			RectI rect = { 0, 0, size.cx, size.cy };
			std::vector<BYTE> buffer(rect.Area() * stride);
			auto hr = this->Get()->CopyPixels(nullptr, stride,
				static_cast<UINT>(buffer.size()), buffer.data()); ThrowFailed(hr);

			return buffer;
		}
		[[nodiscard]] auto CopyPixels(RectI rect, UINT stride) const
		{
			std::vector<BYTE> buffer(rect.Area() * stride);
			WICRect rc = rect;
			auto hr =this->Get()->CopyPixels(&rc, stride, 
				static_cast<UINT>(buffer.size()), buffer.data()); ThrowFailed(hr);

			return buffer;
		}
		[[nodiscard]] auto CopyPixels(RectI rect, UINT stride, UINT bufferSize) const
		{
			std::vector<BYTE> buffer(bufferSize);
			WICRect rc = rect;
			auto hr =this->Get()->CopyPixels(&rc, stride, 
				bufferSize, buffer.data()); ThrowFailed(hr);

			return buffer;
		}

		operator BitmapSource<>() const noexcept
		{
			return BitmapSource<>{ this->Get() };
		}
	};
}

module;
#include <wrl.h>
#include <wincodec.h>

export module PGUI.UI.Imaging.BitmapSource;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Logging;
import PGUI.Factories;
import PGUI.Exceptions;

export namespace PGUI::UI::Imaging
{
	enum class InterpolationMode
	{
		NearestNeighbor = WICBitmapInterpolationModeNearestNeighbor,
		Linear = WICBitmapInterpolationModeLinear,
		Cubic = WICBitmapInterpolationModeCubic,
		Fant = WICBitmapInterpolationModeFant,
		HighQualityCubic = WICBitmapInterpolationModeHighQualityCubic
	};

	enum class TransformOptions
	{
		None = WICBitmapTransformRotate0,
		FlipHorizontal = WICBitmapTransformFlipHorizontal,
		FlipVertical = WICBitmapTransformFlipVertical,
		Rotate0 = WICBitmapTransformRotate0,
		Rotate90 = WICBitmapTransformRotate90,
		Rotate180 = WICBitmapTransformRotate180,
		Rotate270 = WICBitmapTransformRotate270
	};

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

	class BitmapSourceScaler : public BitmapSource<IWICBitmapScaler>
	{
		public:
		BitmapSourceScaler() noexcept = default;
		BitmapSourceScaler(BitmapSource<> source, SizeU targetSize, InterpolationMode interpolationMode)
		{
			const auto& factory = Factories::WICFactory::GetFactory();
			auto hr = factory->CreateBitmapScaler(GetAddress()); ThrowFailed(hr);
			hr = Get()->Initialize(source.GetRaw(), targetSize.cx, targetSize.cy,
				static_cast<WICBitmapInterpolationMode>(interpolationMode)); ThrowFailed(hr);
		}
	};

	class BitmapSourceClipper : public BitmapSource<IWICBitmapClipper>
	{
		public:
		BitmapSourceClipper() noexcept = default;
		BitmapSourceClipper(BitmapSource<> source, RectI clipRect)
		{
			const auto& factory = Factories::WICFactory::GetFactory();
			auto hr = factory->CreateBitmapClipper(GetAddress()); ThrowFailed(hr);
			WICRect wicRect = clipRect;
			hr = Get()->Initialize(source.GetRaw(), &wicRect); ThrowFailed(hr);
		}
	};

	class BitmapSourceFlipRotator : public BitmapSource<IWICBitmapFlipRotator>
	{
		public:
		BitmapSourceFlipRotator() noexcept = default;
		BitmapSourceFlipRotator(BitmapSource<> source, TransformOptions transformOptions)
		{
			const auto& factory = Factories::WICFactory::GetFactory();
			auto hr = factory->CreateBitmapFlipRotator(GetAddress()); ThrowFailed(hr);
			hr = Get()->Initialize(source.GetRaw(), static_cast<WICBitmapTransformOptions>(transformOptions)); ThrowFailed(hr);
		}

	};

	class BitmapSourceColorTransform : public BitmapSource<IWICColorTransform>
	{
		public:
		BitmapSourceColorTransform() noexcept = default;
		//TODO BitmapSourceColorTransform(BitmapSource<> source);
	};
}

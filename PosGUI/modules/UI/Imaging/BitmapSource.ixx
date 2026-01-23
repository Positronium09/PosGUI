module;
#include <wincodec.h>

export module PGUI.UI.Imaging.BitmapSource;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Factories;
import PGUI.ErrorHandling;

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
		static_assert(std::derived_from<Interface, IWICBitmapSource>,
			"Interface must be derived from IWICBitmapSource");

		public:
		BitmapSource() noexcept = default;

		explicit(false) BitmapSource(ComPtr<Interface> bitmapSource) noexcept :
			ComPtrHolder<Interface>{ bitmapSource }
		{ }

		[[nodiscard]] auto GetSize() const noexcept -> Result<SizeU>
		{
			UINT width{ };
			UINT height{ };

			if (const auto hr = this->Get()->GetSize(&width, &height);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot get size");
				return Unexpected{ error };
			}

			return SizeU{ width, height };
		}

		[[nodiscard]] auto GetPixelFormat() const noexcept -> Result<WICPixelFormatGUID>
		{
			WICPixelFormatGUID format{ };
			if (const auto hr = this->Get()->GetPixelFormat(&format);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot get pixel format");
				return Unexpected{ error };
			}

			return format;
		}

		[[nodiscard]] auto GetResolution() const noexcept -> Result<std::pair<double, double>>
		{
			double dpiX{ };
			double dpiY{ };
			if (const auto hr = this->Get()->GetResolution(&dpiX, &dpiY);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot get resolution");
				return Unexpected{ error };
			}

			return std::pair{ dpiX, dpiY };
		}

		[[nodiscard]] auto CopyPixels(UINT stride) const noexcept -> Result<std::vector<BYTE>>
		{
			auto size = this->GetSize();
			const RectI rect = { 0, 0, size.cx, size.cy };
			std::vector<BYTE> buffer(rect.Area() * stride);

			if (const auto hr = this->Get()->CopyPixels(nullptr, stride,
				static_cast<UINT>(buffer.size()), buffer.data());
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot copy pixels");
				return Unexpected{ error };
			}

			return buffer;
		}

		[[nodiscard]] auto CopyPixels(const RectI rect, UINT stride) const noexcept -> Result<std::vector<BYTE>>
		{
			std::vector<BYTE> buffer(rect.Area() * stride);
			WICRect rc = rect;

			if (auto hr = this->Get()->CopyPixels(
				&rc, stride,
				static_cast<UINT>(buffer.size()), buffer.data());
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot copy pixels");
				return Unexpected{ error };
			}

			return buffer;
		}

		[[nodiscard]] auto CopyPixels(const RectI rect, UINT stride, UINT bufferSize) const noexcept -> Result<std::vector<BYTE>>
		{
			std::vector<BYTE> buffer(bufferSize);
			WICRect rc = rect;
			if (auto hr = this->Get()->CopyPixels(
				&rc, stride,
				bufferSize, buffer.data());
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot copy pixels");
				return Unexpected{ error };
			}

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

		BitmapSourceScaler(BitmapSource<> source, const SizeU targetSize, InterpolationMode interpolationMode) noexcept
		{
			const auto& factory = Factories::WICFactory::GetFactory();
			auto hr = factory->CreateBitmapScaler(Put());
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to create IWICBitmapScaler");
				return;
			}

			hr = Get()->Initialize(
				source.GetRaw(), targetSize.cx, targetSize.cy,
				static_cast<WICBitmapInterpolationMode>(interpolationMode));
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to initialize IWICBitmapScaler");
			}
		}
	};

	class BitmapSourceClipper : public BitmapSource<IWICBitmapClipper>
	{
		public:
		BitmapSourceClipper() noexcept = default;

		BitmapSourceClipper(BitmapSource<> source, const RectI clipRect) noexcept
		{
			const auto& factory = Factories::WICFactory::GetFactory();
			auto hr = factory->CreateBitmapClipper(Put());
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to create IWICBitmapClipper");
				return;
			}

			const WICRect wicRect = clipRect;
			hr = Get()->Initialize(source.GetRaw(), &wicRect);

			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to initialize IWICBitmapClipper");
			}
		}
	};

	class BitmapSourceFlipRotator : public BitmapSource<IWICBitmapFlipRotator>
	{
		public:
		BitmapSourceFlipRotator() noexcept = default;

		BitmapSourceFlipRotator(BitmapSource<> source, TransformOptions transformOptions) noexcept
		{
			const auto& factory = Factories::WICFactory::GetFactory();
			auto hr = factory->CreateBitmapFlipRotator(Put());
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to create IWICBitmapFlipRotator");
				return;
			}
			
			hr = Get()->Initialize(
				source.GetRaw(),
				static_cast<WICBitmapTransformOptions>(transformOptions));

			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to initialize IWICBitmapFlipRotator");
			}
		}
	};

	class BitmapSourceColorTransform : public BitmapSource<IWICColorTransform>
	{
		public:
		BitmapSourceColorTransform() noexcept = default;

		//TODO BitmapSourceColorTransform(BitmapSource<> source);
	};
}

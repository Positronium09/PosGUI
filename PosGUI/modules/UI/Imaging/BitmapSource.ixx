module;
#include <wincodec.h>

export module PGUI.UI.Imaging.BitmapSource;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
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

	template <std::derived_from<IWICBitmapSource> Interface = IWICBitmapSource>
	class BitmapSource : public ComPtrHolder<Interface>
	{
		public:
		BitmapSource() noexcept = default;

		explicit(false) BitmapSource(const ComPtr<Interface>& bitmapSource) noexcept :
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
			auto sizeResult = this->GetSize();
			if (!sizeResult)
			{
				return Unexpected{ sizeResult.error() };
			}
			const auto size = sizeResult.value();
			std::vector<BYTE> buffer(size.cy * stride);

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

		explicit(false) operator BitmapSource<>() const noexcept
		{
			return BitmapSource<>{ this->Get() };
		}
	};

	class BitmapSourceScaler : public BitmapSource<IWICBitmapScaler>
	{
		public:
		BitmapSourceScaler() noexcept = default;

		explicit(false) BitmapSourceScaler(const ComPtr<IWICBitmapScaler>& scaler) noexcept :
			BitmapSource{ scaler }
		{ }

		[[nodiscard]] static auto Create(
			const BitmapSource<>& source,
			const SizeU targetSize,
			const InterpolationMode interpolationMode) noexcept -> Result<BitmapSourceScaler>
		{
			const auto& factory = Factories::WICFactory::GetFactory();

			ComPtr<IWICBitmapScaler> scaler;
			if (const auto error = Error{ factory->CreateBitmapScaler(scaler.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			if (auto error = Error{
					scaler->Initialize(
						source.GetRaw(), targetSize.cx, targetSize.cy,
						static_cast<WICBitmapInterpolationMode>(interpolationMode))
				};
				error.IsFailure())
			{
				error.AddDetail(L"targetSize", std::format(L"{}", targetSize));
				return Unexpected{ error };
			}

			return BitmapSourceScaler{ scaler };
		}
	};

	class BitmapSourceClipper : public BitmapSource<IWICBitmapClipper>
	{
		public:
		BitmapSourceClipper() noexcept = default;

		explicit(false) BitmapSourceClipper(const ComPtr<IWICBitmapClipper>& clipper) noexcept :
			BitmapSource{ clipper }
		{ }

		[[nodiscard]] static auto Create(
			const BitmapSource<>& source,
			const RectI clipRect) noexcept -> Result<BitmapSourceClipper>
		{
			const auto& factory = Factories::WICFactory::GetFactory();

			ComPtr<IWICBitmapClipper> clipper;
			if (const auto error = Error{ factory->CreateBitmapClipper(clipper.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			const WICRect wicRect = clipRect;
			if (auto error = Error{ clipper->Initialize(source.GetRaw(), &wicRect) };
				error.IsFailure())
			{
				error.AddDetail(L"clipRect", std::format(L"{}", clipRect));
				return Unexpected{ error };
			}

			return BitmapSourceClipper{ clipper };
		}
	};

	class BitmapSourceFlipRotator : public BitmapSource<IWICBitmapFlipRotator>
	{
		public:
		BitmapSourceFlipRotator() noexcept = default;

		explicit(false) BitmapSourceFlipRotator(const ComPtr<IWICBitmapFlipRotator>& flipRotator) noexcept :
			BitmapSource{ flipRotator }
		{ }

		[[nodiscard]] static auto Create(
			const BitmapSource<>& source,
			const TransformOptions transformOptions) noexcept -> Result<BitmapSourceFlipRotator>
		{
			const auto& factory = Factories::WICFactory::GetFactory();

			ComPtr<IWICBitmapFlipRotator> flipRotator;
			if (const auto error = Error{ factory->CreateBitmapFlipRotator(flipRotator.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			if (const auto error = Error{
					flipRotator->Initialize(
						source.GetRaw(),
						static_cast<WICBitmapTransformOptions>(transformOptions))
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return BitmapSourceFlipRotator{ flipRotator };
		}
	};

	class BitmapSourceColorTransform : public BitmapSource<IWICColorTransform>
	{
		public:
		BitmapSourceColorTransform() noexcept = default;

		//TODO BitmapSourceColorTransform(BitmapSource<> source);
	};
}

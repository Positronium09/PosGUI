module;
#include <d2d1_1.h>
#include <wrl.h>

module PGUI.UI.Brush;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.Logging;
import PGUI.Utils;
import PGUI.UI.D2D.D2DBitmap;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.D2D.D2DEnums;

namespace PGUI::UI
{
	SolidBrush::SolidBrush(const ComPtr<ID2D1SolidColorBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	SolidBrush::SolidBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, const RGBA color)
	{
		const auto hr = renderTarget->CreateSolidColorBrush(color, GetAddress());
		ThrowFailed(hr);
	}

	LinearGradientBrush::LinearGradientBrush(const ComPtr<ID2D1LinearGradientBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	LinearGradientBrush::LinearGradientBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		LinearGradient gradient, const std::optional<RectF>& referenceRect)
	{
		if (gradient.GetPositioningMode() == PositioningMode::Relative)
		{
			if (!referenceRect.has_value())
			{
				throw std::invalid_argument{ "referenceRect must be provided when using Relative positioning mode" };
			}
			gradient.ApplyReferenceRect(*referenceRect);
		}

		const auto& gradientStops = gradient.GetStops();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		auto hr = renderTarget->CreateGradientStopCollection(
			gradientStops.data(), static_cast<UINT32>(gradientStops.size()),
			D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP,
			&gradientStopCollection);
		ThrowFailed(hr);

		hr = renderTarget->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(gradient.Start(), gradient.End()),
			gradientStopCollection.Get(),
			GetAddress()
		);
		ThrowFailed(hr);
	}

	RadialGradientBrush::RadialGradientBrush(const ComPtr<ID2D1RadialGradientBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	RadialGradientBrush::RadialGradientBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		RadialGradient gradient, const std::optional<RectF>& referenceRect)
	{
		if (gradient.GetPositioningMode() == PositioningMode::Relative)
		{
			if (!referenceRect.has_value())
			{
				throw std::invalid_argument{ "referenceRect must be provided when using Relative positioning mode" };
			}
			gradient.ApplyReferenceRect(*referenceRect);
		}

		const auto& gradientStops = gradient.GetStops();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		auto hr = renderTarget->CreateGradientStopCollection(
			gradientStops.data(), static_cast<UINT32>(gradientStops.size()),
			D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP,
			&gradientStopCollection);
		ThrowFailed(hr);

		hr = renderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				gradient.GetEllipse().center, gradient.Offset(),
				gradient.GetEllipse().xRadius, gradient.GetEllipse().yRadius),
			gradientStopCollection.Get(),
			GetAddress()
		);
		ThrowFailed(hr);
	}

	BitmapBrush::BitmapBrush(const ComPtr<ID2D1BitmapBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	BitmapBrush::BitmapBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		D2D::D2DBitmap bitmap, const D2D::BitmapBrushProperties& bitmapBrushProperties)
	{
		const auto hr = renderTarget->CreateBitmapBrush(
			bitmap.GetRaw(),
			bitmapBrushProperties, GetAddress());
		ThrowFailed(hr);
	}

	[[nodiscard]] static auto ExtractSolidBrushParameters(const ComPtr<ID2D1SolidColorBrush>& ptr) noexcept
	{
		return RGBA{ ptr->GetColor() };
	}

	[[nodiscard]] static auto ExtractLinearGradientBrushParameters(const ComPtr<ID2D1LinearGradientBrush>& ptr) noexcept
	{
		const PointF start = ptr->GetStartPoint();
		const PointF end = ptr->GetEndPoint();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		ptr->GetGradientStopCollection(&gradientStopCollection);

		const auto gradientStopCount = gradientStopCollection->GetGradientStopCount();
		GradientStops gradientStops(gradientStopCount);
		gradientStopCollection->GetGradientStops(gradientStops.data(), gradientStopCount);

		return LinearGradient{ start, end, gradientStops };
	}

	[[nodiscard]] static auto ExtractRadialGradientBrushParameters(const ComPtr<ID2D1RadialGradientBrush>& ptr) noexcept
	{
		const PointF center = ptr->GetCenter();
		const PointF offset = ptr->GetGradientOriginOffset();
		const auto xRadius = ptr->GetRadiusX();
		const auto yRadius = ptr->GetRadiusY();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		ptr->GetGradientStopCollection(&gradientStopCollection);

		const auto gradientStopCount = gradientStopCollection->GetGradientStopCount();
		GradientStops gradientStops(gradientStopCount);
		gradientStopCollection->GetGradientStops(gradientStops.data(), gradientStopCount);

		return RadialGradient{ Ellipse{ center, xRadius, yRadius }, offset, gradientStops };
	}

	[[nodiscard]] static auto ExtractBitmapBrushParameters(const ComPtr<ID2D1BitmapBrush>& ptr)
	{
		ComPtr<ID2D1Bitmap> bitmap;
		ComPtr<ID2D1Bitmap1> bitmap1;
		ptr->GetBitmap(&bitmap);

		const auto hr = bitmap.As(&bitmap1);
		ThrowFailed(hr);
		const D2D::D2DBitmap bmp{ bitmap1 };

		const auto extendModeX = static_cast<D2D::ExtendMode>(ptr->GetExtendModeX());
		const auto extendModeY = static_cast<D2D::ExtendMode>(ptr->GetExtendModeY());
		const auto interpolationMode = static_cast<D2D::BitmapInterpolationMode>(ptr->GetInterpolationMode());

		const D2D::BitmapBrushProperties bitmapBrushProperties{
			extendModeX,
			extendModeY,
			interpolationMode
		};

		return BitmapBrushParameters{ bmp, bitmapBrushProperties };
	}

	Brush::Brush(const ComPtr<ID2D1Brush>& ptr) noexcept
	{
		ComPtr<ID2D1SolidColorBrush> solidBrush;
		auto hr = ptr.As(&solidBrush);
		LogFailed(LogLevel::Error, hr);

		if (solidBrush)
		{
			parameters = ExtractSolidBrushParameters(solidBrush);
			brush = SolidBrush{ solidBrush };
			return;
		}

		ComPtr<ID2D1LinearGradientBrush> linearGradientBrush;
		hr = ptr.As(&linearGradientBrush);
		LogFailed(LogLevel::Error, hr);

		if (linearGradientBrush)
		{
			parameters = ExtractLinearGradientBrushParameters(linearGradientBrush);
			brush = LinearGradientBrush{ linearGradientBrush };
			return;
		}

		ComPtr<ID2D1RadialGradientBrush> radialGradientBrush;
		hr = ptr.As(&radialGradientBrush);
		LogFailed(LogLevel::Error, hr);

		if (radialGradientBrush)
		{
			parameters = ExtractRadialGradientBrushParameters(radialGradientBrush);
			brush = RadialGradientBrush{ radialGradientBrush };
			return;
		}

		ComPtr<ID2D1BitmapBrush> bitmapBrush;
		hr = ptr.As(&bitmapBrush);
		LogFailed(LogLevel::Error, hr);

		if (bitmapBrush)
		{
			try
			{
				parameters = ExtractBitmapBrushParameters(bitmapBrush);
				brush = BitmapBrush{ bitmapBrush };
				return;
			}
			catch (const HResultException&)
			{
				// Fall back to empty brush
			}
		}

		brush = EmptyBrush{ };
	}

	Brush::Brush(const BrushParameters& parameters) noexcept :
		parameters{ parameters }
	{ }

	Brush::Brush(const ComPtr<ID2D1RenderTarget>& renderTarget, const BrushParameters& parameters) noexcept :
		Brush{ parameters }
	{
		CreateBrush(renderTarget);
	}

	auto Brush::SetParametersAndCreateBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		const BrushParameters& params) noexcept -> void
	{
		parameters = params;
		CreateBrush(renderTarget);
	}

	auto Brush::CreateBrush(ComPtr<ID2D1RenderTarget> renderTarget) noexcept -> void
	{
		ReleaseBrush();
		std::visit([renderTarget, this]<typename T>(T& param)
		{
			if constexpr (std::is_same_v<T, RGBA>)
			{
				brush = SolidBrush{ renderTarget, param };
			}
			else if constexpr (std::is_same_v<T, LinearGradientBrushParameters>)
			{
				brush = LinearGradientBrush{ renderTarget, param.gradient, param.referenceRect };
			}
			else if constexpr (std::is_same_v<T, RadialGradientBrushParameters>)
			{
				brush = RadialGradientBrush{ renderTarget, param.gradient, param.referenceRect };
			}
			else if constexpr (std::is_same_v<T, BitmapBrushParameters>)
			{
				brush = BitmapBrush{ renderTarget, param.bitmap, param.bitmapBrushProperties };
			}
		}, parameters);
	}

	auto Brush::ReleaseBrush() noexcept -> void
	{
		brush = std::monostate{ };
	}

	auto Brush::SetGradientBrushRect(RectF rect) -> void
	{
		std::visit([rect]<typename T>(T& param)
		{
			if constexpr (GradientBrushParameters<T>)
			{
				param.referenceRect = rect;
			}
		}, parameters);
	}

	Brush::operator ID2D1Brush*() const noexcept
	{
		return std::visit([]<typename T>(const T& brush) -> ID2D1Brush*
		{
			if constexpr (!std::is_same_v<std::monostate, T>)
			{
				return brush;
			}
			else
			{
				return nullptr;
			}
		}, brush);
	}
}

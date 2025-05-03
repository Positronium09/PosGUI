module;
#include <wrl.h>
#include <d2d1_1.h>
#include <stdexcept>
#include <variant>

module PGUI.UI.Brush;
import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.D2D.D2DBitmap;
import PGUI.UI.D2D.D2DStructs;


namespace PGUI::UI
{
	SolidBrush::SolidBrush(ComPtr<ID2D1SolidColorBrush> brush) noexcept : 
		ComPtrHolder{ brush }
	{
	}
	SolidBrush::SolidBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, RGBA color)
	{
		auto hr = renderTarget->CreateSolidColorBrush(color, GetAddress()); ThrowFailed(hr);
	}

	LinearGradientBrush::LinearGradientBrush(ComPtr<ID2D1LinearGradientBrush> brush) noexcept :
		ComPtrHolder{ brush }
	{
	}
	LinearGradientBrush::LinearGradientBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, 
		LinearGradient gradient, std::optional<RectF> referenceRect)
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
			&gradientStopCollection); ThrowFailed(hr);

		hr = renderTarget->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(gradient.Start(), gradient.End()),
			gradientStopCollection.Get(),
			GetAddress()
		); ThrowFailed(hr);
	}

	RadialGradientBrush::RadialGradientBrush(ComPtr<ID2D1RadialGradientBrush> brush) noexcept : 
		ComPtrHolder{ brush }
	{
	}
	RadialGradientBrush::RadialGradientBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, 
		RadialGradient gradient, std::optional<RectF> referenceRect)
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
			&gradientStopCollection); ThrowFailed(hr);
		
		hr = renderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(gradient.GetEllipse().center, gradient.Offset(), 
				gradient.GetEllipse().xRadius, gradient.GetEllipse().yRadius),
			gradientStopCollection.Get(),
			GetAddress()
		); ThrowFailed(hr);
	}

	BitmapBrush::BitmapBrush(ComPtr<ID2D1BitmapBrush> brush) noexcept : 
		ComPtrHolder{ brush }
	{
	}
	BitmapBrush::BitmapBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, 
		D2D::D2DBitmap bitmap, const D2D::BitmapBrushProperties& bitmapBrushProperties)
	{
		auto hr = renderTarget->CreateBitmapBrush(bitmap.GetRaw(),
			bitmapBrushProperties, GetAddress()); ThrowFailed(hr);
	}

	[[nodiscard]] static auto ExtractSolidBrushParameters(ComPtr<ID2D1SolidColorBrush> ptr) noexcept
	{
		return RGBA{ ptr->GetColor() };
	}

	[[nodiscard]] static auto ExtractLinearGradientBrushParameters(ComPtr<ID2D1LinearGradientBrush> ptr) noexcept
	{
		PointF start = ptr->GetStartPoint();
		PointF end = ptr->GetEndPoint();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		ptr->GetGradientStopCollection(&gradientStopCollection);

		auto gradientStopCount = gradientStopCollection->GetGradientStopCount();
		GradientStops gradientStops(gradientStopCount);
		gradientStopCollection->GetGradientStops(gradientStops.data(), gradientStopCount);

		return LinearGradient{ start, end, gradientStops };
	}

	[[nodiscard]] static auto ExtractRadialGradientBrushParameters(ComPtr<ID2D1RadialGradientBrush> ptr) noexcept
	{
		PointF center = ptr->GetCenter();
		PointF offset = ptr->GetGradientOriginOffset();
		float xRadius = ptr->GetRadiusX();
		float yRadius = ptr->GetRadiusY();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		ptr->GetGradientStopCollection(&gradientStopCollection);

		auto gradientStopCount = gradientStopCollection->GetGradientStopCount();
		GradientStops gradientStops(gradientStopCount);
		gradientStopCollection->GetGradientStops(gradientStops.data(), gradientStopCount);

		return RadialGradient{ Ellipse{ center, xRadius, yRadius }, offset, gradientStops };
	}

	[[nodiscard]] static auto ExtractBitmapBrushParameters(ComPtr<ID2D1BitmapBrush> ptr)
	{
		ComPtr<ID2D1Bitmap> bitmap;
		ComPtr<ID2D1Bitmap1> bitmap1;
		ptr->GetBitmap(&bitmap);

		auto hr = bitmap.As(&bitmap1); ThrowFailed(hr);
		D2D::D2DBitmap bmp{ bitmap1 };

		auto extendModeX = static_cast<D2D::ExtendMode>(ptr->GetExtendModeX());
		auto extendModeY = static_cast<D2D::ExtendMode>(ptr->GetExtendModeY());
		auto interpolationMode = static_cast<D2D::BitmapInterpolationMode>(ptr->GetInterpolationMode());

		D2D::BitmapBrushProperties bitmapBrushProperties{
			extendModeX,
			extendModeY,
			interpolationMode
		};

		return BitmapBrushParameters{ bmp, bitmapBrushProperties };
	}

	Brush::Brush(ComPtr<ID2D1Brush> ptr) noexcept
	{
		ComPtr<ID2D1SolidColorBrush> solidBrush;
		ptr.As(&solidBrush);
		if (solidBrush)
		{
			parameters = ExtractSolidBrushParameters(solidBrush);
			brush = SolidBrush{ solidBrush };
			return;
		}

		ComPtr<ID2D1LinearGradientBrush> linearGradientBrush;
		ptr.As(&linearGradientBrush);
		if (linearGradientBrush)
		{
			parameters = ExtractLinearGradientBrushParameters(linearGradientBrush);
			brush = LinearGradientBrush{ linearGradientBrush };
			return;
		}

		ComPtr<ID2D1RadialGradientBrush> radialGradientBrush;
		ptr.As(&radialGradientBrush);
		if (radialGradientBrush)
		{
			parameters = ExtractRadialGradientBrushParameters(radialGradientBrush);
			brush = RadialGradientBrush{ radialGradientBrush };
			return;
		}

		ComPtr<ID2D1BitmapBrush> bitmapBrush;
		ptr.As(&bitmapBrush);
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

	Brush::Brush(BrushParameters parameters) noexcept :
		parameters{ parameters }
	{
	}

	Brush::Brush(ComPtr<ID2D1RenderTarget> renderTarget, BrushParameters parameters) noexcept : 
		Brush{ parameters }
	{
		CreateBrush(renderTarget);
	}

	void Brush::SetParametersAndCreateBrush(ComPtr<ID2D1RenderTarget> renderTarget, 
		const BrushParameters& params) noexcept
	{
		parameters = params;
		CreateBrush(renderTarget);
	}

	void Brush::CreateBrush(ComPtr<ID2D1RenderTarget> renderTarget) noexcept
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
	
	void Brush::ReleaseBrush() noexcept
	{
		brush = std::monostate{ };
	}
	
	void Brush::SetGradientBrushRect(RectF rect)
	{
		std::visit([rect]<typename T>(T & param)
		{
			if constexpr (GradientBrushParameters<T>)
			{
				param.referenceRect = rect;
			}
		}, parameters);
	}
	
	Brush::operator ID2D1Brush* () const noexcept
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

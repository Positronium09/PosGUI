module;
#include <d2d1_1.h>

module PGUI.UI.Brush;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.UI.D2D.D2DBitmap;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.D2D.D2DEnums;

namespace PGUI::UI
{
	SolidBrush::SolidBrush(const ComPtr<ID2D1SolidColorBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	auto SolidBrush::Create(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		const RGBA& color) noexcept -> Result<SolidBrush>
	{
		ComPtr<ID2D1SolidColorBrush> brush;
		if (auto error = Error{ renderTarget->CreateSolidColorBrush(color, brush.put()) };
			error.IsFailure())
		{
			error.AddDetail(L"color", std::format(L"{}", color));
			return Unexpected{ error };
		}

		return SolidBrush{ brush };
	}

	LinearGradientBrush::LinearGradientBrush(const ComPtr<ID2D1LinearGradientBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	auto LinearGradientBrush::Create(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		LinearGradient gradient,
		const std::optional<RectF>& referenceRect) noexcept -> Result<LinearGradientBrush>
	{
		if (gradient.GetPositioningMode() == PositioningMode::Relative)
		{
			if (!referenceRect.has_value())
			{
				return Unexpected{
					Error{ ErrorCode::InvalidArgument }
					.SetCustomMessage(L"Reference rectangle must be provided when using Relative positioning mode")
				};
			}
			gradient.ApplyReferenceRect(*referenceRect);
		}

		const auto& gradientStops = gradient.GetStops();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		if (const auto error = Error{
				renderTarget->CreateGradientStopCollection(
					gradientStops.data(), static_cast<UINT32>(gradientStops.size()),
					D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP,
					gradientStopCollection.put())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		ComPtr<ID2D1LinearGradientBrush> brush;
		if (auto error = Error{
				renderTarget->CreateLinearGradientBrush(
					D2D1::LinearGradientBrushProperties(gradient.Start(), gradient.End()),
					gradientStopCollection.get(),
					brush.put())
			};
			error.IsFailure())
		{
			error
				.AddDetail(L"start", std::format(L"{}", gradient.Start()))
				.AddDetail(L"end", std::format(L"{}", gradient.End()));
			return Unexpected{ error };
		}

		return LinearGradientBrush{ brush };
	}

	RadialGradientBrush::RadialGradientBrush(const ComPtr<ID2D1RadialGradientBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	auto RadialGradientBrush::Create(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		RadialGradient gradient,
		const std::optional<RectF>& referenceRect) noexcept -> Result<RadialGradientBrush>
	{
		if (gradient.GetPositioningMode() == PositioningMode::Relative)
		{
			if (!referenceRect.has_value())
			{
				return Unexpected{
					Error{ ErrorCode::InvalidArgument }
					.SetCustomMessage(L"Reference rectangle must be provided when using Relative positioning mode")
				};
			}
			gradient.ApplyReferenceRect(*referenceRect);
		}

		const auto& gradientStops = gradient.GetStops();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		if (const auto error = Error{
				renderTarget->CreateGradientStopCollection(
					gradientStops.data(), static_cast<UINT32>(gradientStops.size()),
					D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP,
					gradientStopCollection.put())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		ComPtr<ID2D1RadialGradientBrush> brush;
		if (auto error = Error{
				renderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(
						gradient.GetEllipse().center, gradient.Offset(),
						gradient.GetEllipse().xRadius, gradient.GetEllipse().yRadius),
					gradientStopCollection.get(),
					brush.put())
			};
			error.IsFailure())
		{
			error
				.AddDetail(L"ellipse", std::format(L"{}", gradient.GetEllipse()))
				.AddDetail(L"offset", std::format(L"{}", gradient.Offset()));
			return Unexpected{ error };
		}

		return RadialGradientBrush{ brush };
	}

	BitmapBrush::BitmapBrush(const ComPtr<ID2D1BitmapBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	auto BitmapBrush::Create(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		const D2D::D2DBitmap& bitmap,
		const D2D::BitmapBrushProperties& bitmapBrushProperties) noexcept -> Result<BitmapBrush>
	{
		ComPtr<ID2D1BitmapBrush> brush;
		if (const auto error = Error{
				renderTarget->CreateBitmapBrush(
					bitmap.GetRaw(),
					bitmapBrushProperties, brush.put())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return BitmapBrush{ brush };
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

	[[nodiscard]] static auto ExtractBitmapBrushParameters(
		const ComPtr<ID2D1BitmapBrush>& ptr) noexcept -> Result<BitmapBrushParameters>
	{
		ComPtr<ID2D1Bitmap> bitmap;
		ptr->GetBitmap(&bitmap);

		const auto bitmap1 = bitmap.try_query<ID2D1Bitmap1>();
		if (bitmap1.get() == nullptr)
		{
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		const D2D::D2DBitmap bmp{ bitmap1 };

		const auto extendModeX = FromUnderlying<D2D::ExtendMode>(ptr->GetExtendModeX());
		const auto extendModeY = FromUnderlying<D2D::ExtendMode>(ptr->GetExtendModeY());
		const auto interpolationMode = FromUnderlying<D2D::BitmapInterpolationMode>(ptr->GetInterpolationMode());

		const D2D::BitmapBrushProperties bitmapBrushProperties{
			extendModeX,
			extendModeY,
			interpolationMode
		};

		return BitmapBrushParameters{ bmp, bitmapBrushProperties };
	}

	Brush::Brush(const BrushParameters& parameters) noexcept :
		parameters{ parameters }
	{ }

	auto Brush::CreateFrom(const ComPtr<ID2D1Brush>& ptr) noexcept -> Result<Brush>
	{
		if (const auto solidBrush = ptr.try_query<ID2D1SolidColorBrush>())
		{
			Brush result{ ExtractSolidBrushParameters(solidBrush) };
			result.brush = SolidBrush{ solidBrush };
			return result;
		}

		if (const auto linearGradientBrush = ptr.try_query<ID2D1LinearGradientBrush>())
		{
			Brush result{ ExtractLinearGradientBrushParameters(linearGradientBrush) };
			result.brush = LinearGradientBrush{ linearGradientBrush };
			return result;
		}

		if (const auto radialGradientBrush = ptr.try_query<ID2D1RadialGradientBrush>())
		{
			Brush result{ ExtractRadialGradientBrushParameters(radialGradientBrush) };
			result.brush = RadialGradientBrush{ radialGradientBrush };
			return result;
		}

		if (const auto bitmapBrush = ptr.try_query<ID2D1BitmapBrush>())
		{
			const auto parametersResult = ExtractBitmapBrushParameters(bitmapBrush);
			if (!parametersResult.has_value())
			{
				return Unexpected{ parametersResult.error() };
			}

			Brush result{ parametersResult.value() };
			result.brush = BitmapBrush{ bitmapBrush };
			return result;
		}

		return Unexpected{
			Error{ SystemErrorCode::InterfaceNotSupported }
			.SetCustomMessage(L"Unknown brush type")
		};
	}

	auto Brush::CreateBrush(const ComPtr<ID2D1RenderTarget>& renderTarget) noexcept -> Result<void>
	{
		ReleaseBrush();

		return Match(parameters, [&renderTarget, this]<typename T>(const T& param) -> Result<void>
		{
			const auto assign = [this](auto&& brushResult) -> Result<void>
			{
				if (!brushResult.has_value())
				{
					return Unexpected{ brushResult.error() };
				}
				brush = MoveChecked(brushResult.value());
				return EmptyResult;
			};

			if constexpr (std::is_same_v<T, RGBA>)
			{
				return assign(SolidBrush::Create(renderTarget, param));
			}
			else if constexpr (std::is_same_v<T, LinearGradientBrushParameters>)
			{
				return assign(LinearGradientBrush::Create(renderTarget, param.gradient, param.referenceRect));
			}
			else if constexpr (std::is_same_v<T, RadialGradientBrushParameters>)
			{
				return assign(RadialGradientBrush::Create(renderTarget, param.gradient, param.referenceRect));
			}
			else if constexpr (std::is_same_v<T, BitmapBrushParameters>)
			{
				return assign(BitmapBrush::Create(renderTarget, param.bitmap, param.bitmapBrushProperties));
			}
			else
			{
				return EmptyResult;
			}
		});
	}

	auto Brush::ReleaseBrush() noexcept -> void
	{
		brush = EmptyBrush{ };
	}

	auto Brush::SetGradientBrushRect(RectF rect) noexcept -> void
	{
		Match(parameters, [rect]<typename T>(T& param)
		{
			if constexpr (GradientBrushParameters<T>)
			{
				param.referenceRect = rect;
			}
		});
	}

	auto Brush::SetParameters(const BrushParameters& brushParameters) noexcept -> void
	{
		parameters = brushParameters;
	}

	auto Brush::SetParameters(const ComPtr<ID2D1RenderTarget>& renderTarget,
	                          const BrushParameters& brushParameters) noexcept -> Result<void>
	{
		SetParameters(brushParameters);

		return CreateBrush(renderTarget);
	}

	Brush::operator ID2D1Brush*() const noexcept
	{
		return Match(brush, []<typename T>(const T& brush) -> ID2D1Brush*
		{
			if constexpr (!std::is_same_v<EmptyBrush, T>)
			{
				return brush;
			}
			else
			{
				return nullptr;
			}
		});
	}
}

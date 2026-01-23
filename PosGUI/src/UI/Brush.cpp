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

	SolidBrush::SolidBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, const RGBA color) noexcept
	{
		if (const auto hr = renderTarget->CreateSolidColorBrush(color, Put());
			FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              .AddDetail(L"Color", std::format(L"{}", color)),
			              L"Failed to create SolidColorBrush");
		}
	}

	LinearGradientBrush::LinearGradientBrush(const ComPtr<ID2D1LinearGradientBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	LinearGradientBrush::LinearGradientBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		LinearGradient gradient, const std::optional<RectF>& referenceRect) noexcept
	{
		if (gradient.GetPositioningMode() == PositioningMode::Relative)
		{
			if (!referenceRect.has_value())
			{
				Logger::Error(Error{ ErrorCode::InvalidArgument },
				              L"Reference rectangle must be provided when using Relative positioning mode");
				return;
			}
			gradient.ApplyReferenceRect(*referenceRect);
		}

		const auto& gradientStops = gradient.GetStops();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		auto hr = renderTarget->CreateGradientStopCollection(
			gradientStops.data(), static_cast<UINT32>(gradientStops.size()),
			D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP,
			&gradientStopCollection);
		if (FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create GradientStopCollection");
			return;
		}

		hr = renderTarget->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(gradient.Start(), gradient.End()),
			gradientStopCollection.get(),
			Put()
		);
		if (FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              .AddDetail(
				              L"Start",
				              std::format(L"{}", gradient.Start()))
			              .AddDetail(
				              L"End", std::format(L"{}", gradient.End())),
			              L"Failed to create LinearGradientBrush");
			return;
		}
	}

	RadialGradientBrush::RadialGradientBrush(const ComPtr<ID2D1RadialGradientBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	RadialGradientBrush::RadialGradientBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		RadialGradient gradient, const std::optional<RectF>& referenceRect) noexcept
	{
		if (gradient.GetPositioningMode() == PositioningMode::Relative)
		{
			if (!referenceRect.has_value())
			{
				Logger::Error(Error{ ErrorCode::InvalidArgument },
				              L"Reference rectangle must be provided when using Relative positioning mode");
				return;
			}
			gradient.ApplyReferenceRect(*referenceRect);
		}

		const auto& gradientStops = gradient.GetStops();

		ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
		auto hr = renderTarget->CreateGradientStopCollection(
			gradientStops.data(), static_cast<UINT32>(gradientStops.size()),
			D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP,
			&gradientStopCollection);
		if (FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create GradientStopCollection");
			return;
		}

		hr = renderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				gradient.GetEllipse().center, gradient.Offset(),
				gradient.GetEllipse().xRadius, gradient.GetEllipse().yRadius),
			gradientStopCollection.get(),
			Put()
		);
		if (FAILED(hr))
		{
			Logger::Error(Error{ hr }
			              .AddDetail(
				              L"Center",
				              std::format(L"{}", gradient.GetEllipse()))
			              .AddDetail(
				              L"Offset", std::format(L"{}", gradient.Offset())),
			              L"Failed to create RadialGradientBrush");
			return;
		}
	}

	BitmapBrush::BitmapBrush(const ComPtr<ID2D1BitmapBrush>& brush) noexcept :
		ComPtrHolder{ brush }
	{ }

	BitmapBrush::BitmapBrush(
		const ComPtr<ID2D1RenderTarget>& renderTarget,
		D2D::D2DBitmap bitmap, const D2D::BitmapBrushProperties& bitmapBrushProperties) noexcept
	{
		const auto hr = renderTarget->CreateBitmapBrush(
			bitmap.GetRaw(),
			bitmapBrushProperties, Put());
		if (FAILED(hr))
		{
			Logger::Error(Error{ hr },
			              L"Failed to create BitmapBrush");
			return;
		}
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
			Error error{ E_NOINTERFACE };
			Logger::Error(error,
			              L"Failed to extract bitmap from BitmapBrush");
			return Unexpected{ error };
		}

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
		ComPtrHolder<ID2D1Brush, ID2D1SolidColorBrush, ID2D1LinearGradientBrush,
		             ID2D1RadialGradientBrush, ID2D1BitmapBrush> holder{ nullptr };
		holder.Attach(ptr.get());
		auto hrSolidColor = holder.AsAssign<ID2D1Brush, ID2D1SolidColorBrush>();
		auto hrLinearGradient = holder.AsAssign<ID2D1Brush, ID2D1LinearGradientBrush>();
		auto hrRadialGradient = holder.AsAssign<ID2D1Brush, ID2D1RadialGradientBrush>();
		auto hrBitmapBrush = holder.AsAssign<ID2D1Brush, ID2D1BitmapBrush>();

		LogIfFailed(
			LogLevel::Info,
			Error{ hrSolidColor },
			L"Given pointer is not a SolidColorBrush"
		);

		if (holder.IsInitialized<ID2D1SolidColorBrush>())
		{
			const auto& solidBrush = holder.Get<ID2D1SolidColorBrush>();
			parameters = ExtractSolidBrushParameters(solidBrush);
			brush = SolidBrush{ solidBrush };
			return;
		}

		LogIfFailed(
			LogLevel::Info,
			Error{ hrLinearGradient },
			L"Given pointer is not a LinearGradientBrush"
		);

		if (holder.IsInitialized<ID2D1LinearGradientBrush>())
		{
			const auto& linearGradientBrush = holder.Get<ID2D1LinearGradientBrush>();
			parameters = ExtractLinearGradientBrushParameters(linearGradientBrush);
			brush = LinearGradientBrush{ linearGradientBrush };
			return;
		}

		LogIfFailed(
			LogLevel::Info,
			Error{ hrRadialGradient },
			L"Given pointer is not a RadialGradientBrush"
		);

		if (holder.IsInitialized<ID2D1RadialGradientBrush>())
		{
			const auto& radialGradientBrush = holder.Get<ID2D1RadialGradientBrush>();
			parameters = ExtractRadialGradientBrushParameters(radialGradientBrush);
			brush = RadialGradientBrush{ radialGradientBrush };
			return;
		}

		LogIfFailed(
			LogLevel::Info,
			Error{ hrBitmapBrush },
			L"Given pointer is not a BitmapBrush"
		);

		if (holder.IsInitialized<ID2D1BitmapBrush>())
		{
			const auto& bitmapBrush = holder.Get<ID2D1BitmapBrush>();
			const auto result = ExtractBitmapBrushParameters(bitmapBrush);
			if (!result.has_value())
			{
				Logger::Info(result.error(), L"Failed to extract BitmapBrush parameters");
				brush = EmptyBrush{ };
				return;
			}
			parameters = result.value();
			brush = BitmapBrush{ bitmapBrush };

			return;
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

module;
#include <Windows.h>
#include <d2d1_1.h>
#include <wrl.h>
#include <optional>
#include <variant>
#include <concepts>

export module PGUI.UI.Brush;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.Color;
import PGUI.UI.Gradient;
import PGUI.UI.D2D.D2DBitmap;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI
{
	class SolidBrush : public ComPtrHolder<ID2D1SolidColorBrush>
	{
		public:
		SolidBrush() noexcept = default;
		explicit SolidBrush(ComPtr<ID2D1SolidColorBrush> brush) noexcept;
		SolidBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, RGBA color);
		~SolidBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().Get(); }
	};

	class LinearGradientBrush : public ComPtrHolder<ID2D1LinearGradientBrush>
	{
		public:
		LinearGradientBrush() noexcept = default;
		explicit LinearGradientBrush(ComPtr<ID2D1LinearGradientBrush> brush) noexcept;
		LinearGradientBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, 
			LinearGradient gradient, std::optional<RectF> referenceRect = std::nullopt);
		~LinearGradientBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().Get(); }
	};

	class RadialGradientBrush : public ComPtrHolder<ID2D1RadialGradientBrush>
	{
		public:
		RadialGradientBrush() noexcept = default;
		explicit RadialGradientBrush(ComPtr<ID2D1RadialGradientBrush> brush) noexcept;
		RadialGradientBrush(const ComPtr<ID2D1RenderTarget>& renderTarget,
			RadialGradient gradient, std::optional<RectF> referenceRect = std::nullopt);
		~RadialGradientBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().Get(); }
	};

	class BitmapBrush : public ComPtrHolder<ID2D1BitmapBrush>
	{
		public:
		BitmapBrush() noexcept = default;
		explicit BitmapBrush(ComPtr<ID2D1BitmapBrush> brush) noexcept;
		BitmapBrush(const ComPtr<ID2D1RenderTarget>& renderTarget,
			D2D::D2DBitmap bitmap, const D2D::BitmapBrushProperties& bitmapBrushProperties);
		~BitmapBrush() noexcept = default;

		explicit(false) operator ID2D1Brush* () const noexcept { return Get().Get(); }
	};;

	struct LinearGradientBrushParameters
	{
		LinearGradient gradient;
		std::optional<RectF> referenceRect;

		constexpr LinearGradientBrushParameters(LinearGradient gradient,
			std::optional<RectF> referenceRect = std::nullopt) :
			gradient{ gradient }, referenceRect{ referenceRect }
		{
		}
	};
	struct RadialGradientBrushParameters
	{
		RadialGradient gradient;
		std::optional<RectF> referenceRect;

		constexpr RadialGradientBrushParameters(RadialGradient gradient,
			std::optional<RectF> referenceRect = std::nullopt) :
			gradient{ gradient }, referenceRect{ referenceRect }
		{
		}
	};

	struct BitmapBrushParameters
	{
		D2D::D2DBitmap bitmap;
		D2D::BitmapBrushProperties bitmapBrushProperties;

		BitmapBrushParameters(D2D::D2DBitmap bitmap,
			D2D::BitmapBrushProperties bitmapBrushProperties = D2D::BitmapBrushProperties{ }) :
			bitmap{ bitmap }, bitmapBrushProperties{ bitmapBrushProperties }
		{
		}
	};

	using EmptyBrush = std::monostate;

	using BrushParameters = std::variant<EmptyBrush, RGBA, BitmapBrushParameters,
		LinearGradientBrushParameters, RadialGradientBrushParameters>;

	template <typename T>
	concept GradientBrushParameters =
		std::is_same_v<T, LinearGradientBrushParameters> ||
		std::is_same_v<T, RadialGradientBrushParameters>;

	class Brush final
	{
		using BrushVariant = std::variant<EmptyBrush,
			SolidBrush, LinearGradientBrush, RadialGradientBrush, BitmapBrush>;

		public:
		explicit Brush(ComPtr<ID2D1Brush> ptr) noexcept;
		explicit Brush(BrushParameters parameters) noexcept;
		Brush(ComPtr<ID2D1RenderTarget> renderTarget, BrushParameters parameters) noexcept;

		void SetParametersAndCreateBrush(ComPtr<ID2D1RenderTarget> renderTarget, 
			const BrushParameters& parameters) noexcept;

		void CreateBrush(ComPtr<ID2D1RenderTarget> renderTarget) noexcept;
		void ReleaseBrush() noexcept;

		void SetGradientBrushRect(RectF rect);

		[[nodiscard]] auto GetParameters() const noexcept { return parameters; }

		explicit(false) operator ID2D1Brush*() const noexcept;
		explicit(false) operator bool() const noexcept { return !std::holds_alternative<std::monostate>(brush); }

		private:
		BrushVariant brush;
		BrushParameters parameters;
	};
}

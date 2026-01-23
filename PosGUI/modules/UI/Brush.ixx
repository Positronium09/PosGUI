module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.Brush;

import std;

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

		explicit SolidBrush(const ComPtr<ID2D1SolidColorBrush>& brush) noexcept;

		SolidBrush(const ComPtr<ID2D1RenderTarget>& renderTarget, RGBA color) noexcept;

		~SolidBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().get(); }
	};

	class LinearGradientBrush : public ComPtrHolder<ID2D1LinearGradientBrush>
	{
		public:
		LinearGradientBrush() noexcept = default;

		explicit LinearGradientBrush(const ComPtr<ID2D1LinearGradientBrush>& brush) noexcept;

		LinearGradientBrush(
			const ComPtr<ID2D1RenderTarget>& renderTarget,
			LinearGradient gradient, const std::optional<RectF>& referenceRect = std::nullopt) noexcept;

		~LinearGradientBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().get(); }
	};

	class RadialGradientBrush : public ComPtrHolder<ID2D1RadialGradientBrush>
	{
		public:
		RadialGradientBrush() noexcept = default;

		explicit RadialGradientBrush(const ComPtr<ID2D1RadialGradientBrush>& brush) noexcept;

		RadialGradientBrush(
			const ComPtr<ID2D1RenderTarget>& renderTarget,
			RadialGradient gradient, const std::optional<RectF>& referenceRect = std::nullopt) noexcept;

		~RadialGradientBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().get(); }
	};

	class BitmapBrush : public ComPtrHolder<ID2D1BitmapBrush>
	{
		public:
		BitmapBrush() noexcept = default;

		explicit BitmapBrush(const ComPtr<ID2D1BitmapBrush>& brush) noexcept;

		BitmapBrush(const ComPtr<ID2D1RenderTarget>& renderTarget,
			D2D::D2DBitmap bitmap, const D2D::BitmapBrushProperties& bitmapBrushProperties) noexcept;

		~BitmapBrush() noexcept = default;

		explicit(false) operator ID2D1Brush*() const noexcept { return Get().get(); }
	};;

	struct LinearGradientBrushParameters
	{
		LinearGradient gradient;
		std::optional<RectF> referenceRect;

		explicit(false) constexpr LinearGradientBrushParameters(
			const LinearGradient& gradient,
			const std::optional<RectF>& referenceRect = std::nullopt) :
			gradient{ gradient }, referenceRect{ referenceRect }
		{
		}
	};

	struct RadialGradientBrushParameters
	{
		RadialGradient gradient;
		std::optional<RectF> referenceRect;

		explicit(false) constexpr RadialGradientBrushParameters(
			const RadialGradient& gradient,
			const std::optional<RectF>& referenceRect = std::nullopt) :
			gradient{ gradient }, referenceRect{ referenceRect }
		{
		}
	};

	struct BitmapBrushParameters
	{
		D2D::D2DBitmap bitmap;
		D2D::BitmapBrushProperties bitmapBrushProperties;

		explicit(false) BitmapBrushParameters(
			const D2D::D2DBitmap& bitmap,
			const D2D::BitmapBrushProperties bitmapBrushProperties = D2D::BitmapBrushProperties{ }) :
			bitmap{ bitmap }, bitmapBrushProperties{ bitmapBrushProperties }
		{
		}
	};

	using EmptyBrush = std::monostate;

	using BrushParameters = std::variant<
		EmptyBrush, RGBA, BitmapBrushParameters,
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
		explicit Brush(const ComPtr<ID2D1Brush>& ptr) noexcept;

		explicit Brush(const BrushParameters& parameters) noexcept;

		Brush(const ComPtr<ID2D1RenderTarget>& renderTarget, const BrushParameters& parameters) noexcept;

		auto SetParametersAndCreateBrush(const ComPtr<ID2D1RenderTarget>& renderTarget,
			const BrushParameters& params) noexcept -> void;

		auto CreateBrush(ComPtr<ID2D1RenderTarget> renderTarget) noexcept -> void;

		auto ReleaseBrush() noexcept -> void;

		auto SetGradientBrushRect(RectF rect) -> void;

		[[nodiscard]] auto GetParameters() const noexcept { return parameters; }

		explicit(false) operator ID2D1Brush*() const noexcept;

		explicit(false) operator bool() const noexcept { return !std::holds_alternative<std::monostate>(brush); }

		private:
		BrushVariant brush;
		BrushParameters parameters;
	};
}

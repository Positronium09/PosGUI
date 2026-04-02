module;
#include <d2d1_1.h>

export module PGUI.UI.Gradient;

import std;

import PGUI.Shape2D;
import PGUI.UI.Color;
import PGUI.Utils;

export namespace PGUI::UI
{
	struct GradientStop : D2D1_GRADIENT_STOP
	{
		constexpr GradientStop() noexcept = default;

		constexpr GradientStop(const float gradientPosition, const RGBA gradientColor) noexcept :
			D2D1_GRADIENT_STOP{ .position = gradientPosition, .color = gradientColor }
		{
		}

		explicit(false) constexpr GradientStop(const D2D1_GRADIENT_STOP& gradientStop) noexcept :
			D2D1_GRADIENT_STOP{ .position = gradientStop.position, .color = gradientStop.color }
		{
		}
	};

	using GradientStops = std::vector<GradientStop>;

	class Gradient
	{
		public:
		virtual ~Gradient() noexcept = default;

		template <typename Self>
		[[nodiscard]] auto&& GetStops(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.stops);
		}

		[[nodiscard]] auto GetPositioningMode() const noexcept { return mode; }
		auto SetPositioningMode(const PositioningMode newMode) noexcept -> void { mode = newMode; }

		virtual auto ApplyReferenceRect(RectF rect) noexcept -> void = 0;

		protected:
		explicit Gradient(const GradientStops& stops) noexcept;

		private:
		GradientStops stops;
		PositioningMode mode = PositioningMode::Relative;
	};

	class LinearGradient final : public Gradient
	{
		public:
		LinearGradient(PointF start, PointF end, const GradientStops& stops) noexcept;

		[[nodiscard]] auto Start() const noexcept { return start; }
		auto Start(const PointF newStart) noexcept -> void { start = newStart; }
		[[nodiscard]] auto End() const noexcept { return end; }
		auto End(const PointF newEnd) noexcept -> void { end = newEnd; }

		auto ApplyReferenceRect(RectF rect) noexcept -> void override;

		[[nodiscard]] auto ReferenceRectApplied(RectF rect) const noexcept -> LinearGradient;

		private:
		PointF start;
		PointF end;
	};

	class RadialGradient final : public Gradient
	{
		public:
		RadialGradient(Ellipse ellipse, PointF offset, const GradientStops& stops) noexcept;

		[[nodiscard]] auto GetEllipse() const noexcept { return ellipse; }
		auto SetEllipse(const Ellipse newEllipse) noexcept -> void { ellipse = newEllipse; }
		[[nodiscard]] auto Offset() const noexcept { return offset; }
		auto Offset(const PointF newOffset) noexcept -> void { offset = newOffset; }

		auto ApplyReferenceRect(RectF rect) noexcept -> void override;

		[[nodiscard]] auto ReferenceRectApplied(RectF rect) const noexcept -> RadialGradient;

		private:
		Ellipse ellipse;
		PointF offset;
	};
}

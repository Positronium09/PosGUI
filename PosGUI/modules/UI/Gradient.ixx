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
		GradientStop() noexcept = default;

		GradientStop(const float _position, const RGBA _color) noexcept :
			D2D1_GRADIENT_STOP{ .position = _position, .color = _color }
		{
		}

		explicit(false) GradientStop(const D2D1_GRADIENT_STOP& gradientStop) noexcept :
			D2D1_GRADIENT_STOP()
		{
			position = gradientStop.position;
			color = gradientStop.color;
		}
	};

	using GradientStops = std::vector<GradientStop>;

	class Gradient
	{
		public:
		virtual ~Gradient() noexcept = default;

		[[nodiscard]] const auto& GetStops() noexcept { return stops; }
		[[nodiscard]] auto& GetStops() const noexcept { return stops; }

		[[nodiscard]] auto GetPositioningMode() const noexcept { return mode; }
		auto SetPositioningMode(const PositioningMode _mode) noexcept -> void { mode = _mode; }

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
		auto Start(const PointF _start) noexcept -> void { start = _start; }
		[[nodiscard]] auto End() const noexcept { return end; }
		auto End(const PointF _end) noexcept -> void { end = _end; }

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
		auto SetEllipse(const Ellipse _ellipse) noexcept -> void { ellipse = _ellipse; }
		[[nodiscard]] auto Offset() const noexcept { return offset; }
		auto Offset(const PointF _offset) noexcept -> void { offset = _offset; }

		auto ApplyReferenceRect(RectF rect) noexcept -> void override;

		[[nodiscard]] auto ReferenceRectApplied(RectF rect) const noexcept -> RadialGradient;

		private:
		Ellipse ellipse;
		PointF offset;
	};
}

module;
#include <d2d1_1.h>
#include <vector>

export module PGUI.UI.Gradient;

import PGUI.Shape2D;
import PGUI.UI.Color;
import PGUI.Utils;

export namespace PGUI::UI
{
	struct GradientStop : public D2D1_GRADIENT_STOP
	{
		GradientStop() noexcept = default;

		GradientStop(float _position, RGBA _color) noexcept : D2D1_GRADIENT_STOP()
		{
			position = _position;
			color = _color;
		}

		explicit(false) GradientStop(D2D1_GRADIENT_STOP gradientStop) noexcept : D2D1_GRADIENT_STOP()
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
		void SetPositioningMode(PositioningMode _mode) noexcept { mode = _mode; }

		virtual void ApplyReferenceRect(RectF rect) noexcept = 0;

		protected:
		explicit Gradient(const GradientStops& stops) noexcept;

		private:
		GradientStops stops;
		PositioningMode mode = PositioningMode::Relative;
	};

	class LinearGradient : public Gradient
	{
		public:
		LinearGradient(PointF start, PointF end, const GradientStops& stops) noexcept;

		[[nodiscard]] auto Start() const noexcept { return start; }
		void Start(PointF _start) noexcept { start = _start; }
		[[nodiscard]] auto End() const noexcept { return end; }
		void End(PointF _end) noexcept { end = _end; }

		void ApplyReferenceRect(RectF rect) noexcept override;
		[[nodiscard]] auto ReferenceRectApplied(RectF rect) const noexcept -> LinearGradient;

		private:
		PointF start;
		PointF end;
	};

	class RadialGradient : public Gradient
	{
		public:
		RadialGradient(Ellipse ellipse, PointF offset, const GradientStops& stops) noexcept;

		[[nodiscard]] auto GetEllipse() const noexcept { return ellipse; }
		void SetEllipse(Ellipse _ellipse) noexcept { ellipse = _ellipse; }
		[[nodiscard]] auto Offset() const noexcept { return offset; }
		void Offset(PointF _offset) noexcept { offset = _offset; }

		void ApplyReferenceRect(RectF rect) noexcept override;
		[[nodiscard]] auto ReferenceRectApplied(RectF rect) const noexcept -> RadialGradient;

		private:
		Ellipse ellipse;
		PointF offset;
	};
}

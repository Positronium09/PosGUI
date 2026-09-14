module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Geometry.CompositionEllipseGeometry;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.UI.D2D.D2DEllipseGeometry;
import PGUI.Wrapper;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Geometry.CompositionGeometry;

import std;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionEllipseGeometry : public CompositionGeometryT<WUC::CompositionEllipseGeometry>
	{
		public:
		using CompositionGeometryT::CompositionGeometryT;

		[[nodiscard]] static auto Create(const Ellipse& ellipse) noexcept -> Result<CompositionEllipseGeometry>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &ellipse]
			{
				const auto geometry = compositor.CreateEllipseGeometry();
				geometry.Center({ ellipse.center.x, ellipse.center.y });
				geometry.Radius({ ellipse.xRadius, ellipse.yRadius });

				return CompositionEllipseGeometry{ geometry };
			});
		}
		[[nodiscard]] static auto Create(const Point2F& center, const float xRadius, const float yRadius) noexcept -> Result<CompositionEllipseGeometry>
		{
			return Create(Ellipse{ center, xRadius, yRadius });
		}

		[[nodiscard]] static auto CreateFrom(const D2D::D2DEllipseGeometry& g) noexcept -> Result<CompositionEllipseGeometry>
		{
			return Create(g.GetEllipse());
		}

		[[nodiscard]] auto GetCenter() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().Center() };
			});
		}
		[[nodiscard]] auto SetCenter(const Point2F& center) const noexcept -> Result<void>
		{
			return CallProjected([this, &center]
			{
				this->Get().Center(center);
			});
		}

		[[nodiscard]] auto GetRadius() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().Radius() };
			});
		}
		[[nodiscard]] auto SetRadius(const Point2F& radius) const noexcept -> Result<void>
		{
			return CallProjected([this, &radius]
			{
				this->Get().Radius(radius);
			});
		}

		[[nodiscard]] auto GetEllipse() const noexcept -> Result<Ellipse>
		{
			return CallProjected([this]
			{
				const auto center = this->Get().Center();
				const auto radius = this->Get().Radius();
				return Ellipse{ Point2F{ center }, radius.x, radius.y };
			});
		}
		[[nodiscard]] auto SetEllipse(const Ellipse& ellipse) const noexcept -> Result<void>
		{
			if (const auto result = SetCenter(ellipse.center);
				!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			if (const auto result = SetRadius({ ellipse.xRadius, ellipse.yRadius });
				!result.has_value())
			{
				return Unexpected{ result.error() };
			}

			return EmptyResult;
		}
	};
}

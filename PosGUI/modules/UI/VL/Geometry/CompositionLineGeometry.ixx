module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Geometry.CompositionLineGeometry;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Geometry.CompositionGeometry;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionLineGeometry : public CompositionGeometryT<WUC::CompositionLineGeometry>
	{
		public:
		using CompositionGeometryT::CompositionGeometryT;

		[[nodiscard]] static auto Create(const Point2F& p1, const Point2F& p2) noexcept -> Result<CompositionLineGeometry>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &p1, &p2]
			{
				const auto geometry = compositor.CreateLineGeometry();
				geometry.Start(p1);
				geometry.End(p2);
				
				return CompositionLineGeometry{ geometry };
			});
		}
		[[nodiscard]] static auto Create(const LineSegmentF& line) noexcept -> Result<CompositionLineGeometry>
		{
			return Create(line.start, line.end);
		}

		[[nodiscard]] auto GetEnd() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().End() };
			});
		}
		[[nodiscard]] auto SetEnd(const Point2F& end) const noexcept -> Result<void>
		{
			return CallProjected([this, &end]
			{
				this->Get().End(end);
			});
		}

		[[nodiscard]] auto GetStart() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().Start() };
			});
		}
		[[nodiscard]] auto SetStart(const Point2F& start) const noexcept -> Result<void>
		{
			return CallProjected([this, &start]
			{
				this->Get().Start(start);
			});
		}
	};
}

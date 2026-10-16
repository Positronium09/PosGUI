module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Geometry.CompositionRoundedRectangleGeometry;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.D2D.D2DRoundedRectangleGeometry;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Geometry.CompositionGeometry;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionRoundedRectangleGeometry : public CompositionGeometryT<WUC::CompositionRoundedRectangleGeometry>
	{
		public:
		using CompositionGeometryT::CompositionGeometryT;

		[[nodiscard]] static auto Create(const RoundedRect& rect) noexcept -> Result<CompositionRoundedRectangleGeometry>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &rect]
			{
				const auto geometry = compositor.CreateRoundedRectangleGeometry();
				geometry.Offset(rect.TopLeft());
				geometry.Size(rect.Size());
				geometry.CornerRadius({ rect.xRadius, rect.yRadius });

				return CompositionRoundedRectangleGeometry{ geometry };
			});
		}
		[[nodiscard]] static auto Create(const RectF& rect, const Point2F& cornerRadius) noexcept -> Result<CompositionRoundedRectangleGeometry>
		{
			return Create(RoundedRect{ rect, cornerRadius.x, cornerRadius.y });
		}
		[[nodiscard]] static auto CreateFrom(const D2D::D2DRoundedRectangleGeometry& g) noexcept -> Result<CompositionRoundedRectangleGeometry>
		{
			return Create(g.GetRoundedRect());
		}

		[[nodiscard]] auto GetCornerRadius() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().CornerRadius() };
			});
		}
		[[nodiscard]] auto SetCornerRadius(const Point2F& cornerRadius) const noexcept -> Result<void>
		{
			return CallProjected([this, &cornerRadius]
			{
				Get().CornerRadius(cornerRadius);
			});
		}

		[[nodiscard]] auto GetOffset() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().Offset() };
			});
		}
		[[nodiscard]] auto SetOffset(const Point2F& offset) const noexcept -> Result<void>
		{
			return CallProjected([this, &offset]
			{
				Get().Offset(offset);
			});
		}

		[[nodiscard]] auto GetSize() const noexcept -> Result<SizeF>
		{
			return CallProjected([this]
			{
				return SizeF{ Get().Size() };
			});
		}
		[[nodiscard]] auto SetSize(const SizeF& size) const noexcept -> Result<void>
		{
			return CallProjected([this, &size]
			{
				Get().Size(size);
			});
		}
	};
}

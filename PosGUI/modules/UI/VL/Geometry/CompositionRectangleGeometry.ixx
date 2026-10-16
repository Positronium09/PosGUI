module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Geometry.CompositionRectangleGeometry;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.D2D.D2DRectangleGeometry;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Geometry.CompositionGeometry;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class [[deprecated("This API contains a known issue in Windows 10, versions 2004 and below. Don't use this API in your code")]] 
	CompositionRectangleGeometry : public CompositionGeometryT<WUC::CompositionRectangleGeometry>
	{
		public:
		using CompositionGeometryT::CompositionGeometryT;
		/*
		[[nodiscard]] static auto Create(const RectF& rect) noexcept -> Result<CompositionRectangleGeometry>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &rect]
			{
				auto geometry = compositor.CreateRectangleGeometry();
				geometry.Offset(rect.TopLeft());
				geometry.Size(rect.Size());

				return CompositionRectangleGeometry{ geometry };
			});
		}

		[[nodiscard]] static auto CreateFrom(const D2D::D2DRectangleGeometry& g) noexcept -> Result<CompositionRectangleGeometry>
		{
			return Create(g.GetRect());
		}
		*/

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

		[[nodiscard]] auto GetRectangle() const noexcept -> Result<RectF>
		{
			return CallProjected([this]
			{
				return RectF{ 
					static_cast<Point2F>(Get().Offset()), 
					static_cast<SizeF>(Get().Size())
				};
			});
		}
		[[nodiscard]] auto SetRectangle(const RectF& rect) const noexcept -> Result<void>
		{
			if (auto result = SetOffset(rect.TopLeft());
				!result.has_value())
			{
				return Unexpected{ result.error().AddDetail(L"part", L"Offset") };
			}
			if (auto result = SetSize(rect.Size());
				!result.has_value())
			{
				return Unexpected{ result.error().AddDetail(L"part", L"Size") };
			}

			return EmptyResult;
		}
	};
}

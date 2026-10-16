module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Clip.CompositionGeometricClip;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionViewBox;
import PGUI.UI.VL.Clip.CompositionClip;
import PGUI.UI.VL.Geometry.CompositionGeometry;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionGeometricClip : public CompositionClipT<WUC::CompositionGeometricClip>
	{
		public:
		using CompositionClipT::CompositionClipT;

		[[nodiscard]] static auto Create(
			const CompositionGeometry& geometry) noexcept -> Result<CompositionGeometricClip>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &geometry]
			{
				return CompositionGeometricClip{ compositor.CreateGeometricClip(geometry.Get()) };
			});
		}

		[[nodiscard]] auto GetGeometry() const noexcept -> Result<CompositionGeometry>
		{
			return CallProjected([this]
			{
				return CompositionGeometry{ Get().Geometry() };
			});
		}
		[[nodiscard]] auto SetGeometry(const CompositionGeometry& geometry) const noexcept -> Result<void>
		{
			return CallProjected([this, &geometry]
			{
				Get().Geometry(geometry.Get());
			});
		}

		[[nodiscard]] auto GetViewBox() const noexcept -> Result<CompositionViewBox>
		{
			return CallProjected([this]
			{
				return CompositionViewBox{ Get().ViewBox() };
			});
		}
		[[nodiscard]] auto SetViewBox(const CompositionViewBox& viewBox) const noexcept -> Result<void>
		{
			return CallProjected([this, &viewBox]
			{
				Get().ViewBox(viewBox.Get());
			});
		}
	};
}

module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.ContainerVisual;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.Visual;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.VisualCollection;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::ContainerVisual> Projected = WUC::ContainerVisual>
	class ContainerVisualT : public VisualT<Projected>
	{
		public:
		using Base = VisualT<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetChildren() const noexcept -> Result<VisualCollection>
		{
			return CallProjected([this]
			{
				return VisualCollection{ this->Get().Children() };
			});
		}
	};
	using ContainerVisual = ContainerVisualT<>;

	template <HandleDerivedFrom<WUC::Visual> Projected>
	[[nodiscard]] auto GetParentOf(const VisualT<Projected>& visual) noexcept -> Result<ContainerVisual>
	{
		return CallProjected([&visual]
		{
			return ContainerVisual{ visual.Get().Parent() };
		});
	}
}

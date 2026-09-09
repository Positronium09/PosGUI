module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.ContainerVisual;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.Compositor;
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

		[[nodiscard]] static auto Create() noexcept -> Result<ContainerVisualT>
			requires std::same_as<Projected, WUC::ContainerVisual>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return ContainerVisualT{ compositor.CreateContainerVisual() };
			});
		}
		[[nodiscard]] static auto Create(const Point3F& position, const SizeF& size) noexcept -> Result<ContainerVisualT>
			requires std::same_as<Projected, WUC::ContainerVisual>
		{
			return Create().and_then([position, size](const ContainerVisualT& visual) noexcept -> Result<ContainerVisualT>
			{
				if (auto ret = visual.SetPosition(position);
					!ret.has_value())
				{
					return Unexpected{ ret.error().SetCustomMessage(L"Failed to set position") };
				}
				if (auto ret = visual.SetSize(size);
					!ret.has_value())
				{
					return Unexpected{ ret.error().SetCustomMessage(L"Failed to set size") };
				}
				return visual;
			});
		}

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

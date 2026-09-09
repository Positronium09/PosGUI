module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionSurface;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::ICompositionSurface> Projected = WUC::ICompositionSurface>
	class CompositionSurfaceT : public Wrapper<Projected>
	{
		public:
		using Base = Wrapper<Projected>;
		using Base::Base;

	};
	using CompositionSurface = CompositionSurfaceT<>;
}

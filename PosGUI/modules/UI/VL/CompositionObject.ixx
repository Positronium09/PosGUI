module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionObject;

import PGUI.Wrapper;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionObject> Projected = WUC::CompositionObject>
	class CompositionObjectT : public Wrapper<Projected>
	{
		public:
		using Base = Wrapper<Projected>;

		using Base::Base;
	};
	using CompositionObject = CompositionObjectT<>;
}

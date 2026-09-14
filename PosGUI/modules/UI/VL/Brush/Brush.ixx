module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Brush.CompositionBrush;

import std;

import PGUI.Wrapper;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionBrush> Projected = WUC::CompositionBrush>
	class CompositionBrushT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;
	};
	using CompositionBrush = CompositionBrushT<>;
}

module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	using BackfaceVisibility = WUC::CompositionBackfaceVisibility;
	using BorderMode = WUC::CompositionBorderMode;
	using CompositeMode = WUC::CompositionCompositeMode;
}

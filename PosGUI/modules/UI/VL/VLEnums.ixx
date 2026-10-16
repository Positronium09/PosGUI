module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

export module PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL
{
	using BackfaceVisibility = WUC::CompositionBackfaceVisibility;
	using BorderMode = WUC::CompositionBorderMode;
	using CompositeMode = WUC::CompositionCompositeMode;
	using Stretch = WUC::CompositionStretch;
	using BitmapInterpolationMode = WUC::CompositionBitmapInterpolationMode;
	using AnimationDelayBehavior = WUC::AnimationDelayBehavior;
	using AnimationDirection = WUC::AnimationDirection;
	using AnimationIterationBehavior = WUC::AnimationIterationBehavior;
	using AnimationStopBehavior = WUC::AnimationStopBehavior;
	using ColorSpace = WUC::CompositionColorSpace;
	using EasingFunctionMode = WUC::CompositionEasingFunctionMode;
	using ChainingMode = WUCI::InteractionChainingMode;
	using InteractionSourceMode = WUCI::InteractionSourceMode;
	using InteractionSourceRedirectionMode = WUCI::InteractionSourceRedirectionMode;
	using ManipulationRedirectionMode = WUCI::VisualInteractionSourceRedirectionMode;
	using ClampingOption = WUCI::InteractionTrackerClampingOption;
	using PositionUpdateOption = WUCI::InteractionTrackerPositionUpdateOption;
	using InteractionBindingAxisModes = WUCI::InteractionBindingAxisModes;
	using InteractionSourceRedirectionMode = WUCI::InteractionSourceRedirectionMode;
	using GetValueStatus = WUC::CompositionGetValueStatus;
}

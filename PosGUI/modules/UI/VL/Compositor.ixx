module;
#include <winrt/Windows.UI.Composition.h>

#include <windows.ui.composition.interop.h>

#include <winrt/Windows.System.h>

export module PGUI.UI.VL.Compositor;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace WUC = winrt::Windows::UI::Composition;
namespace WSystem = winrt::Windows::System;
using IDesktopInterop = ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop;

namespace PGUI::UI::VL::Compositor::Internal
{
	WUC::Compositor compositor{ nullptr };
	WSystem::DispatcherQueueController dispatcherQueueController{ nullptr };

	ComPtr<IDesktopInterop> desktopInterop{ nullptr };
}

export namespace PGUI::UI::VL::Compositor
{
	auto InitDispatcherQueueController() -> void;
	[[nodiscard]] auto GetCompositor() noexcept -> Result<WUC::Compositor>;
	[[nodiscard]] auto GetDesktopInterop() noexcept -> Result<ComPtr<IDesktopInterop>>;
}

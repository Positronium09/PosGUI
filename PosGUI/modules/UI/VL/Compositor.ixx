module;
#include <winrt/Windows.UI.Composition.h>

#include <windows.ui.composition.interop.h>

export module PGUI.UI.VL.Compositor;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace WUC = winrt::Windows::UI::Composition;
namespace AWUC = ABI::Windows::UI::Composition;
using IDesktopInterop = ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop;

export namespace PGUI::UI::VL::Compositor
{
	auto InitDispatcherQueueController() -> void;
	auto Shutdown() -> void;
	[[nodiscard]] auto GetCompositor() noexcept -> Result<WUC::Compositor>;
	[[nodiscard]] auto GetDesktopInterop() noexcept -> Result<ComPtr<IDesktopInterop>>;
	[[nodiscard]] auto GetCompositorInterop() noexcept -> Result<ComPtr<AWUC::ICompositorInterop>>;
}

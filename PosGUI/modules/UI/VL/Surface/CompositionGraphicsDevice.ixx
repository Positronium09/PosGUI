module;
#include <winrt/Windows.UI.Composition.h>

#include <windows.ui.composition.interop.h>

export module PGUI.UI.VL.Surface.CompositionGraphicsDevice;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace WUC = winrt::Windows::UI::Composition;
namespace AWUC = ABI::Windows::UI::Composition;

using IGraphicsDeviceInterop = ABI::Windows::UI::Composition::ICompositionGraphicsDeviceInterop;

export namespace PGUI::UI::VL::CompositionGraphicsDevice
{
	auto Shutdown() -> void;

	[[nodiscard]] auto GetGraphicsDevice() noexcept -> Result<WUC::CompositionGraphicsDevice>;
	[[nodiscard]] auto GetGraphicsDeviceInterop() noexcept -> Result<ComPtr<IGraphicsDeviceInterop>>;

	[[nodiscard]] auto Trim() noexcept -> Result<void>;
}

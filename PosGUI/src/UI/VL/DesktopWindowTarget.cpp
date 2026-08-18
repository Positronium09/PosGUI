module;
#include <winrt/Windows.UI.Composition.Desktop.h>

#include <windows.ui.composition.desktop.h>

#include <windows.ui.composition.interop.h>

module PGUI.UI.VL.DesktopWindowTarget;

import PGUI.Window;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.VL.Compositor;

namespace WUCD = winrt::Windows::UI::Composition::Desktop;
using IDesktopWindowTarget = ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget;

namespace PGUI::UI::VL
{
	auto DesktopWindowTarget::Create(
		const RawWindowPtr<> wnd,
		const bool isTopMost) noexcept -> Result<DesktopWindowTarget>
	{
		DesktopWindowTarget target;

		const auto desktopInterop = Compositor::GetDesktopInterop();
		if (!desktopInterop.has_value())
		{
			return Unexpected{ desktopInterop.error() };
		}
		const auto& interop = desktopInterop.value();

		if (auto error = Error{
				interop->CreateDesktopWindowTarget(
					wnd->Hwnd(),
					isTopMost,
					ProjectionPut<IDesktopWindowTarget>(target.Get()))
			};
			error.IsFailure())
		{
			return Unexpected{
				error
				.AddDetail(L"wnd.Hwnd", std::format(L"{:p}", static_cast<void*>(wnd->Hwnd())))
				.AddDetail(L"isTopMost", std::format(L"{}", isTopMost))
			};
		}

		return target;
	}
}

module;
#include <winrt/Windows.UI.Composition.Desktop.h>

export module PGUI.UI.VL.DesktopWindowTarget;

import PGUI.Window;
import PGUI.ProjectionHolder;
import PGUI.ErrorHandling;

namespace WUCD = winrt::Windows::UI::Composition::Desktop;

export namespace PGUI::UI::VL
{
	class DesktopWindowTarget : public ProjectionHolder<WUCD::DesktopWindowTarget>
	{
		public:
		[[nodiscard]] static auto Create(RawWindowPtr<> wnd, bool isTopMost = false) noexcept -> Result<DesktopWindowTarget>;
	};
}

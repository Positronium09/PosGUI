module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

module PGUI.UI.VLWindow;

import PGUI.Utils;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.VL.Compositor;

namespace PGUI::UI
{
	VLWindow::VLWindow(const WindowClassPtr& wndClass) noexcept :
		Window{ wndClass }
	{
		RegisterHandler(WM_CREATE, &VLWindow::OnCreate);
		RegisterHandler(
			WM_PAINT,
			[](const HWND hwnd, const MessageID, const Argument1,
			   const Argument2) noexcept -> MessageHandlerResult
			{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);

				return 0;
			});
	}

	auto VLWindow::SetRoot(const VL::Visual& visual) const noexcept -> Result<void>
	{
		return desktopWindowTarget.SetRoot(visual);
	}

	auto VLWindow::OnCreate(MessageID, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		auto result = VL::DesktopWindowTarget::Create(this, false);
		if (!result.has_value())
		{
			Logger::Error(result.error(), L"Failed to create desktop window target");
			return 1;
		}

		desktopWindowTarget = MoveChecked(result.value());

		return 0;
	}
}

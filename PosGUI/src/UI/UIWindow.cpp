module;
#include <Windows.h>

module PGUI.UI.UIWindow;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIWindow::UIWindow() noexcept :
		UIWindow{ WindowClass::Create(L"PGUI_UIWindow") }
	{ }

	UIWindow::UIWindow(const WindowClassPtr& wndClass) noexcept :
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_LBUTTONDOWN, [this](UINT, WPARAM, LPARAM lParam) -> MessageHandlerResult
		{
			PointF pt = MAKEPOINTS(lParam);
			pt = PhysicalToLogical(pt);

			for (const auto& component : components)
			{
				if (component->GetBounds().IsPointInside(pt))
				{
					component->clickedEvent.Invoke(pt);
				}
			}

			return 0;
		});
	}
}

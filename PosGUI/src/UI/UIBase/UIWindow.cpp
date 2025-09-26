module PGUI.UI.UIBase:UIWindow;

import std;

import :UIElement;
import :UIContainer;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIWindow::UIWindow() :
		DirectXCompositionWindow{ WindowClass::Create(L"PGUI_UIWindow") }
	{ }

	UIWindow::UIWindow(const WindowClassPtr& wndClass) :
		DirectXCompositionWindow{ wndClass }
	{ }

	auto UIWindow::Draw(const Graphics graphics) -> void
	{
		BeginDraw();

		Render(graphics);
		childrenContainer.Render(graphics);

		EndDraw();
	}
}

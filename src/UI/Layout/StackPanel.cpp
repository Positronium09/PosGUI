module PGUI.UI.Layout.StackPanel;

import PGUI.WindowClass;

namespace PGUI::UI::Layout
{
	StackPanel::StackPanel() :
		StackPanel{ WindowClass::Create(L"PGUI_StackPanel") }
	{
	}

	StackPanel::StackPanel(const WindowClassPtr& wndClass) :
		UIComponent{ wndClass }
	{
		RegisterHandler(WM_USER_LAYOUT_UPDATE, &StackPanel::OnLayoutUpdate);
	}

	StackPanel::StackPanel(Orientation orientation) :
		StackPanel{ WindowClass::Create(L"PGUI_StackPanel"), orientation }
	{
	}

	StackPanel::StackPanel(const WindowClassPtr& wndClass, Orientation orientation) :
		UIComponent{ wndClass },
		stackLayout{ orientation }
	{
		RegisterHandler(WM_USER_LAYOUT_UPDATE, &StackPanel::OnLayoutUpdate);
	}
}
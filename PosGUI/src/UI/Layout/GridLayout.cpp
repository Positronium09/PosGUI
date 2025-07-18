module;
#include <Windows.h>

module PGUI.UI.Layout.GridLayout;

import PGUI.WindowClass;

namespace PGUI::UI::Layout
{
	GridLayout::GridLayout() noexcept :
		LayoutPanel{ WindowClass::Create(L"PGUI_GridLayout") }
	{ }

	auto GridLayout::SetRowGap(const long gap) noexcept -> void
	{
		rowGap = gap;
		RearrangeChildren();
	}

	auto GridLayout::SetColumnGap(const long gap) noexcept -> void
	{
		columnGap = gap;
		RearrangeChildren();
	}

	auto GridLayout::SetItemProperty(const HWND hwnd, const GridItemProperties& properties) -> void
	{
		itemProperties[hwnd] = properties;
		RearrangeChildren();
	}

	auto GridLayout::SetMinCellSize(const long size) noexcept -> void
	{
		minCellSize = size;
		RearrangeChildren();
	}

	auto GridLayout::RearrangeChildren() noexcept -> void
	{
		//
	}

	auto GridLayout::OnChildAdded(const WindowPtr<Window>& wnd) -> void
	{
		SetItemProperty(wnd->Hwnd(), GridItemProperties{ });
		RearrangeChildren();
	}

	auto GridLayout::OnChildRemoved(const HWND hwnd) -> void
	{
		itemProperties.erase(hwnd);
		RearrangeChildren();
	}
}

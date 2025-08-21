module;
#include <Windows.h>

module PGUI.UI.Layout.DockLayout;

import std;

import PGUI.Utils;
import PGUI.WindowClass;
import PGUI.UI.Layout.LayoutPanel;

namespace PGUI::UI::Layout
{
	DockLayout::DockLayout() noexcept :
		LayoutPanel{ WindowClass::Create(L"PGUI_DockLayout") }
	{
		dockPriorities.insert_or_assign(DockPosition::None, DockPriority::None);
		dockPriorities.insert_or_assign(DockPosition::Fill, DockPriority::None);
		dockPriorities.insert_or_assign(DockPosition::Top, DockPriority::First);
		dockPriorities.insert_or_assign(DockPosition::Left, DockPriority::Second);
		dockPriorities.insert_or_assign(DockPosition::Right, DockPriority::Third);
		dockPriorities.insert_or_assign(DockPosition::Bottom, DockPriority::Fourth);
	}

	auto DockLayout::SetDockPosition(const HWND hwnd, const DockPosition position) noexcept -> Error
	{
		if (dockPositions.contains(hwnd))
		{
			dockPositions[hwnd] = position;
			RearrangeChildren();
			return Error{ S_OK };
		}

		return Error{ E_INVALIDARG }
			.SuggestFix(L"No child window with given hwnd");
	}

	auto DockLayout::SetMaxDockSize(const DockPosition position, float size) noexcept -> Error
	{
		if (size < 0.0F)
		{
			return Error{ E_INVALIDARG }
				.SuggestFix(L"Size cannot be negative");
		}
		if (position == DockPosition::None)
		{
			return Error{ E_INVALIDARG }
				.SuggestFix(L"Cannot set max size for DockPosition::None");
		}
		if (position == DockPosition::Fill)
		{
			return Error{ E_INVALIDARG }
				.SuggestFix(L"Cannot set max size for DockPosition::Fill");
		}
		if (size == 0.0F)
		{
			ClearMaxDockSize(position);
			return Error{ S_OK };
		}
		if (maxDockSizes.contains(position) && maxDockSizes[position] == size)
		{
			return Error{ S_OK };
		}

		maxDockSizes.insert_or_assign(position, size);
		RearrangeChildren();

		return Error{ S_OK };
	}

	auto DockLayout::ClearMaxDockSize(const DockPosition position) noexcept -> void
	{
		if (maxDockSizes.contains(position))
		{
			maxDockSizes.erase(position);
		}
	}

	auto DockLayout::SetDockPriority(
		const DockPosition position,
		const DockPriority priority) noexcept -> void
	{
		if (position == DockPosition::None || position == DockPosition::Fill)
		{
			return;
		}
		if (dockPriorities.at(position) == priority)
		{
			return;
		}

		const auto& [toSwap, samePriority] = *std::ranges::find_if(
			dockPriorities,
			[priority](const auto& pair)
			{
				return pair.second == priority;
			});

		dockPriorities.insert_or_assign(toSwap, dockPriorities.at(position));
		dockPriorities[position] = priority;
		RearrangeChildren();
	}

	auto DockLayout::RearrangeChildren() noexcept -> void
	{
		std::vector<std::pair<HWND, DockPosition>> dockedItems;
		dockedItems.reserve(dockPositions.size());

		for (const auto& [hwnd, position] : dockPositions)
		{
			dockedItems.emplace_back(hwnd, position);
		}
		std::ranges::stable_sort(
			dockedItems,
			[this](const auto& a, const auto& b)
			{
				const auto priorityA = dockPriorities.at(a.second);
				const auto priorityB = dockPriorities.at(b.second);
				return ToUnderlying(priorityA) < ToUnderlying(priorityB);
			});

		const auto clientRect = GetClientRect();
		auto availableSpace = clientRect;

		for (const auto& [hwnd, position] : dockedItems)
		{
			const auto childWindow = GetChildWindow(hwnd);
			switch (position)
			{
				case DockPosition::Top:
				{
					auto topHeight = childWindow->GetClientSize().cy;
					if (maxDockSizes.contains(DockPosition::Top) &&
					    availableSpace.top + topHeight > maxDockSizes[DockPosition::Top])
					{
						topHeight = maxDockSizes[DockPosition::Top] - availableSpace.top;
					}
					const auto rect = RectF{
						availableSpace.left,
						availableSpace.top,
						availableSpace.right,
						availableSpace.top + topHeight
					};
					childWindow->MoveAndResize(rect);
					availableSpace.top += topHeight;
					break;
				}
				case DockPosition::Bottom:
				{
					auto bottomHeight = childWindow->GetClientSize().cy;
					if (maxDockSizes.contains(DockPosition::Bottom) &&
					    clientRect.bottom - availableSpace.bottom + bottomHeight > maxDockSizes[DockPosition::Bottom])
					{
						bottomHeight = maxDockSizes[DockPosition::Bottom] - clientRect.bottom + availableSpace.bottom;
					}
					const auto rect = RectF{
						availableSpace.left,
						availableSpace.bottom - bottomHeight,
						availableSpace.right,
						availableSpace.bottom
					};
					childWindow->MoveAndResize(rect);
					availableSpace.bottom -= bottomHeight;
					break;
				}
				case DockPosition::Left:
				{
					auto leftWidth = childWindow->GetClientSize().cx;
					if (maxDockSizes.contains(DockPosition::Left) &&
					    availableSpace.left + leftWidth > maxDockSizes[DockPosition::Left])
					{
						leftWidth = maxDockSizes[DockPosition::Left] - availableSpace.left;
					}
					const auto rect = RectF{
						availableSpace.left,
						availableSpace.top,
						availableSpace.left + leftWidth,
						availableSpace.bottom
					};
					childWindow->MoveAndResize(rect);
					availableSpace.left += leftWidth;
					break;
				}
				case DockPosition::Right:
				{
					auto rightWidth = childWindow->GetClientSize().cx;
					if (maxDockSizes.contains(DockPosition::Right) &&
					    clientRect.right - availableSpace.right + rightWidth > maxDockSizes[DockPosition::Right])
					{
						rightWidth = maxDockSizes[DockPosition::Right] - clientRect.right + availableSpace.right;
					}
					const auto rect = RectF{
						availableSpace.right - rightWidth,
						availableSpace.top,
						availableSpace.right,
						availableSpace.bottom
					};
					childWindow->MoveAndResize(rect);
					availableSpace.right -= rightWidth;
					break;
				}
				case DockPosition::Fill:
				{
					childWindow->MoveAndResize(availableSpace);
					break;
				}
				case DockPosition::None:
				{
					break;
				}
			}
		}
	}

	auto DockLayout::OnChildAdded(const WindowPtr<Window>& wnd) -> void
	{
		dockPositions.insert_or_assign(wnd->Hwnd(), DockPosition::None);
		RearrangeChildren();
	}

	auto DockLayout::OnChildRemoved(const HWND hwnd) -> void
	{
		dockPositions.erase(hwnd);
		RearrangeChildren();
	}
}

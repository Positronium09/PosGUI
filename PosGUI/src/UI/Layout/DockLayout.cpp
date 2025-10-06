module;
#include <Windows.h>

module PGUI.UI.Layout.DockLayout;

import std;

import PGUI.Utils;
import PGUI.WindowClass;
import PGUI.UI.Layout.LayoutPanel;

namespace PGUI::UI::Layout
{
	DockLayout::DockLayout(const RectF bounds) noexcept :
		LayoutPanel{ bounds }
	{
		dockPriorities.insert_or_assign(DockPosition::None, DockPriority::None);
		dockPriorities.insert_or_assign(DockPosition::Fill, DockPriority::None);
		dockPriorities.insert_or_assign(DockPosition::Top, DockPriority::First);
		dockPriorities.insert_or_assign(DockPosition::Left, DockPriority::Second);
		dockPriorities.insert_or_assign(DockPosition::Right, DockPriority::Third);
		dockPriorities.insert_or_assign(DockPosition::Bottom, DockPriority::Fourth);
	}

	auto DockLayout::SetDockPosition(const LayoutItem& item, const DockPosition position) -> void
	{
		if (const auto result = GetItemIndex(item);
			result.has_value())
		{
			SetDockPosition(*result, position);
		}
	}

	auto DockLayout::GetDockPosition(const LayoutItem& item) const noexcept -> Result<DockPosition>
	{
		const auto result = GetItemIndex(item);
		if (result.has_value())
		{
			return GetItemPosition(*result);
		}
		return Unexpected{ result.error() };
	}

	auto DockLayout::SetMaxDockSize(const DockPosition position, float size) noexcept -> Error
	{
		if (size < 0.0F)
		{
			return Error{ ErrorCode::InvalidArgument }
				.SuggestFix(L"Size cannot be negative");
		}
		if (position == DockPosition::None)
		{
			return Error{ ErrorCode::InvalidArgument }
				.SuggestFix(L"Cannot set max size for DockPosition::None");
		}
		if (position == DockPosition::Fill)
		{
			return Error{ ErrorCode::InvalidArgument }
				.SuggestFix(L"Cannot set max size for DockPosition::Fill");
		}
		if (size == 0.0F)
		{
			ClearMaxDockSize(position);
			return Error{ ErrorCode::Success };
		}
		if (maxDockSizes.contains(position) && maxDockSizes[position] == size)
		{
			return Error{ ErrorCode::Success };
		}

		maxDockSizes.insert_or_assign(position, size);
		RearrangeItems();

		return Error{ ErrorCode::Success };
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
		RearrangeItems();
	}

	auto DockLayout::SetDockPosition(const std::size_t id, const DockPosition position) noexcept -> void
	{
		if (dockPositions.contains(id))
		{
			dockPositions[id] = position;
			RearrangeItems();
		}
	}

	auto DockLayout::GetItemPosition(const std::size_t id) const noexcept -> Result<DockPosition>
	{
		if (dockPositions.contains(id))
		{
			return dockPositions.at(id);
		}
		return Unexpected{ Error{ ErrorCode::InvalidArgument } };
	}

	auto DockLayout::RearrangeItems() noexcept -> void
	{
		std::vector<std::pair<std::size_t, DockPosition>> dockedItems;
		dockedItems.reserve(dockPositions.size());

		for (const auto& [id, position] : dockPositions)
		{
			dockedItems.emplace_back(id, position);
		}
		std::ranges::stable_sort(
			dockedItems,
			[this](const auto& a, const auto& b)
			{
				const auto priorityA = dockPriorities.at(a.second);
				const auto priorityB = dockPriorities.at(b.second);
				return ToUnderlying(priorityA) < ToUnderlying(priorityB);
			});

		const auto space = GetRect();
		auto availableSpace = space;

		for (auto& [id, position] : dockedItems)
		{
			auto item = *GetItem(id);
			switch (position)
			{
				case DockPosition::Top:
				{
					auto topHeight = MeasureItem(item).cy;
					if (maxDockSizes.contains(DockPosition::Top) &&
					    availableSpace.top + topHeight > maxDockSizes[DockPosition::Top])
					{
						topHeight = maxDockSizes[DockPosition::Top] - availableSpace.top;
					}
					const auto itemRect = RectF{
						availableSpace.left,
						availableSpace.top,
						availableSpace.right,
						availableSpace.top + topHeight
					};
					ArrangeItem(item, itemRect);
					availableSpace.top += topHeight;
					break;
				}
				case DockPosition::Bottom:
				{
					auto bottomHeight = MeasureItem(item).cy;
					if (maxDockSizes.contains(DockPosition::Bottom) &&
					    space.bottom - availableSpace.bottom + bottomHeight > maxDockSizes[DockPosition::Bottom])
					{
						bottomHeight = maxDockSizes[DockPosition::Bottom] - space.bottom + availableSpace.bottom;
					}
					const auto itemRect = RectF{
						availableSpace.left,
						availableSpace.bottom - bottomHeight,
						availableSpace.right,
						availableSpace.bottom
					};
					ArrangeItem(item, itemRect);
					availableSpace.bottom -= bottomHeight;
					break;
				}
				case DockPosition::Left:
				{
					auto leftWidth = MeasureItem(item).cx;
					if (maxDockSizes.contains(DockPosition::Left) &&
					    availableSpace.left + leftWidth > maxDockSizes[DockPosition::Left])
					{
						leftWidth = maxDockSizes[DockPosition::Left] - availableSpace.left;
					}
					const auto itemRect = RectF{
						availableSpace.left,
						availableSpace.top,
						availableSpace.left + leftWidth,
						availableSpace.bottom
					};
					ArrangeItem(item, itemRect);
					availableSpace.left += leftWidth;
					break;
				}
				case DockPosition::Right:
				{
					auto rightWidth = MeasureItem(item).cx;
					if (maxDockSizes.contains(DockPosition::Right) &&
					    space.right - availableSpace.right + rightWidth > maxDockSizes[DockPosition::Right])
					{
						rightWidth = maxDockSizes[DockPosition::Right] - space.right + availableSpace.right;
					}
					const auto itemRect = RectF{
						availableSpace.right - rightWidth,
						availableSpace.top,
						availableSpace.right,
						availableSpace.bottom
					};
					ArrangeItem(item, itemRect);
					availableSpace.right -= rightWidth;
					break;
				}
				case DockPosition::Fill:
				{
					ArrangeItem(item, availableSpace);
					break;
				}
				case DockPosition::None:
				{
					break;
				}
			}
		}
	}

	auto DockLayout::OnItemAdded(const LayoutItem& layoutItem) -> void
	{
		if (const auto result = GetItemIndex(layoutItem);
			result.has_value() && !dockPositions.contains(*result))
		{
			LayoutPanel::OnItemAdded(layoutItem);
			return;
		}
		dockPositions.insert_or_assign(GetItemCount(), DockPosition::None);
		LayoutPanel::OnItemAdded(layoutItem);
	}

	auto DockLayout::OnItemRemoved(const std::size_t id) -> void
	{
		dockPositions.erase(id);
		LayoutPanel::OnItemRemoved(id);
	}
}

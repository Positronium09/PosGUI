module;
#include <Windows.h>

module PGUI.UI.Layout.GridLayout;

import PGUI.WindowClass;

namespace PGUI::UI::Layout
{
	auto OccupySet(std::set<std::pair<long, long>>& set,
	               const long row, const long column,
	               const long rowSpan, const long columnSpan) noexcept -> void
	{
		for (auto r = row; r < row + rowSpan; r++)
		{
			for (auto c = column; c < column + columnSpan; c++)
			{
				set.emplace(r, c);
			}
		}
	}

	auto IsOccupied(const std::set<std::pair<long, long>>& set,
	                const long row, const long column,
	                const long rowSpan, const long columnSpan) noexcept
	{
		for (auto r = row; r < row + rowSpan; r++)
		{
			for (auto c = column; c < column + columnSpan; c++)
			{
				if (set.contains({ r, c }))
				{
					return true;
				}
			}
		}

		return false;
	}

	auto IsUnoccupied(const std::set<std::pair<long, long>>& set,
	                  const long row, const long column,
	                  const long rowSpan, const long columnSpan) noexcept
	{
		return !IsOccupied(set, row, column, rowSpan, columnSpan);
	}

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
		needsSorting = true;

		const auto result = HasEntry(hwnd);
		if (!result.has_value())
		{
			itemProperties.emplace_back(hwnd, properties);
			return;
		}

		itemProperties.at(*result).second = properties;

		RearrangeChildren();
	}

	auto GridLayout::GetItemProperty(const HWND hwnd) const noexcept -> Result<GridItemProperties>
	{
		auto result = HasEntry(hwnd);
		if (result.has_value())
		{
			return itemProperties.at(*result).second;
		}

		auto& error = result.error();
		Logger::Error(error, L"Cannot find item property for");
		return Unexpected{ error };
	}

	auto GridLayout::SetMinCellSize(const long size) noexcept -> void
	{
		minCellSize = size;
		RearrangeChildren();
	}

	auto GridLayout::RearrangeChildren() noexcept -> void
	{
		//TODO(Drunk) keep track of occupied cells with LinearPosition and keep track of the next available cell
		//TODO(Drunk) no sorting needed for performance
		//TODO(Drunk) if columnspan + column > available then move to the next row
		//TODO(Drunk) if two manually placed windows overlap allow overlapping
		//TODO(Drunk) if the next is filled skip and update the next available cell
		//TODO(Drunk) with custom grid and row spans there can be empty cells ignore those
		//TODO(Drunk) if column or row is out of range just expand the grid with 1fr and 1fr for missing columns and rows
		//TODO(Drunk) rest is easy dawg u can do it

		if (needsSorting)
		{
			SortProperties();
			needsSorting = false;
		}

		std::set<std::pair<long, long>> occupied;

		for (const auto& [hwnd, properties] : itemProperties)
		{ }

		for (const auto& [hwnd, properties] : itemProperties)
		{
			// defined row auto column placement
		}

		for (const auto& [hwnd, properties] : itemProperties)
		{
			// defined row auto column placement
		}

		for (const auto& [hwnd, properties] : itemProperties)
		{
			// defined row auto column placement
		}
	}

	auto GridLayout::SortProperties() noexcept -> void
	{
		std::ranges::stable_sort(
			itemProperties,
			[](const auto& lhs, const auto& rhs) noexcept
			{
				const GridItemProperties& leftProperties = lhs.second;
				const GridItemProperties& rightProperties = rhs.second;
				const auto leftDefined = leftProperties.row != AUTO_PLACE && leftProperties.column != AUTO_PLACE;
				const auto rightDefined = rightProperties.row != AUTO_PLACE && rightProperties.column != AUTO_PLACE;

				const auto leftLinearOrder = *leftProperties.row + *leftProperties.column;
				const auto rightLinearOrder = *rightProperties.row + *rightProperties.column;

				if (leftDefined && rightDefined)
				{
					return leftLinearOrder < rightLinearOrder;
				}
				if (leftDefined)
				{
					return true;
				}
				return false;
			});

		const auto firstUndefined = std::ranges::find_if(
			itemProperties,
			[](const auto& item) noexcept
			{
				return item.second.row == AUTO_PLACE || item.second.column == AUTO_PLACE;
			});
		const auto firstUndefinedIndex = std::distance(itemProperties.begin(), firstUndefined);

		std::ranges::stable_sort(
			itemProperties | std::views::drop(firstUndefinedIndex),
			[](const auto& lhs, const auto& rhs) noexcept
			{
				const GridItemProperties& leftProperties = lhs.second;
				const GridItemProperties& rightProperties = rhs.second;

				return *leftProperties.row < *rightProperties.row;
			});

		const auto lastDefinedRow = std::ranges::find_if(
			itemProperties | std::views::drop(firstUndefinedIndex),
			[](const auto& item) noexcept
			{
				return item.second.row != AUTO_PLACE;
			});
		const auto lastDefinedRowIndex = std::distance(itemProperties.begin(), lastDefinedRow);

		std::ranges::stable_sort(
			itemProperties | std::views::drop(lastDefinedRowIndex),
			[](const auto& lhs, const auto& rhs) noexcept
			{
				const GridItemProperties& leftProperties = lhs.second;
				const GridItemProperties& rightProperties = rhs.second;

				return *leftProperties.column < *rightProperties.column;
			});
	}

	auto GridLayout::GetMaxDefinedRow() noexcept -> std::size_t
	{
		const auto lastDefinedRow = std::ranges::find_if(
			itemProperties,
			[](const auto& item) noexcept
			{
				return item.second.row != AUTO_PLACE;
			});
		const auto lastDefinedRowIndex = std::distance(itemProperties.begin(), lastDefinedRow);
		const auto lastRow = std::ranges::max_element(
			itemProperties | std::views::take(lastDefinedRowIndex),
			[](const auto& lhs, const auto& rhs) noexcept
			{
				return *lhs.second.row < *rhs.second.row;
			});
		return lastRow != itemProperties.end() ? *lastRow->second.row : 0;
	}

	auto GridLayout::OnChildAdded(const WindowPtr<Window>& wnd) -> void
	{
		SetItemProperty(wnd->Hwnd(), GridItemProperties{ });
		RearrangeChildren();
	}

	auto GridLayout::OnChildRemoved(const HWND hwnd) -> void
	{
		itemProperties.erase(std::ranges::find_if(
			itemProperties,
			[&hwnd](const auto& item) noexcept
			{
				return item.first == hwnd;
			}));
		RearrangeChildren();
	}
}

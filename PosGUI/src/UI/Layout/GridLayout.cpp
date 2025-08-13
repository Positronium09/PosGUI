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

	auto GridLayout::SetRowGap(const FixedSize gap) noexcept -> void
	{
		rowGap = gap;
		RearrangeChildren();
	}

	auto GridLayout::SetColumnGap(const FixedSize gap) noexcept -> void
	{
		columnGap = gap;
		RearrangeChildren();
	}

	auto GridLayout::SetGap(const FixedSize gap) noexcept -> void
	{
		rowGap = gap;
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

	auto GridLayout::SetMinCellSize(const FixedSize size) noexcept -> void
	{
		minCellSize = size;
		RearrangeChildren();
	}

	auto GridLayout::RearrangeChildren() noexcept -> void
	{
		if (GetChildWindows().empty())
		{
			return;
		}
		if (GetChildWindows().size() == 1)
		{
			const auto& child = GetChildWindows().front();
			child->MoveAndResize({ padding.left, padding.top }, GetClientSize());
			return;
		}

		if (needsSorting)
		{
			SortProperties();
			needsSorting = false;
		}

		const auto rowSizes = GetRowSizesFromDefinition();
		const auto columnSizes = GetColumnSizesFromDefinition();

		std::set<std::pair<long, long>> occupiedPositions;

		const auto placeFixedPosition = [&occupiedPositions, &rowSizes, &columnSizes, this](
			const long row, const long column,
			const long rowSpan, const long columnSpan) -> RectL
		{
			const PointL position{
				std::accumulate(
					columnSizes.begin(), columnSizes.begin() + column,
					0L) + column * columnGap + padding.left,
				std::accumulate(
					rowSizes.begin(), rowSizes.begin() + row,
					0L) + row * rowGap + padding.top
			};
			const SizeL size{
				std::accumulate(
					columnSizes.begin() + column,
					columnSizes.begin() + column + columnSpan,
					0L) + (columnSpan - 1) * columnGap,
				std::accumulate(
					rowSizes.begin() + row,
					rowSizes.begin() + row + rowSpan,
					0L) + (rowSpan - 1) * rowGap
			};

			OccupySet(occupiedPositions, row, column, rowSpan, columnSpan);

			return RectL{ position, size };
		};


		for (const auto& [hwnd, properties] : itemProperties)
		{
			const auto row = *properties.row;
			const auto column = *properties.column;
			const auto rowSpan = *properties.rowSpan;
			const auto columnSpan = *properties.columnSpan;
			auto actualRowSpan = rowSpan;
			if (row != AUTO_PLACE)
			{
				actualRowSpan = std::min(
					actualRowSpan,
					static_cast<long>(rowSizes.size()) - row
				);
			}
			auto actualColumnSpan = columnSpan;
			if (column != AUTO_PLACE)
			{
				actualColumnSpan = std::min(
					columnSpan,
					static_cast<long>(columnSizes.size()) - column
				);
			}

			const auto window = GetWindowPtrFromHWND(hwnd);

			RectL rect;
			if (row != AUTO_PLACE && column != AUTO_PLACE)
			{
				rect = placeFixedPosition(row, column, actualRowSpan, actualColumnSpan);
			}
			else if (row != AUTO_PLACE && column == AUTO_PLACE)
			{
				auto placed = false;
				long tryColumn = 0;

				while (tryColumn <= columnSizes.size() - actualColumnSpan && !placed)
				{
					if (IsOccupied(occupiedPositions, row, tryColumn, actualRowSpan, actualColumnSpan))
					{
						tryColumn++;
						continue;
					}
					rect = placeFixedPosition(row, tryColumn, actualRowSpan, actualColumnSpan);
					placed = true;
				}
				if (!placed)
				{
					tryColumn = static_cast<long>(columnSizes.size() - 1);
					while (IsOccupied(occupiedPositions, row, tryColumn, 1, 1) && tryColumn > 0)
					{
						tryColumn--;
					}
					rect = placeFixedPosition(row, tryColumn, actualRowSpan, actualColumnSpan);
				}
			}
			else if (row == AUTO_PLACE && column != AUTO_PLACE)
			{
				auto placed = false;
				long tryRow = 0;

				while (tryRow <= rowSizes.size() - actualRowSpan && !placed)
				{
					if (IsOccupied(occupiedPositions, tryRow, column, actualRowSpan, actualColumnSpan))
					{
						tryRow++;
						continue;
					}
					rect = placeFixedPosition(tryRow, column, actualRowSpan, actualColumnSpan);
					placed = true;
				}
				if (!placed)
				{
					tryRow = static_cast<long>(columnSizes.size() - 1);
					while (IsOccupied(occupiedPositions, tryRow, column, 1, 1) && tryRow > 0)
					{
						tryRow--;
					}
					rect = placeFixedPosition(tryRow, column, actualRowSpan, actualColumnSpan);
				}
			}
			else
			{
				long tryRow = 0;
				long tryColumn = 0;
				auto placed = false;

				while (tryRow <= rowSizes.size() - actualRowSpan && !placed)
				{
					while (tryColumn <= columnSizes.size() - actualColumnSpan && !placed)
					{
						if (IsOccupied(occupiedPositions, tryRow, tryColumn, actualRowSpan, actualColumnSpan))
						{
							tryColumn++;
							continue;
						}
						rect = placeFixedPosition(tryRow, tryColumn, actualRowSpan, actualColumnSpan);
						placed = true;
					}
					tryRow++;
					tryColumn = 0;
				}
				if (!placed)
				{
					tryRow = static_cast<long>(rowSizes.size() - 1);
					tryColumn = static_cast<long>(columnSizes.size() - 1);

					while (IsOccupied(occupiedPositions, tryRow, tryColumn, 1, 1) && tryRow > 0)
					{
						while (IsOccupied(occupiedPositions, tryRow, tryColumn, 1, 1) && tryColumn > 0)
						{
							tryColumn--;
						}
						tryColumn = static_cast<long>(columnSizes.size() - 1);
						tryRow--;
					}
					rect = placeFixedPosition(tryRow, tryColumn, actualRowSpan, actualColumnSpan);
				}
			}

			window->MoveAndResize(rect);
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

	auto GridLayout::GetRowSizesFromDefinition() const noexcept -> std::vector<long>
	{
		std::size_t maxDefinedRow = 0;

		if (const auto maxRow = std::ranges::max_element(
				itemProperties,
				[](const auto& lhs, const auto& rhs) noexcept
				{
					return *lhs.second.row + *lhs.second.rowSpan < *rhs.second.row + *rhs.second.rowSpan;
				});
			maxRow != itemProperties.end())
		{
			maxDefinedRow = *maxRow->second.row + *maxRow->second.rowSpan - 1;
		}

		std::size_t cellsNeeded = 0;
		for (const auto& properties : itemProperties | std::views::values)
		{
			cellsNeeded += *properties.rowSpan * *properties.columnSpan;
		}

		auto autoExpandedRowCount = static_cast<std::size_t>(
			std::ceil(static_cast<float>(GetChildWindows().size()) / columnDefinitions.size()));

		while (autoExpandedRowCount * columnDefinitions.size() < cellsNeeded)
		{
			autoExpandedRowCount++;
		}

		const auto rowCount = std::max({ maxDefinedRow + 1, rowDefinitions.size(), autoExpandedRowCount });
		const auto availableSpace = GetClientSize().cy - (
			                            padding.top + padding.bottom + static_cast<long>(rowCount - 1) * rowGap
		                            );
		const auto nonDefinedRowCount = std::clamp(
			rowCount - rowDefinitions.size(), 1ULL,
			std::numeric_limits<std::size_t>::max());
		auto remainingSpace = availableSpace;
		auto totalFractionalSize = 0.0F;
		for (const auto& definition : rowDefinitions)
		{
			if (std::holds_alternative<FractionalSize>(definition))
			{
				totalFractionalSize += std::get<FractionalSize>(definition);
			}
			else if (std::holds_alternative<FixedSize>(definition))
			{
				remainingSpace -= std::get<FixedSize>(definition);
			}
		}
		if (std::holds_alternative<FractionalSize>(autoRowSize))
		{
			totalFractionalSize += std::get<FractionalSize>(autoRowSize) *
				(static_cast<float>(nonDefinedRowCount) - 1.0F);
		}
		else
		{
			remainingSpace -= std::get<FixedSize>(autoRowSize) * static_cast<long>(nonDefinedRowCount);
		}

		std::vector<long> rowSizes;
		rowSizes.reserve(rowCount);

		for (const auto& definition : rowDefinitions)
		{
			if (std::holds_alternative<FixedSize>(definition))
			{
				rowSizes.push_back(std::get<FixedSize>(definition));
				continue;
			}
			const auto fractionalSize = std::get<FractionalSize>(definition);
			const auto size = static_cast<long>(remainingSpace * (fractionalSize / totalFractionalSize));
			rowSizes.push_back(size);
		}

		while (rowSizes.size() < rowCount)
		{
			if (std::holds_alternative<FixedSize>(autoRowSize))
			{
				rowSizes.push_back(std::get<FixedSize>(autoRowSize));
				continue;
			}

			const auto fractionalSize = std::get<FractionalSize>(autoRowSize);
			const auto size = static_cast<long>(remainingSpace * (fractionalSize / totalFractionalSize));
			rowSizes.push_back(size);
		}

		return rowSizes;
	}

	auto GridLayout::GetColumnSizesFromDefinition() const noexcept -> std::vector<long>
	{
		std::size_t maxDefinedColumn = 0;

		if (const auto maxRow = std::ranges::max_element(
				itemProperties,
				[](const auto& lhs, const auto& rhs) noexcept
				{
					return *lhs.second.column + *lhs.second.columnSpan < *rhs.second.column + *rhs.second.columnSpan;
				});
			maxRow != itemProperties.end())
		{
			maxDefinedColumn = *maxRow->second.column + *maxRow->second.columnSpan - 1;
		}

		const auto columnCount = std::max(maxDefinedColumn + 1, columnDefinitions.size());
		const auto availableSpace = GetClientSize().cx - (
			                            padding.left + padding.right + static_cast<long>(columnCount - 1) * columnGap
		                            );

		auto remainingSpace = availableSpace;
		auto totalFractionalSize = 0.0F;
		for (const auto& definition : columnDefinitions)
		{
			if (std::holds_alternative<FractionalSize>(definition))
			{
				totalFractionalSize += std::get<FractionalSize>(definition);
			}
			else if (std::holds_alternative<FixedSize>(definition))
			{
				remainingSpace -= std::get<FixedSize>(definition);
			}
		}

		std::vector<long> columnSizes;
		columnSizes.reserve(columnCount);
		for (const auto& definition : columnDefinitions)
		{
			if (std::holds_alternative<FixedSize>(definition))
			{
				columnSizes.push_back(std::get<FixedSize>(definition));
				continue;
			}
			const auto fractionalSize = std::get<FractionalSize>(definition);
			const auto size = static_cast<long>(remainingSpace * (fractionalSize / totalFractionalSize));
			columnSizes.push_back(size);
		}

		while (columnSizes.size() < columnCount)
		{
			if (std::holds_alternative<FixedSize>(autoRowSize))
			{
				columnSizes.push_back(std::get<FixedSize>(autoRowSize));
				continue;
			}
			const auto fractionalSize = std::get<FractionalSize>(autoRowSize);
			const auto size = static_cast<long>(remainingSpace * (fractionalSize / totalFractionalSize));
			columnSizes.push_back(size);
		}

		return columnSizes;
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

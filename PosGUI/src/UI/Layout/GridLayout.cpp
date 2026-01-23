module;
#include <Windows.h>

module PGUI.UI.Layout.GridLayout;

import PGUI.WindowClass;
import PGUI.UI.Layout.LayoutPanel;

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

	auto GridLayout::SetRowGap(const FixedSize gap) noexcept -> void
	{
		if (rowGap != gap)
		{
			rowGap = gap;
			RearrangeItems();
		}
	}

	auto GridLayout::SetColumnGap(const FixedSize gap) noexcept -> void
	{
		if (columnGap != gap)
		{
			columnGap = gap;
			RearrangeItems();
		}
	}

	auto GridLayout::SetGap(const FixedSize gap) noexcept -> void
	{
		if (rowGap != gap || columnGap != gap)
		{
			rowGap = gap;
			columnGap = gap;
			RearrangeItems();
		}
	}

	auto GridLayout::SetItemProperty(const LayoutItem& item, const GridItemProperties& properties) -> void
	{
		if (const auto result = GetItemId(item);
			result.has_value())
		{
			SetItemProperty(*result, properties);
		}
	}

	auto GridLayout::GetItemProperty(const LayoutItem& item) const noexcept -> Result<GridItemProperties>
	{
		const auto result = GetItemId(item);
		if (result.has_value())
		{
			return GetItemProperty(*result);
		}

		return Unexpected{ result.error() };
	}

	auto GridLayout::GetItemProperty(const std::size_t id) const noexcept -> Result<GridItemProperties>
	{
		auto result = HasEntry(id);
		if (result.has_value())
		{
			return itemProperties.at(*result).second;
		}
		return Unexpected{ result.error() };
	}

	auto GridLayout::SetMinCellSize(const FixedSize size) noexcept -> void
	{
		if (minCellSize != size)
		{
			minCellSize = size;
			RearrangeItems();
		}
	}

	auto GridLayout::RemoveColumnDefinitionAtIndex(const std::size_t index) noexcept -> Error
	{
		if (index >= columnDefinitions.size())
		{
			return Error{ ErrorCode::InvalidArgument };
		}

		if (columnDefinitions.size() == 1)
		{
			return Error{ ErrorCode::InvalidArgument }
				.SuggestFix(L"Cannot remove the last column definition");
		}

		columnDefinitions.erase(
			std::next(columnDefinitions.begin(), static_cast<std::ptrdiff_t>(index)));

		RearrangeItems();

		return Error{ ErrorCode::Success };
	}

	auto GridLayout::RemoveRowDefinitionAtIndex(const std::size_t index) noexcept -> Error
	{
		if (index >= rowDefinitions.size())
		{
			return Error{ ErrorCode::InvalidArgument };
		}

		rowDefinitions.erase(
			std::next(rowDefinitions.begin(), static_cast<std::ptrdiff_t>(index)));

		RearrangeItems();

		return Error{ ErrorCode::Success };
	}

	auto GridLayout::RearrangeItems() noexcept -> void
	{
		if (GetItemCount() == 0)
		{
			return;
		}

		if (needsSorting)
		{
			SortProperties();
			needsSorting = false;
		}

		long maxDefinedColumn = 0;
		long maxColumnSpan = 1;

		if (const auto maxColumn = std::ranges::max_element(
				itemProperties,
				[](const auto& lhs, const auto& rhs) noexcept
		{
			return *lhs.second.column < *rhs.second.column;
		});
			maxColumn != itemProperties.end())
		{
			maxDefinedColumn = std::max(maxDefinedColumn, *maxColumn->second.column);
		}
		if (const auto maxSpan = std::ranges::max_element(
				itemProperties,
				[](const auto& lhs, const auto& rhs) noexcept
				{
					return *lhs.second.columnSpan < *rhs.second.columnSpan;
				});
			maxSpan != itemProperties.end())
		{
			maxColumnSpan = *maxSpan->second.columnSpan;
		}

		maxDefinedColumn = std::max(maxDefinedColumn, static_cast<long>(columnDefinitions.size()));
		maxDefinedColumn += maxColumnSpan - 1;

		std::set<std::pair<long, long>> occupiedPositions = blankCells;
		std::unordered_map<std::size_t, std::tuple<long, long, long, long>> itemPositions;

		long maxPlacedRow = 0;
		const auto populateStructures = [&occupiedPositions, &itemPositions, &maxPlacedRow](
			const std::size_t id,
			const long row, const long column, const long rowSpan, const long columnSpan)
		{
			itemPositions[id] = std::make_tuple(row, column, rowSpan, columnSpan);
			OccupySet(occupiedPositions, row, column, rowSpan, columnSpan);
			maxPlacedRow = std::max(maxPlacedRow, row + rowSpan - 1);
		};

		for (const auto& [id, properties] : itemProperties)
		{
			const auto row = *properties.row;
			const auto column = *properties.column;
			const auto rowSpan = *properties.rowSpan;
			const auto columnSpan = *properties.columnSpan;
			const auto actualRowSpan = rowSpan;
			const auto actualColumnSpan = columnSpan;

			if (row != AUTO_PLACE && column != AUTO_PLACE)
			{
				populateStructures(id, row, column, actualRowSpan, actualColumnSpan);
			}
			else if (row != AUTO_PLACE && column == AUTO_PLACE)
			{
				auto placed = false;
				long tryColumn = 0;

				while (tryColumn <= maxDefinedColumn - actualColumnSpan && !placed)
				{
					if (IsOccupied(occupiedPositions, row, tryColumn, actualRowSpan, actualColumnSpan))
					{
						tryColumn++;
						continue;
					}
					populateStructures(id, row, column, actualRowSpan, actualColumnSpan);
					placed = true;
				}
				if (!placed)
				{
					tryColumn = maxDefinedColumn - 1;
					while (IsOccupied(occupiedPositions, row, tryColumn, 1, 1) && tryColumn > 0)
					{
						tryColumn--;
					}
					populateStructures(id, row, column, actualRowSpan, actualColumnSpan);
				}
			}
			else if (row == AUTO_PLACE && column != AUTO_PLACE)
			{
				long tryRow = 0;
				if (placementType == GridCellPlacementType::Appended)
				{
					tryRow = maxPlacedRow;
				}

				while (true)
				{
					if (IsUnoccupied(occupiedPositions, tryRow, column, actualRowSpan, actualColumnSpan))
					{
						populateStructures(id, row, column, actualRowSpan, actualColumnSpan);
						break;
					}
					tryRow++;
				}
			}
			else
			{
				long tryRow = 0;
				if (placementType == GridCellPlacementType::Appended)
				{
					tryRow = maxPlacedRow;
				}
				long tryColumn = 0;
				auto placed = false;

				while (!placed)
				{
					while (tryColumn <= maxDefinedColumn - actualColumnSpan)
					{
						if (IsUnoccupied(occupiedPositions, tryRow, tryColumn, actualRowSpan, actualColumnSpan))
						{
							populateStructures(id, tryRow, tryColumn, actualRowSpan, actualColumnSpan);
							placed = true;
							break;
						}
						tryColumn++;
					}
					tryRow++;
					tryColumn = 0;
				}
			}
		}

		if (const auto maxColumn = std::ranges::max_element(
			occupiedPositions,
			[](const auto& lhs, const auto& rhs)
			{
				return lhs.second < rhs.second;
			});
			maxColumn != occupiedPositions.end())
		{
			maxDefinedColumn = maxColumn->second;
		}

		std::vector<long> rowSizes = GetRowSizes(maxPlacedRow + 1);
		std::vector<long> columnSizes = GetColumnSizes(maxDefinedColumn + 1);

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

		for (const auto& [id, positions] : itemPositions)
		{
			const auto& [row, column, rowSpan, columnSpan] = positions;
			const auto itemRect = placeFixedPosition(row, column, rowSpan, columnSpan);
			ArrangeItem(*GetItem(id), itemRect);
		}
	}

	auto GridLayout::GetItemId(const LayoutItem& item) const noexcept -> Result<std::size_t>
	{
		return GetItemIndex(item);
	}

	auto GridLayout::SetItemProperty(std::size_t id, const GridItemProperties& properties) noexcept -> void
	{
		const auto boundChangeHandler = std::bind_front(&GridLayout::PropertyChangeHandler, this);
		const auto boundColumnSpanValidator = std::bind_front(&GridLayout::ColumnSpanValidator, this,
		                                                      *properties.column);

		const auto result = HasEntry(id);
		if (!result.has_value())
		{
			itemProperties.emplace_back(id, properties);
			auto& prop = itemProperties.back().second;

			prop.column.AddObserver(boundChangeHandler);
			prop.row.AddObserver(boundChangeHandler);
			prop.rowSpan.AddObserver(boundChangeHandler);
			prop.columnSpan.AddObserver(boundChangeHandler);
			prop.columnSpan.AddValidator(boundColumnSpanValidator);

			needsSorting = true;
			RearrangeItems();

			return;
		}

		auto& prop = itemProperties.at(*result).second;

		if (prop.column.Get() == properties.column.Get() &&
		    prop.row.Get() == properties.row.Get() &&
		    prop.rowSpan.Get() == properties.rowSpan.Get() &&
		    prop.columnSpan.Get() == properties.columnSpan.Get())
		{
			return;
		}

		prop.column.ClearObservers();
		prop.row.ClearObservers();
		prop.rowSpan.ClearObservers();
		prop.columnSpan.ClearObservers();
		prop.columnSpan.ClearValidators();

		prop = properties;

		prop.column.AddObserver(boundChangeHandler);
		prop.row.AddObserver(boundChangeHandler);
		prop.rowSpan.AddObserver(boundChangeHandler);
		prop.columnSpan.AddObserver(boundChangeHandler);
		prop.columnSpan.AddValidator(boundColumnSpanValidator);

		needsSorting = true;
		RearrangeItems();
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

	auto GridLayout::GetRowSizes(const std::size_t rowCount) const noexcept -> std::vector<long>
	{
		const auto availableSpace = GetSize().cy - (
			                            padding.top + padding.bottom + static_cast<long>(rowCount - 1) * rowGap
		                            );
		const auto nonDefinedRowCount = std::clamp(
			rowCount - rowDefinitions.size(), 0ULL,
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
		if (std::holds_alternative<FractionalSize>(autoCellSize))
		{
			totalFractionalSize += std::get<FractionalSize>(autoCellSize) * static_cast<float>(nonDefinedRowCount);
		}
		else
		{
			remainingSpace -= std::get<FixedSize>(autoCellSize) * static_cast<long>(nonDefinedRowCount);
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
			if (std::holds_alternative<FixedSize>(autoCellSize))
			{
				rowSizes.push_back(std::get<FixedSize>(autoCellSize));
				continue;
			}

			const auto fractionalSize = std::get<FractionalSize>(autoCellSize);
			const auto size = static_cast<long>(remainingSpace * (fractionalSize / totalFractionalSize));
			rowSizes.push_back(size);
		}

		for (auto& size : rowSizes)
		{
			if (size < minCellSize)
			{
				size = minCellSize;
			}
		}

		if (!growToFit)
		{
			return rowSizes;
		}

		const auto totalRowSize =
			std::accumulate(rowSizes.begin(),
			                rowSizes.end(), 0L) +
			(rowCount - 1) * rowGap;

		if (const auto totalSpace = GetSize().cy - (padding.top + padding.bottom);
			totalRowSize < totalSpace)
		{
			const auto delta = totalSpace - totalRowSize;
			const auto toAdd = static_cast<float>(delta) / rowCount;

			for (auto& size : rowSizes)
			{
				size += static_cast<long>(toAdd);
			}
		}

		return rowSizes;
	}

	auto GridLayout::GetColumnSizes(const std::size_t columnCount) const noexcept -> std::vector<long>
	{
		const auto availableSpace = GetSize().cx - (
			                            padding.left + padding.right + static_cast<long>(columnCount - 1) * columnGap
		                            );
		const auto nonDefinedColumnCount = std::clamp(
			columnCount - columnDefinitions.size(), 0ULL,
			std::numeric_limits<std::size_t>::max());
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
		if (std::holds_alternative<FractionalSize>(autoCellSize))
		{
			totalFractionalSize += std::get<FractionalSize>(autoCellSize) * static_cast<float>(nonDefinedColumnCount);
		}
		else
		{
			remainingSpace -= std::get<FixedSize>(autoCellSize) * static_cast<long>(nonDefinedColumnCount);
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
			if (std::holds_alternative<FixedSize>(autoCellSize))
			{
				columnSizes.push_back(std::get<FixedSize>(autoCellSize));
				continue;
			}
			const auto fractionalSize = std::get<FractionalSize>(autoCellSize);
			const auto size = static_cast<long>(remainingSpace * (fractionalSize / totalFractionalSize));
			columnSizes.push_back(size);
		}

		for (auto& size : columnSizes)
		{
			if (size < minCellSize)
			{
				size = minCellSize;
			}
		}

		if (!growToFit)
		{
			return columnSizes;
		}

		const auto totalColumnSize =
			std::accumulate(columnSizes.begin(),
			                columnSizes.end(), 0L) +
			(columnCount - 1) * columnGap;
		if (const auto totalSpace = GetSize().cx - (padding.left + padding.right);
			totalColumnSize < totalSpace)
		{
			const auto delta = totalSpace - totalColumnSize;
			const auto toAdd = static_cast<float>(delta) / columnCount;
			for (auto& size : columnSizes)
			{
				size += static_cast<long>(toAdd);
			}
		}

		return columnSizes;
	}

	auto GridLayout::OnItemAdded(const LayoutItem& layoutItem) -> void
	{
		LayoutPanel::OnItemAdded(layoutItem);
		SetItemProperty(GetItemCount() - 1, GridItemProperties{ });
	}

	auto GridLayout::OnItemRemoved(std::size_t size) -> void
	{
		itemProperties.erase(std::ranges::find_if(
			itemProperties,
			[&size](const auto& item) noexcept
			{
				return item.first == size;
			}));
		LayoutPanel::OnItemRemoved(size);
	}
}

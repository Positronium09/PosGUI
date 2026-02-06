module;
#include <Windows.h>

export module PGUI.UI.Layout.GridLayout;

import PGUI.Window;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.Layout.LayoutPanel;
import PGUI.DataBinding;
import PGUI.DataBinding.Property;

import std;

export namespace PGUI::UI::Layout
{
	using FixedSize = long;
	using FractionalSize = float;
	using GridCellDefinition = std::variant<FixedSize, FractionalSize>;
	constexpr auto AUTO_PLACE = -1;

	namespace GridLiterals
	{
		// ReSharper disable CppUserDefinedLiteralSuffixDoesNotStartWithUnderscore
		consteval auto operator""_fixed(const unsigned long long size) noexcept -> FixedSize
		{
			return static_cast<FixedSize>(size);
		}

		consteval auto operator""_fixed(const long double size) noexcept -> FixedSize
		{
			return static_cast<FixedSize>(size);
		}

		consteval auto operator""_fractional(const unsigned long long  size) noexcept -> FractionalSize
		{
			return static_cast<FractionalSize>(size);
		}

		consteval auto operator""_fractional(const long double size) noexcept -> FractionalSize
		{
			return static_cast<FractionalSize>(size);
		}
		// ReSharper restore CppUserDefinedLiteralSuffixDoesNotStartWithUnderscore
	}

	struct GridLayoutPadding
	{
		FixedSize top = 0;
		FixedSize left = 0;
		FixedSize bottom = 0;
		FixedSize right = 0;
	};

	enum class GridCellPlacementType
	{
		Packed,
		Appended
	};

	struct GridItemProperties final
	{
		GridItemProperties() noexcept :
			GridItemProperties{ AUTO_PLACE, AUTO_PLACE }
		{
		}
		GridItemProperties(
			const long row,
			const long column,
			const long rowSpan = 1,
			const long columnSpan = 1) noexcept :
			row{ row }, column{ column },
			rowSpan{ rowSpan }, columnSpan{ columnSpan }
		{
		}

		DataBinding::ValidatedPropertyNM<long> row{ AUTO_PLACE,
			{ [](const auto& val) static
			{
				return val >= AUTO_PLACE;
			} }
		};
		DataBinding::ValidatedPropertyNM<long> column{ AUTO_PLACE,
			{ [](const auto& val) static
			{
				return val >= AUTO_PLACE;
			} }
		};
		DataBinding::ValidatedPropertyNM<long> rowSpan{ 1,
			{ [](const auto& val) static
			{
				return val > 0;
			} }
		};
		DataBinding::ValidatedPropertyNM<long> columnSpan{ 1,
			{ [](const auto& val) static
			{
				return val > 0;
			} }
		};
	};

	class GridLayout final : public LayoutPanel
	{
		public:
		using LayoutPanel::LayoutPanel;

		auto SetRowGap(FixedSize gap) noexcept -> void;
		[[nodiscard]] auto GetRowGap() const noexcept
		{
			return rowGap;
		}
		auto SetColumnGap(FixedSize gap) noexcept -> void;
		[[nodiscard]] auto GetColumnGap() const noexcept
		{
			return columnGap;
		}

		auto SetGap(FixedSize gap) noexcept -> void;
		[[nodiscard]] auto GetGaps() const noexcept
		{
			return std::make_pair(rowGap, columnGap);
		}

		template <typename T>
		auto AddItem(const T& item, const GridItemProperties& properties) noexcept -> void
		{
			SetItemProperty(GetItemCount(), properties);
			LayoutPanel::AddItem(item);
		}

		auto SetItemProperty(const LayoutItem& item, const GridItemProperties& properties) -> void;

		[[nodiscard]] auto GetItemProperty(const LayoutItem& item) const noexcept -> Result<GridItemProperties>;

		auto SetMinCellSize(FixedSize size) noexcept -> void;
		[[nodiscard]] auto GetMinCellSize() const noexcept
		{
			return minCellSize;
		}

		auto SetColumnDefinitions(const std::vector<GridCellDefinition>& definitions) noexcept -> void
		{
			columnDefinitions = definitions;
			RearrangeItems();
		}
		[[nodiscard]] auto GetColumnDefinitions() const noexcept
		{
			return columnDefinitions;
		}
		auto AddColumnDefinition(const GridCellDefinition& definition) noexcept
		{
			columnDefinitions.push_back(definition);
			RearrangeItems();
		}
		auto SetRowDefinitions(const std::vector<GridCellDefinition>& definitions) noexcept -> void
		{
			rowDefinitions = definitions;
			RearrangeItems();
		}
		[[nodiscard]] auto GetRowDefinitions() const noexcept
		{
			return rowDefinitions;
		}
		auto AddRowDefinition(const GridCellDefinition& definition) noexcept
		{
			rowDefinitions.push_back(definition);
			RearrangeItems();
		}

		auto RemoveColumnDefinitionAtIndex(std::size_t index) noexcept -> Error;
		auto RemoveRowDefinitionAtIndex(std::size_t index) noexcept -> Error;

		auto SetGrowToFit(const bool grow) noexcept -> void
		{
			growToFit = grow;
			RearrangeItems();
		}
		[[nodiscard]] auto GetGrowToFit() const noexcept
		{
			return growToFit;
		}

		auto SetPadding(const GridLayoutPadding& newPadding) noexcept -> void
		{
			padding = newPadding;
			RearrangeItems();
		}
		[[nodiscard]] auto GetPadding() const noexcept
		{
			return padding;
		}

		auto SetAutoCellSize(const GridCellDefinition size) noexcept -> Error
		{
			autoCellSize = size;
			RearrangeItems();

			return Error{ ErrorCode::Success };
		}
		[[nodiscard]] auto GetAutoCellSize() const noexcept
		{
			return autoCellSize;
		}

		auto SetPlacementType(const GridCellPlacementType type) noexcept -> void
		{
			placementType = type;
			RearrangeItems();
		}
		[[nodiscard]] auto GetPlacementType() const noexcept
		{
			return placementType;
		}

		auto InsertBlankCell(const long row, const long column) noexcept -> Error
		{
			if (row <= AUTO_PLACE || column <= AUTO_PLACE)
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Cannot insert a blank cell with auto place");
			}
			blankCells.emplace(row, column);
			RearrangeItems();

			return Error{ ErrorCode::Success };
		}
		auto RemoveBlankCell(const long row, const long column) noexcept -> Error
		{
			if (row <= AUTO_PLACE || column <= AUTO_PLACE)
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Cannot remove a blank cell with auto place");
			}

			blankCells.erase(std::make_pair(row, column));
			RearrangeItems();
			return Error{ ErrorCode::Success };
		}

		protected:
		auto RearrangeItems() noexcept -> void override;

		private:
		FixedSize minCellSize = 5;
		FixedSize rowGap = 0;
		FixedSize columnGap = 0;
		GridCellDefinition autoCellSize = 1.0F;
		bool growToFit = false;
		GridCellPlacementType placementType = GridCellPlacementType::Packed;
		GridLayoutPadding padding{ 0, 0, 0, 0 };
		std::set<std::pair<long, long>> blankCells;

		std::vector<GridCellDefinition> columnDefinitions{
			GridCellDefinition{ 1.0F }
		};
		std::vector<GridCellDefinition> rowDefinitions;

		bool needsSorting = false;
		std::vector<std::pair<std::size_t, GridItemProperties>> itemProperties;

		auto GetItemId(const LayoutItem& item) const noexcept -> Result<std::size_t>;
		auto SetItemProperty(std::size_t id, const GridItemProperties& properties) noexcept -> void;
		auto GetItemProperty(std::size_t id) const noexcept -> Result<GridItemProperties>;
		auto HasEntry(std::size_t id) const noexcept -> Result<std::size_t>
		{
			const auto iter = std::ranges::find_if(
				itemProperties,
				[&id](const auto& item) noexcept
			{
				return item.first == id;
			});
			if (iter != itemProperties.end())
			{
				return std::distance(itemProperties.begin(), iter);
			}

			return Unexpected{
				Error{ ErrorCode::InvalidArgument }
			};
		}
		auto SortProperties() noexcept -> void;
		auto GetRowSizes(std::size_t rowCount) const noexcept -> std::vector<long>;
		auto GetColumnSizes(std::size_t columnCount) const noexcept -> std::vector<long>;

		auto PropertyChangeHandler(const long&) noexcept
		{
			needsSorting = true;
		}
		auto ColumnSpanValidator(const long& value, const long column) const noexcept
		{
			if (column == AUTO_PLACE)
			{
				return value > 0;
			}
			return column + value <= columnDefinitions.size();
		}

		auto OnItemAdded(const LayoutItem&) -> void override;
		auto OnItemRemoved(std::size_t) -> void override;
	};
}

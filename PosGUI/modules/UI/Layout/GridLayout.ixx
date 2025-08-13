module;
#include <Windows.h>

export module PGUI.UI.Layout.GridLayout;

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
		constexpr auto operator""_fixed(const unsigned long long size) noexcept -> FixedSize
		{
			return static_cast<FixedSize>(size);
		}

		constexpr auto operator""_fixed(const long double size) noexcept -> FixedSize
		{
			return static_cast<FixedSize>(size);
		}

		constexpr auto operator""_fractional(const unsigned long long  size) noexcept -> FractionalSize
		{
			return static_cast<FractionalSize>(size);
		}

		constexpr auto operator""_fractional(const long double size) noexcept -> FractionalSize
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

	struct GridItemProperties final
	{
		GridItemProperties() noexcept :
			GridItemProperties{ AUTO_PLACE, AUTO_PLACE }
		{ }
		GridItemProperties(
			const long row, 
			const long column, 
			const long rowSpan = 1, 
			const long columnSpan = 1) noexcept :
				row{ row }, column{ column },
				rowSpan{ rowSpan }, columnSpan{ columnSpan }
		{ }

		DataBinding::ValidatedPropertyNM<long> row{ AUTO_PLACE, 
			{ [](const auto& val)
			{
				return val >= AUTO_PLACE;
			} }
		};
		DataBinding::ValidatedPropertyNM<long> column{ AUTO_PLACE, 
			{ [](const auto& val)
			{
				return val >= AUTO_PLACE;
			} }
		};
		DataBinding::ValidatedPropertyNM<long> rowSpan{ 1,
			{ [](const auto& val)
			{
				return val > 0;
			} }
		};
		DataBinding::ValidatedPropertyNM<long> columnSpan{ 1,
			{ [](const auto& val)
			{
				return val > 0;
			} }
		};
	};

	class GridLayout : public LayoutPanel
	{
		public:
		GridLayout() noexcept;

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

		auto SetItemProperty(HWND hwnd, const GridItemProperties& properties) -> void;
		auto SetItemProperty(const RawWindowPtr<> wnd, const GridItemProperties& properties) -> void
		{
			SetItemProperty(wnd->Hwnd(), properties);
		}
		[[nodiscard]] auto GetItemProperty(HWND hwnd) const noexcept -> Result<GridItemProperties>;
		[[nodiscard]] auto GetItemProperty(const RawWindowPtr<> wnd) const noexcept
		{
			return GetItemProperty(wnd->Hwnd());
		}

		auto SetMinCellSize(FixedSize size) noexcept -> void;
		[[nodiscard]] auto GetMinCellSize() const noexcept
		{
			return minCellSize;
		}

		auto SetColumnDefinitions(const std::vector<GridCellDefinition>& definitions) noexcept -> void
		{
			columnDefinitions = definitions;
			RearrangeChildren();
		}
		[[nodiscard]] auto GetColumnDefinitions() const noexcept
		{
			return columnDefinitions;
		}
		auto AddColumnDefinition(const GridCellDefinition& definition) noexcept
		{
			columnDefinitions.push_back(definition);
			RearrangeChildren();
		}
		auto SetRowDefinitions(const std::vector<GridCellDefinition>& definitions) noexcept -> void
		{
			rowDefinitions = definitions;
			RearrangeChildren();
		}
		[[nodiscard]] auto GetRowDefinitions() const noexcept
		{
			return rowDefinitions;
		}
		auto AddRowDefinition(const GridCellDefinition& definition) noexcept
		{
			rowDefinitions.push_back(definition);
			RearrangeChildren();
		}

		auto SetPadding(const GridLayoutPadding& padding_) noexcept -> void
		{
			padding = padding_;
			RearrangeChildren();
		}
		[[nodiscard]] auto GetPadding() const noexcept
		{
			return padding;
		}

		auto SetAutoRowSize(const GridCellDefinition size) noexcept -> Error
		{
			autoRowSize = size;
			RearrangeChildren();

			return Error{ S_OK };
		}
		[[nodiscard]] auto GetAutoFillSize() const noexcept
		{
			return autoRowSize;
		}

		protected:
		auto RearrangeChildren() noexcept -> void override;

		private:
		FixedSize minCellSize = 5;
		FixedSize rowGap = 0;
		FixedSize columnGap = 0;
		GridCellDefinition autoRowSize = 100L;
		GridLayoutPadding padding{ 0, 0, 0, 0 };

		std::vector<GridCellDefinition> columnDefinitions{
			GridCellDefinition{ 1.0F }
		};
		std::vector<GridCellDefinition> rowDefinitions;

		bool needsSorting = false;
		std::vector<std::pair<HWND, GridItemProperties>> itemProperties;

		auto HasEntry(HWND hwnd) const noexcept -> Result<std::size_t>
		{
			const auto iter = std::ranges::find_if(
				itemProperties,
				[&hwnd](const auto& item) noexcept
				{
					return item.first == hwnd;
				});
			if (iter != itemProperties.end())
			{
				return std::distance(itemProperties.begin(), iter);
			}

			return Unexpected{
				Error{ E_INVALIDARG }
				.AddTag(ErrorTags::STL) };
		}
		auto SortProperties() noexcept -> void;
		auto GetRowSizesFromDefinition() const noexcept -> std::vector<long>;
		auto GetColumnSizesFromDefinition() const noexcept -> std::vector<long>;

		auto OnChildAdded(const WindowPtr<Window>&) -> void override;
		auto OnChildRemoved(HWND hwnd) -> void override;
	};
}

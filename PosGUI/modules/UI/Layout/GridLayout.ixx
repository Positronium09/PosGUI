module;
#include <Windows.h>

export module PGUI.UI.Layout.GridLayout;

import PGUI.UI.Layout.LayoutPanel;
import PGUI.DataBinding;
import PGUI.DataBinding.Property;

import std;

export namespace PGUI::UI::Layout
{

	using FixedSize = long;
	using FractionalSize = float;
	using GridCellDefinition = std::variant<FixedSize, FractionalSize>;

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

	struct GridItemProperties
	{
		GridItemProperties() noexcept :
			GridItemProperties{ -1, -1 }
		{ }
		GridItemProperties(
			const long row, 
			const long column, 
			const long rowSpan = 1, 
			const long columnSpan = 1) noexcept :
				row{ row }, column{ column },
				rowSpan{ rowSpan }, columnSpan{ columnSpan }
		{ }

		long row = -1;
		long column = -1;
		DataBinding::ValidatedPropertyNM<long> rowSpan = { 1, { [](const auto& val)
		{
			return val > 0;
		} } };
		DataBinding::ValidatedPropertyNM<long> columnSpan = { 1, { [](const auto& val)
		{
			return val > 0;
		} } };

		constexpr auto LinearOrder() const noexcept
		{
			return row + column;
		}

		[[nodiscard]] auto operator<=>(const GridItemProperties& other) const noexcept
		{
			return LinearOrder() <=> other.LinearOrder();
		}
	};

	class GridLayout : public LayoutPanel
	{
		public:
		GridLayout() noexcept;

		auto SetRowGap(long gap) noexcept -> void;
		[[nodiscard]] auto GetRowGap() const noexcept
		{
			return rowGap;
		}
		auto SetColumnGap(long gap) noexcept -> void;
		[[nodiscard]] auto GetColumnGap() const noexcept
		{
			return columnGap;
		}

		auto SetGap(const long gap) noexcept -> void
		{
			SetRowGap(gap);
			SetColumnGap(gap);
		}
		[[nodiscard]] auto GetGaps() const noexcept
		{
			return std::pair{ rowGap, columnGap };
		}

		auto SetItemProperty(HWND hwnd, const GridItemProperties& properties) -> void;
		auto SetItemProperty(const RawWindowPtr<> wnd, const GridItemProperties& properties) -> void
		{
			SetItemProperty(wnd->Hwnd(), properties);
		}
		[[nodiscard]] const auto& GetItemProperty(const HWND hwnd) const noexcept
		{
			return itemProperties.at(hwnd);
		}
		[[nodiscard]] const auto& GetItemProperty(const RawWindowPtr<> wnd) const noexcept
		{
			return GetItemProperty(wnd->Hwnd());
		}

		auto SetMinCellSize(long size) noexcept -> void;
		[[nodiscard]] auto GetMinCellSize() const noexcept
		{
			return minCellSize;
		}

		protected:
		auto RearrangeChildren() noexcept -> void override;

		private:
		long minCellSize = 5;
		long rowGap = 0;
		long columnGap = 0;
		
		std::map<HWND, GridItemProperties> itemProperties;
		std::vector<GridCellDefinition> columnDefinitions{
			GridCellDefinition{ 1.0F }
		};
		std::vector<GridCellDefinition> rowDefinitions;

		auto OnChildAdded(const WindowPtr<Window>&) -> void override;
		auto OnChildRemoved(HWND hwnd) -> void override;
	};
}

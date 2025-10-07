module;
#include <Windows.h>

export module PGUI.UI.Layout.DockLayout;

import std;

import PGUI.UI.Layout.LayoutPanel;

export namespace PGUI::UI::Layout
{
	enum class DockPosition
	{
		None = -1,
		Top,
		Left,
		Right,
		Bottom,
		Fill
	};

	enum class DockPriority
	{
		First,
		Second,
		Third,
		Fourth,
		None
	};

	class DockLayout : public LayoutPanel
	{
		public:
		explicit DockLayout(RectF bounds) noexcept;

		template <typename T>
		auto AddItem(const T& item, DockPosition position) noexcept -> void
		{
			dockPositions.insert_or_assign(GetItemCount() - 1, position);
			LayoutPanel::AddItem(item);
		}

		auto SetDockPosition(const LayoutItem& item, DockPosition position) -> void;
		auto SetDockPosition(std::size_t id, DockPosition position) noexcept -> void;

		[[nodiscard]] auto GetItemPosition(std::size_t id) const noexcept -> Result<DockPosition>;
		[[nodiscard]] auto GetDockPosition(const LayoutItem& item) const noexcept -> Result<DockPosition>;

		auto SetMaxDockSize(DockPosition position, float size) noexcept -> Error;
		auto ClearMaxDockSize(DockPosition position) noexcept -> void;

		auto SetDockPriority(DockPosition position, DockPriority priority) noexcept -> void;
		[[nodiscard]] auto GetDockPriority(const DockPosition position) const noexcept -> DockPriority
		{
			return dockPriorities.at(position);
		}

		private:
		std::unordered_map<std::size_t, DockPosition> dockPositions;
		std::unordered_map<DockPosition, float> maxDockSizes;
		std::unordered_map<DockPosition, DockPriority> dockPriorities;

		auto RearrangeItems() noexcept -> void override;

		auto OnItemAdded(const LayoutItem&) -> void override;
		auto OnItemRemoved(std::size_t id) -> void override;
	};
}

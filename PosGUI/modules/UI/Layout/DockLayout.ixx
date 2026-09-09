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
			LayoutPanel::AddItem(item);
			dockPositions.insert_or_assign(GetItemCount() - 1, position);
		}

		[[nodiscard]] auto SetDockPosition(const LayoutItem& item, DockPosition position) -> Result<void>;
		[[nodiscard]] auto SetDockPosition(std::size_t id, DockPosition position) noexcept -> Result<void>;

		[[nodiscard]] auto GetItemPosition(std::size_t id) const noexcept -> Result<DockPosition>;
		[[nodiscard]] auto GetDockPosition(const LayoutItem& item) const noexcept -> Result<DockPosition>;

		[[nodiscard]] auto SetMaxDockSize(DockPosition position, float size) noexcept -> Result<void>;
		auto ClearMaxDockSize(DockPosition position) noexcept -> void;

		[[nodiscard]] auto SetDockPriority(DockPosition position, DockPriority priority) noexcept -> Result<void>;
		[[nodiscard]] auto GetDockPriority(DockPosition position) const noexcept -> Result<DockPriority>;

		private:
		std::flat_map<std::size_t, DockPosition> dockPositions;
		std::flat_map<DockPosition, float> maxDockSizes;
		std::flat_map<DockPosition, DockPriority> dockPriorities;

		auto RearrangeItems() noexcept -> void override;

		auto OnItemAdded(const LayoutItem&) -> void override;
		auto OnItemRemoved(std::size_t id) -> void override;
	};
}

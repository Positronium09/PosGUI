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

		auto AddItem(const LayoutItem& item, DockPosition position) noexcept -> void;

		auto SetDockPosition(const LayoutItem& item, DockPosition position) -> void;
		auto SetDockPosition(const RawPtr<LayoutItem> item, const DockPosition position) -> void
		{
			SetDockPosition(*item, position);
		}
		auto SetDockPosition(const RawWindowPtr<> wnd, const DockPosition position) noexcept -> void
		{
			const LayoutItem item = wnd;
			SetDockPosition(item, position);
		}
		auto SetDockPosition(LayoutPanel& panel, const DockPosition position)
		{
			SetDockPosition(&panel, position);
		}

		[[nodiscard]] auto GetDockPosition(const LayoutItem& item) const noexcept -> Result<DockPosition>;
		[[nodiscard]] auto GetDockPosition(const RawPtr<LayoutItem> item) const noexcept -> Result<DockPosition>
		{
			return GetDockPosition(*item);
		}
		[[nodiscard]] auto GetDockPosition(const RawWindowPtr<> wnd) const noexcept
		{
			const LayoutItem item = wnd;
			return GetDockPosition(item);
		}
		[[nodiscard]] auto GetDockPosition(LayoutPanel& panel) const noexcept
		{
			return GetDockPosition(&panel);
		}

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

		auto AddItem(const LayoutItem& item) noexcept -> void override;

		auto SetDockPosition(std::size_t id, DockPosition position) noexcept -> void;
		[[nodiscard]] auto GetItemPosition(std::size_t id) const noexcept -> Result<DockPosition>;

		auto RearrangeItems() noexcept -> void override;

		auto OnItemRemoved(std::size_t id) -> void override;
	};
}

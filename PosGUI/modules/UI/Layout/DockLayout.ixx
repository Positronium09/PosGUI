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
		None = -1,
		First,
		Second,
		Third,
		Fourth
	};

	class DockLayout : public LayoutPanel
	{
		public:
		explicit DockLayout() noexcept;

		auto SetDockPosition(HWND hwnd, DockPosition position) noexcept -> Error;
		auto SetDockPosition(const WindowPtr<Window>& child, const DockPosition position) noexcept -> Error
		{
			return SetDockPosition(child->Hwnd(), position);
		}

		auto SetMaxDockSize(DockPosition position, float size) noexcept -> Error;
		auto ClearMaxDockSize(DockPosition position) noexcept -> void;

		auto SetDockPriority(DockPosition position, DockPriority priority) noexcept -> void;
		[[nodiscard]] auto GetDockPriority(const DockPosition position) const noexcept -> DockPriority
		{
			return dockPriorities.at(position);
		}

		private:
		std::unordered_map<HWND, DockPosition> dockPositions;
		std::unordered_map<DockPosition, float> maxDockSizes;
		std::unordered_map<DockPosition, DockPriority> dockPriorities;

		auto RearrangeChildren() noexcept -> void override;

		auto OnChildAdded(const WindowPtr<Window>&) -> void override;
		auto OnChildRemoved(HWND) -> void override;
	};
}

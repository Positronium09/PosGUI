module;
#include <Windows.h>

export module PGUI.UI.Layout.StackLayout;

import PGUI.UI.Graphics;
import PGUI.UI.Layout.LayoutEnums;
import PGUI.UI.Layout.LayoutStructs;
import PGUI.UI.Layout.LayoutPanel;

export namespace PGUI::UI::Layout
{
	struct StackLayoutPadding
	{
		long startPad = 0;
		long endingPad = 0;
		long crossStartPad = 0;
		long crossEndPad = 0;
	};

	class StackLayout : public LayoutPanel
	{
		public:
		explicit StackLayout(
			LayoutOrientation orientation,
			MainAxisAlignment mainAxisAlignment = MainAxisAlignment::Center,
			CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::Center,
			StackLayoutPadding padding = { 0, 0, 0, 0 },
			long gap = 0) noexcept;

		auto RearrangeChildren() noexcept -> void override;

		auto SetOrientation(LayoutOrientation orientation) noexcept -> void;
		[[nodiscard]] auto GetOrientation() const noexcept { return orientation; }

		auto SetMainAxisAlignment(MainAxisAlignment alignment) noexcept -> void;
		[[nodiscard]] auto GetMainAxisAlignment() const noexcept { return mainAxisAlignment; }

		virtual auto SetCrossAxisAlignment(CrossAxisAlignment alignment) noexcept -> void;
		[[nodiscard]] auto GetCrossAxisAlignment() const noexcept { return crossAxisAlignment; }

		auto SetGap(long gap) noexcept -> void;
		[[nodiscard]] auto GetGap() const noexcept { return gap; }

		auto SetPadding(StackLayoutPadding padding) noexcept -> void;
		[[nodiscard]] auto GetPadding() const noexcept { return padding; }

		protected:
		explicit StackLayout(
			std::wstring_view className,
			LayoutOrientation orientation,
			MainAxisAlignment mainAxisAlignment = MainAxisAlignment::Center,
			CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::Center,
			StackLayoutPadding padding = { 0, 0, 0, 0 },
			long gap = 0) noexcept;

		virtual auto RearrangeHorizontal() const noexcept -> void;
		virtual auto RearrangeVertical() const noexcept -> void;

		private:
		LayoutOrientation orientation;
		MainAxisAlignment mainAxisAlignment;
		CrossAxisAlignment crossAxisAlignment;
		StackLayoutPadding padding;
		long gap;

		auto OnChildAdded(const WindowPtr<Window>&) -> void override;
		auto OnChildRemoved(HWND) -> void override;
	};
}

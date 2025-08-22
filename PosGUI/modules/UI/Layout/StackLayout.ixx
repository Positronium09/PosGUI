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
		float startPad = 0;
		float endingPad = 0;
		float crossStartPad = 0;
		float crossEndPad = 0;

		auto operator==(const StackLayoutPadding&) const noexcept -> bool = default;
	};

	class StackLayout final : public LayoutPanel
	{
		public:
		explicit StackLayout(
			RectF bounds,
			LayoutOrientation orientation,
			MainAxisAlignment mainAxisAlignment = MainAxisAlignment::Center,
			CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::Center,
			WrapMode wrapMode = WrapMode::NoWrap,
			StackLayoutPadding padding = { 0, 0, 0, 0 },
			float gap = 0, float crossGap = 0) noexcept;

		auto RearrangeItems() noexcept -> void override;

		auto SetOrientation(LayoutOrientation orientation) noexcept -> void;
		[[nodiscard]] auto GetOrientation() const noexcept { return orientation; }

		auto SetMainAxisAlignment(MainAxisAlignment alignment) noexcept -> void;
		[[nodiscard]] auto GetMainAxisAlignment() const noexcept { return mainAxisAlignment; }

		auto SetCrossAxisAlignment(CrossAxisAlignment alignment) noexcept -> void;
		[[nodiscard]] auto GetCrossAxisAlignment() const noexcept { return crossAxisAlignment; }

		auto SetMainAxisGap(float gap) noexcept -> void;
		[[nodiscard]] auto GetMainAxisGap() const noexcept { return mainAxisGap; }

		auto SetCrossAxisGap(float crossGap) noexcept -> void;
		[[nodiscard]] auto GetCrossAxisGap() const noexcept { return crossAxisGap; }

		auto SetPadding(StackLayoutPadding padding) noexcept -> void;
		[[nodiscard]] auto GetPadding() const noexcept { return padding; }

		auto SetWrapMode(WrapMode wrapMode) noexcept -> void;
		[[nodiscard]] auto GetWrapMode() const noexcept { return wrapMode; }

		protected:
		auto RearrangeHorizontalNoWrap() const noexcept -> void;
		auto RearrangeVerticalNoWrap() const noexcept -> void;
		auto RearrangeHorizontalWrap() const noexcept -> void;
		auto RearrangeVerticalWrap() const noexcept -> void;

		private:
		LayoutOrientation orientation;
		MainAxisAlignment mainAxisAlignment;
		CrossAxisAlignment crossAxisAlignment;
		StackLayoutPadding padding;
		WrapMode wrapMode;
		float mainAxisGap;
		float crossAxisGap;

		auto RearrangeHorizontalRow(
			std::size_t startChildIndex, std::size_t endChildIndex,
			float yPosition, std::size_t rowCount) const noexcept -> void;
		auto RearrangeVerticalColumn(
			std::size_t startChildIndex, std::size_t endChildIndex,
			float xPosition, std::size_t columnCount) const noexcept -> void;
	};
}

export module PGUI.UI.Layout.WrapLayout;

import PGUI.UI.Layout.StackLayout;
import PGUI.UI.Layout.LayoutEnums;

export namespace PGUI::UI::Layout
{
	struct WrapLayoutPadding
	{
		long startPad = 0;
		long endingPad = 0;
		long crossStartPad = 0;
		long crossEndPad = 0;
	};


	class WrapLayout final : public StackLayout
	{
		public:
		explicit WrapLayout(
			LayoutOrientation orientation,
			MainAxisAlignment mainAxisAlignment = MainAxisAlignment::Center,
			CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::Center,
			StackLayoutPadding padding = { 0, 0, 0, 0 },
			long gap = 0,
			long crossGap = 0) noexcept;

		auto SetCrossGap(long _crossGap) noexcept -> void;
		[[nodiscard]] auto GetCrossGap() const noexcept -> long
		{
			return crossGap;
		}

		auto SetCrossAxisAlignment(CrossAxisAlignment alignment) noexcept -> void override;

		protected:
		auto RearrangeHorizontal() const noexcept -> void override;
		auto RearrangeVertical() const noexcept -> void override;

		private:
		long crossGap = 0;
		auto RearrangeHorizontalRow(
			std::size_t startChildIndex, std::size_t endChildIndex,
			long yPosition) const noexcept -> void;
		auto RearrangeVerticalColumn(
			std::size_t startChildIndex, std::size_t endChildIndex,
			long xPosition) const noexcept -> void;
	};
}

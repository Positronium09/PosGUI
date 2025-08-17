module;
#include <Windows.h>

module PGUI.UI.Layout.WrapLayout;

import std;

import PGUI.ErrorHandling;

namespace PGUI::UI::Layout
{
	WrapLayout::WrapLayout(
		const LayoutOrientation orientation,
		const MainAxisAlignment mainAxisAlignment,
		const CrossAxisAlignment crossAxisAlignment,
		const StackLayoutPadding padding,
		const long gap, const long crossGap) noexcept :
		StackLayout{
			L"PGUI_WrapLayout",
			orientation,
			mainAxisAlignment,
			crossAxisAlignment,
			padding,
			gap
		}, crossGap{ crossGap }
	{ }

	auto WrapLayout::SetCrossGap(const long _crossGap) noexcept -> void
	{
		crossGap = _crossGap;
		RearrangeChildren();
	}

	auto WrapLayout::SetCrossAxisAlignment(CrossAxisAlignment alignment) noexcept -> void
	{
		if (alignment == CrossAxisAlignment::Stretch)
		{
			Logger::Warning(L"CrossAxisAlignment::Stretch is not supported with WrapLayout");
			return;
		}
		StackLayout::SetCrossAxisAlignment(alignment);
	}

	auto WrapLayout::RearrangeHorizontal() const noexcept -> void
	{
		const auto clientSize = GetClientSize();
		const auto totalChildrenSize = GetTotalChildrenSize();
		const auto& stackLayoutPadding = GetPadding();
		const auto availableWidth = clientSize.cx - stackLayoutPadding.startPad - stackLayoutPadding.endingPad;
		const auto stackLayoutGap = GetGap();

		if (totalChildrenSize.cx + (GetChildWindows().size() - 1) * stackLayoutGap <= availableWidth)
		{
			StackLayout::RearrangeHorizontal();
			return;
		}

		std::vector<long> rowSizes;
		rowSizes.reserve(GetChildWindows().size());
		std::vector<std::size_t> startIndices;
		startIndices.reserve(GetChildWindows().size());
		startIndices.push_back(0);

		long currentRowSize = 0;
		long maxHeight = 0;

		for (const auto& [index, child] : GetChildWindows() | std::views::enumerate)
		{
			const auto childSize = child->GetClientSize();
			if (currentRowSize + childSize.cx > availableWidth)
			{
				rowSizes.push_back(maxHeight);
				startIndices.push_back(index);
				currentRowSize = 0;
				maxHeight = 0;
			}
			maxHeight = std::max(maxHeight, childSize.cy);
			currentRowSize += childSize.cx + stackLayoutGap;
		}
		while (rowSizes.size() < startIndices.size())
		{
			for (const auto& child : GetChildWindows() | std::views::drop(startIndices.back()))
			{
				maxHeight = std::max(maxHeight, child->GetClientSize().cy);
			}
			rowSizes.push_back(maxHeight);
		}

		auto currentY = stackLayoutPadding.crossStartPad;
		switch (GetCrossAxisAlignment())
		{
			case CrossAxisAlignment::Center:
			{
				const auto totalHeight =
					std::accumulate(rowSizes.begin(), rowSizes.end(), 0L) +
					static_cast<long>(rowSizes.size() - 1) * crossGap;
				const auto availableHeight = clientSize.cy - stackLayoutPadding.crossStartPad - stackLayoutPadding.
				                             crossEndPad;
				currentY += (availableHeight - totalHeight) / 2;
				break;
			}
			case CrossAxisAlignment::End:
			{
				const auto totalHeight =
					std::accumulate(rowSizes.begin(), rowSizes.end(), 0L) +
					static_cast<long>(rowSizes.size() - 1) * crossGap;
				const auto availableHeight = clientSize.cy - stackLayoutPadding.crossStartPad - stackLayoutPadding.
				                             crossEndPad;
				currentY += availableHeight - totalHeight;
				break;
			}
			default:
			{
				break;
			}
		}

		for (const auto& [index, rowSize] : rowSizes | std::views::enumerate)
		{
			const auto startIndex = startIndices[index];
			const auto endIndex = static_cast<std::size_t>(index + 1) < startIndices.size()
				? startIndices[index + 1] - 1
				: GetChildWindows().size() - 1;
			RearrangeHorizontalRow(startIndex, endIndex, currentY);
			currentY += rowSize + crossGap;
		}
	}

	auto WrapLayout::RearrangeVertical() const noexcept -> void
	{
		const auto clientSize = GetClientSize();
		const auto totalChildrenSize = GetTotalChildrenSize();
		const auto& stackLayoutPadding = GetPadding();
		const auto availableHeight = clientSize.cy - stackLayoutPadding.startPad - stackLayoutPadding.endingPad;
		const auto stackLayoutGap = GetGap();

		if (totalChildrenSize.cy + (GetChildWindows().size() - 1) * stackLayoutGap <= availableHeight)
		{
			StackLayout::RearrangeVertical();
			return;
		}

		std::vector<long> columnSizes;
		columnSizes.reserve(GetChildWindows().size());

		std::vector<std::size_t> startIndices;
		startIndices.reserve(GetChildWindows().size());
		startIndices.push_back(0);

		long currentColumnSize = 0;
		long maxWidth = 0;

		for (const auto& [index, child] : GetChildWindows() | std::views::enumerate)
		{
			const auto childSize = child->GetClientSize();
			if (currentColumnSize + childSize.cy > availableHeight)
			{
				columnSizes.push_back(maxWidth);
				startIndices.push_back(index);
				currentColumnSize = 0;
				maxWidth = 0;
			}
			maxWidth = std::max(maxWidth, childSize.cx);
			currentColumnSize += childSize.cy + stackLayoutGap;
		}

		while (columnSizes.size() < startIndices.size())
		{
			for (const auto& child : GetChildWindows() | std::views::drop(startIndices.back()))
			{
				maxWidth = std::max(maxWidth, child->GetClientSize().cx);
			}
			columnSizes.push_back(maxWidth);
		}

		auto currentX = stackLayoutPadding.crossStartPad;

		switch (GetCrossAxisAlignment())
		{
			case CrossAxisAlignment::Center:
			{
				const auto totalWidth =
					std::accumulate(columnSizes.begin(), columnSizes.end(), 0L) +
					static_cast<long>(columnSizes.size() - 1) * crossGap;
				const auto availableWidth = clientSize.cx -
				                            stackLayoutPadding.crossStartPad - stackLayoutPadding.crossEndPad;
				currentX += (availableWidth - totalWidth) / 2;
				break;
			}
			case CrossAxisAlignment::End:
			{
				const auto totalWidth =
					std::accumulate(columnSizes.begin(), columnSizes.end(), 0L) +
					static_cast<long>(columnSizes.size() - 1) * crossGap;
				const auto availableWidth = clientSize.cx -
				                            stackLayoutPadding.crossStartPad - stackLayoutPadding.crossEndPad;
				currentX += availableWidth - totalWidth;
				break;
			}
			default:
			{
				break;
			}
		}

		for (const auto& [index, columnSize] : columnSizes | std::views::enumerate)
		{
			const auto startIndex = startIndices[index];
			const auto endIndex = static_cast<std::size_t>(index + 1) < startIndices.size()
				? startIndices[index + 1] - 1
				: GetChildWindows().size() - 1;
			RearrangeVerticalColumn(startIndex, endIndex, currentX);
			currentX += columnSize + crossGap;
		}
	}

	auto WrapLayout::RearrangeHorizontalRow(const std::size_t startChildIndex,
	                                        const std::size_t endChildIndex,
	                                        long yPosition) const noexcept -> void
	{
		const auto& stackLayoutPadding = GetPadding();
		auto currentX = stackLayoutPadding.startPad;

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalWidth = std::accumulate(
					GetChildWindows().begin() + startChildIndex,
					GetChildWindows().begin() + endChildIndex + 1, 0L,
					[](long sum, const auto& child)
					{
						return sum + child->GetClientSize().cx;
					}) + static_cast<long>(endChildIndex - startChildIndex) * GetGap();
				const auto availableWidth = GetClientSize().cx -
				                            stackLayoutPadding.startPad - stackLayoutPadding.endingPad;
				currentX += (availableWidth - totalWidth) / 2;
				break;
			}
			case MainAxisAlignment::End:
			{
				const auto totalWidth = std::accumulate(
					GetChildWindows().begin() + startChildIndex,
					GetChildWindows().begin() + endChildIndex + 1, 0L,
					[](long sum, const auto& child)
					{
						return sum + child->GetClientSize().cx;
					}) + static_cast<long>(endChildIndex - startChildIndex) * GetGap();
				const auto availableWidth = GetClientSize().cx -
				                            stackLayoutPadding.startPad - stackLayoutPadding.endingPad;
				currentX += availableWidth - totalWidth;
				break;
			}
			default:
			{
				break;
			}
		}

		for (const auto& child : GetChildWindows() | std::views::drop(startChildIndex) |
		                         std::views::take(endChildIndex - startChildIndex + 1))
		{
			child->Move({ currentX, yPosition });
			currentX += child->GetClientSize().cx + GetGap();
			
		}
	}

	auto WrapLayout::RearrangeVerticalColumn(std::size_t startChildIndex,
	                                         std::size_t endChildIndex,
	                                         long xPosition) const noexcept -> void
	{
		const auto& stackLayoutPadding = GetPadding();
		auto currentY = stackLayoutPadding.startPad;

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalHeight = std::accumulate(
					                         GetChildWindows().begin() + startChildIndex,
					                         GetChildWindows().begin() + endChildIndex + 1, 0L,
					                         [](long sum, const auto& child)
					                         {
						                         return sum + child->GetClientSize().cy;
					                         }) + static_cast<long>(endChildIndex - startChildIndex) * GetGap();
				const auto availableHeight = GetClientSize().cy -
				                             stackLayoutPadding.startPad - stackLayoutPadding.endingPad;
				currentY += (availableHeight - totalHeight) / 2;
				break;
			}
			case MainAxisAlignment::End:
			{
				const auto totalHeight = std::accumulate(
					                         GetChildWindows().begin() + startChildIndex,
					                         GetChildWindows().begin() + endChildIndex + 1, 0L,
					                         [](long sum, const auto& child)
					                         {
						                         return sum + child->GetClientSize().cy;
					                         }) + static_cast<long>(endChildIndex - startChildIndex) * GetGap();
				const auto availableHeight = GetClientSize().cy -
				                             stackLayoutPadding.startPad - stackLayoutPadding.endingPad;
				currentY += availableHeight - totalHeight;
				break;
			}
			default:
			{
				break;
			}
		}

		for (const auto& child : GetChildWindows() | std::views::drop(startChildIndex) |
		                         std::views::take(endChildIndex - startChildIndex + 1))
		{
			child->Move({ xPosition, currentY });
			currentY += child->GetClientSize().cy + GetGap();
		}
	}
}

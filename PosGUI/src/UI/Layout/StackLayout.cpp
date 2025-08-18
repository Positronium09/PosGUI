module PGUI.UI.Layout.StackLayout;

import std;

import PGUI.WindowClass;
import PGUI.ErrorHandling;

namespace PGUI::UI::Layout
{
	StackLayout::StackLayout(
		const LayoutOrientation orientation,
		const MainAxisAlignment mainAxisAlignment, const CrossAxisAlignment crossAxisAlignment,
		const WrapMode wrapMode, const StackLayoutPadding padding,
		const long gap, const long crossGap) noexcept :
		LayoutPanel{ WindowClass::Create(L"PGUI_StackLayout") },
		orientation{ orientation }, mainAxisAlignment{ mainAxisAlignment },
		crossAxisAlignment{ crossAxisAlignment }, padding{ padding },
		wrapMode{ wrapMode },
		mainAxisGap{ gap }, crossAxisGap{ crossGap }
	{ }

	auto StackLayout::RearrangeChildren() noexcept -> void
	{
		if (orientation == LayoutOrientation::Horizontal)
		{
			if (wrapMode == WrapMode::Wrap)
			{
				RearrangeHorizontalWrap();
			}
			else
			{
				RearrangeHorizontalNoWrap();
			}
			return;
		}
		if (wrapMode == WrapMode::Wrap)
		{
			RearrangeVerticalWrap();
		}
		else
		{
			RearrangeVerticalNoWrap();
		}
	}

	auto StackLayout::SetMainAxisGap(const long _gap) noexcept -> void
	{
		if (mainAxisGap != _gap)
		{
			mainAxisGap = _gap;
			RearrangeChildren();
		}
	}

	auto StackLayout::SetCrossAxisGap(long _crossGap) noexcept -> void
	{
		if (crossAxisGap != _crossGap)
		{
			crossAxisGap = _crossGap;
			if (wrapMode == WrapMode::Wrap)
			{
				RearrangeChildren();
			}
		}
	}

	auto StackLayout::SetPadding(const StackLayoutPadding _padding) noexcept -> void
	{
		if (padding != _padding)
		{
			padding = _padding;
			RearrangeChildren();
		}
	}

	auto StackLayout::SetWrapMode(const WrapMode mode) noexcept -> void
	{
		if (wrapMode != mode)
		{
			wrapMode = mode;
			RearrangeChildren();
		}
	}

	auto StackLayout::SetOrientation(const LayoutOrientation _orientation) noexcept -> void
	{
		if (orientation != _orientation)
		{
			orientation = _orientation;
			RearrangeChildren();
		}
	}

	auto StackLayout::SetMainAxisAlignment(const MainAxisAlignment alignment) noexcept -> void
	{
		if (mainAxisAlignment != alignment)
		{
			mainAxisAlignment = alignment;
			RearrangeChildren();
		}
	}

	auto StackLayout::SetCrossAxisAlignment(const CrossAxisAlignment alignment) noexcept -> void
	{
		if (crossAxisAlignment == alignment)
		{
			return;
		}
		crossAxisAlignment = alignment;
		if (alignment == CrossAxisAlignment::Stretch && wrapMode == WrapMode::Wrap)
		{
			Logger::Warning(
				L"CrossAxisAlignment::Stretch cannot be used with WrapMode::Wrap reverting to CrossAxisAlignment::Center");
			crossAxisAlignment = CrossAxisAlignment::Center;
		}
		RearrangeChildren();
	}

	auto StackLayout::OnChildAdded(const WindowPtr<Window>&) -> void
	{
		RearrangeChildren();
	}

	auto StackLayout::OnChildRemoved(HWND) -> void
	{
		RearrangeChildren();
	}

	auto StackLayout::RearrangeHorizontalNoWrap() const noexcept -> void
	{
		const auto size = GetClientSize();
		const auto childrenSize = GetTotalChildrenSize().cx;
		const auto requiredSpace =
			childrenSize +
			static_cast<long>(GetChildWindows().size() - 1) * mainAxisGap;

		auto position = padding.startPad;

		if (mainAxisAlignment == MainAxisAlignment::End)
		{
			position = size.cx - requiredSpace - padding.endingPad;
		}
		else if (mainAxisAlignment == MainAxisAlignment::Center)
		{
			position = (size.cx + padding.startPad - padding.endingPad - requiredSpace) / 2;
		}

		for (const auto& child : GetChildWindows())
		{
			const auto childSize = child->GetClientSize();
			long top = 0;
			switch (crossAxisAlignment)
			{
				case CrossAxisAlignment::Start:
				{
					top = padding.crossStartPad;
					break;
				}
				case CrossAxisAlignment::Center:
				{
					top = padding.crossStartPad + (size.cy - childSize.cy) / 2;
					break;
				}
				case CrossAxisAlignment::End:
				{
					top = size.cy - padding.crossEndPad - childSize.cy;
					break;
				}
				case CrossAxisAlignment::Stretch:
				{
					child->Resize({ size.cy, childSize.cx });
					break;
				}
			}

			child->Move({ position, top });
			position += childSize.cx + mainAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalNoWrap() const noexcept -> void
	{
		const auto size = GetClientSize();
		const auto childrenSize = GetTotalChildrenSize().cy;
		const auto requiredSpace =
			childrenSize +
			static_cast<long>(GetChildWindows().size() - 1) * mainAxisGap;

		auto position = padding.startPad;

		if (mainAxisAlignment == MainAxisAlignment::End)
		{
			position = size.cy - requiredSpace - padding.endingPad;
		}
		else if (mainAxisAlignment == MainAxisAlignment::Center)
		{
			position = (size.cy + padding.startPad - padding.endingPad - requiredSpace) / 2;
		}

		for (const auto& child : GetChildWindows())
		{
			const auto childSize = child->GetClientSize();
			long left = 0;
			switch (crossAxisAlignment)
			{
				case CrossAxisAlignment::Start:
				{
					left = padding.crossStartPad;
					break;
				}
				case CrossAxisAlignment::Center:
				{
					left = (size.cx - childSize.cx) / 2;
					break;
				}
				case CrossAxisAlignment::End:
				{
					left = size.cx - padding.crossEndPad - childSize.cx;
					break;
				}
				case CrossAxisAlignment::Stretch:
				{
					child->Resize({ size.cx, childSize.cy });
					break;
				}
			}

			child->Move({ left, position });
			position += childSize.cy + mainAxisGap;
		}
	}

	auto StackLayout::RearrangeHorizontalWrap() const noexcept -> void
	{
		const auto clientSize = GetClientSize();
		const auto totalChildrenSize = GetTotalChildrenSize();
		const auto availableWidth = clientSize.cx - padding.startPad - padding.endingPad;
		const auto stackLayoutGap = GetMainAxisGap();

		if (totalChildrenSize.cx + (GetChildWindows().size() - 1) * stackLayoutGap <= availableWidth)
		{
			RearrangeHorizontalNoWrap();
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

		auto currentY = padding.crossStartPad;
		switch (GetCrossAxisAlignment())
		{
			case CrossAxisAlignment::Center:
			{
				const auto totalHeight =
					std::accumulate(rowSizes.begin(), rowSizes.end(), 0L) +
					static_cast<long>(rowSizes.size() - 1) * crossAxisGap;
				const auto availableHeight = clientSize.cy - padding.crossStartPad - padding.
				                             crossEndPad;
				currentY += (availableHeight - totalHeight) / 2;
				break;
			}
			case CrossAxisAlignment::End:
			{
				const auto totalHeight =
					std::accumulate(rowSizes.begin(), rowSizes.end(), 0L) +
					static_cast<long>(rowSizes.size() - 1) * crossAxisGap;
				const auto availableHeight = clientSize.cy - padding.crossStartPad - padding.
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
			currentY += rowSize + crossAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalWrap() const noexcept -> void
	{
		const auto clientSize = GetClientSize();
		const auto totalChildrenSize = GetTotalChildrenSize();
		const auto availableHeight = clientSize.cy - padding.startPad - padding.endingPad;
		const auto stackLayoutGap = GetMainAxisGap();

		if (totalChildrenSize.cy + (GetChildWindows().size() - 1) * stackLayoutGap <= availableHeight)
		{
			RearrangeVerticalNoWrap();
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

		auto currentX = padding.crossStartPad;

		switch (GetCrossAxisAlignment())
		{
			case CrossAxisAlignment::Center:
			{
				const auto totalWidth =
					std::accumulate(columnSizes.begin(), columnSizes.end(), 0L) +
					static_cast<long>(columnSizes.size() - 1) * crossAxisGap;
				const auto availableWidth = clientSize.cx -
				                            padding.crossStartPad - padding.crossEndPad;
				currentX += (availableWidth - totalWidth) / 2;
				break;
			}
			case CrossAxisAlignment::End:
			{
				const auto totalWidth =
					std::accumulate(columnSizes.begin(), columnSizes.end(), 0L) +
					static_cast<long>(columnSizes.size() - 1) * crossAxisGap;
				const auto availableWidth = clientSize.cx -
				                            padding.crossStartPad - padding.crossEndPad;
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
			currentX += columnSize + crossAxisGap;
		}
	}

	auto StackLayout::RearrangeHorizontalRow(
		const std::size_t startChildIndex,
		const std::size_t endChildIndex,
		long yPosition) const noexcept -> void
	{
		auto currentX = padding.startPad;

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalWidth =
					std::accumulate(
						GetChildWindows().begin() + startChildIndex,
						GetChildWindows().begin() + endChildIndex + 1, 0L,
						[](long sum, const auto& child)
						{
							return sum + child->GetClientSize().cx;
						}) + static_cast<long>(endChildIndex - startChildIndex) * GetMainAxisGap();
				const auto availableWidth = GetClientSize().cx -
				                            padding.startPad - padding.endingPad;
				currentX += (availableWidth - totalWidth) / 2;
				break;
			}
			case MainAxisAlignment::End:
			{
				const auto totalWidth =
					std::accumulate(
						GetChildWindows().begin() + startChildIndex,
						GetChildWindows().begin() + endChildIndex + 1, 0L,
						[](long sum, const auto& child)
						{
							return sum + child->GetClientSize().cx;
						}) + static_cast<long>(endChildIndex - startChildIndex) * GetMainAxisGap();
				const auto availableWidth = GetClientSize().cx -
				                            padding.startPad - padding.endingPad;
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
			currentX += child->GetClientSize().cx + GetMainAxisGap();
		}
	}

	auto StackLayout::RearrangeVerticalColumn(
		const std::size_t startChildIndex,
		const std::size_t endChildIndex,
		long xPosition) const noexcept -> void
	{
		auto currentY = padding.startPad;

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalHeight =
					std::accumulate(
						GetChildWindows().begin() + startChildIndex,
						GetChildWindows().begin() + endChildIndex + 1, 0L,
						[](long sum, const auto& child)
						{
							return sum + child->GetClientSize().cy;
						}) + static_cast<long>(endChildIndex - startChildIndex) * GetMainAxisGap();
				const auto availableHeight = GetClientSize().cy -
				                             padding.startPad - padding.endingPad;
				currentY += (availableHeight - totalHeight) / 2;
				break;
			}
			case MainAxisAlignment::End:
			{
				const auto totalHeight =
					std::accumulate(
						GetChildWindows().begin() + startChildIndex,
						GetChildWindows().begin() + endChildIndex + 1, 0L,
						[](long sum, const auto& child)
						{
							return sum + child->GetClientSize().cy;
						}) + static_cast<long>(endChildIndex - startChildIndex) * GetMainAxisGap();
				const auto availableHeight = GetClientSize().cy -
				                             padding.startPad - padding.endingPad;
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
			currentY += child->GetClientSize().cy + GetMainAxisGap();
		}
	}
}

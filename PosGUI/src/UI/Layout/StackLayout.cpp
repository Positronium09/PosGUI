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
		const float gap, const float crossGap) noexcept :
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

	auto StackLayout::SetMainAxisGap(const float _gap) noexcept -> void
	{
		if (mainAxisGap != _gap)
		{
			mainAxisGap = _gap;
			RearrangeChildren();
		}
	}

	auto StackLayout::SetCrossAxisGap(const float _crossGap) noexcept -> void
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
		if (crossAxisAlignment != alignment)
		{
			crossAxisAlignment = alignment;
			RearrangeChildren();
		}
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
			static_cast<float>(GetChildWindows().size() - 1) * mainAxisGap;

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
			float top = 0;
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
			static_cast<float>(GetChildWindows().size() - 1) * mainAxisGap;

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
			float left = 0;
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

		if (totalChildrenSize.cx + (GetChildWindows().size() - 1) * mainAxisGap <= availableWidth)
		{
			RearrangeHorizontalNoWrap();
			return;
		}

		std::vector<float> rowSizes;
		rowSizes.reserve(GetChildWindows().size());
		std::vector<std::size_t> startIndices;
		startIndices.reserve(GetChildWindows().size());
		startIndices.push_back(0);

		float currentRowSize = 0;
		float maxHeight = 0;

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
			currentRowSize += childSize.cx + mainAxisGap;
		}
		while (rowSizes.size() < startIndices.size())
		{
			for (const auto& child : GetChildWindows() | std::views::drop(startIndices.back()))
			{
				maxHeight = std::max(maxHeight, child->GetClientSize().cy);
			}
			rowSizes.push_back(maxHeight);
		}

		if (crossAxisAlignment == CrossAxisAlignment::Stretch)
		{
			const auto rowCount = rowSizes.size();
			const auto height = (GetClientSize().cy -
			                     (padding.crossStartPad + padding.crossEndPad +
			                      static_cast<float>(rowCount - 1) * crossAxisGap)) /
			                    static_cast<float>(rowCount);

			for (auto& size : rowSizes)
			{
				size = height;
			}
		}

		auto currentY = padding.crossStartPad;
		switch (GetCrossAxisAlignment())
		{
			case CrossAxisAlignment::Center:
			{
				const auto totalHeight =
					std::accumulate(rowSizes.begin(), rowSizes.end(), 0.0F) +
					static_cast<float>(rowSizes.size() - 1) * crossAxisGap;
				const auto availableHeight = clientSize.cy - padding.crossStartPad - padding.crossEndPad;
				currentY += (availableHeight - totalHeight) / 2;
				break;
			}
			case CrossAxisAlignment::End:
			{
				const auto totalHeight =
					std::accumulate(rowSizes.begin(), rowSizes.end(), 0.0F) +
					static_cast<float>(rowSizes.size() - 1) * crossAxisGap;
				const auto availableHeight = clientSize.cy - padding.crossStartPad - padding.crossEndPad;
				currentY += availableHeight - totalHeight;
				break;
			}
			default:
			{
				break;
			}
		}

		const auto rowCount = rowSizes.size();
		for (const auto& [index, rowSize] : rowSizes | std::views::enumerate)
		{
			const auto startIndex = startIndices[index];
			const auto endIndex = static_cast<std::size_t>(index + 1) < startIndices.size()
				? startIndices[index + 1] - 1
				: GetChildWindows().size() - 1;
			RearrangeHorizontalRow(startIndex, endIndex, currentY, rowCount);
			currentY += rowSize + crossAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalWrap() const noexcept -> void
	{
		const auto clientSize = GetClientSize();
		const auto totalChildrenSize = GetTotalChildrenSize();
		const auto availableHeight = clientSize.cy - padding.startPad - padding.endingPad;

		if (totalChildrenSize.cy + (GetChildWindows().size() - 1) * mainAxisGap <= availableHeight)
		{
			RearrangeVerticalNoWrap();
			return;
		}

		std::vector<float> columnSizes;
		columnSizes.reserve(GetChildWindows().size());

		std::vector<std::size_t> startIndices;
		startIndices.reserve(GetChildWindows().size());
		startIndices.push_back(0);

		float currentColumnSize = 0;
		float maxWidth = 0;

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
			maxWidth = std::max<float>(maxWidth, childSize.cx);
			currentColumnSize += childSize.cy + mainAxisGap;
		}

		while (columnSizes.size() < startIndices.size())
		{
			for (const auto& child : GetChildWindows() | std::views::drop(startIndices.back()))
			{
				maxWidth = std::max<float>(maxWidth, child->GetClientSize().cx);
			}
			columnSizes.push_back(maxWidth);
		}

		if (crossAxisAlignment == CrossAxisAlignment::Stretch)
		{
			const auto columnCount = columnSizes.size();
			const auto width = (GetClientSize().cx -
			                    (padding.crossStartPad + padding.crossEndPad +
			                     static_cast<float>(columnCount - 1) * crossAxisGap)) /
			                   static_cast<float>(columnCount);

			for (auto& size : columnSizes)
			{
				size = width;
			}
		}

		auto currentX = padding.crossStartPad;

		switch (GetCrossAxisAlignment())
		{
			case CrossAxisAlignment::Center:
			{
				const auto totalWidth =
					std::accumulate(columnSizes.begin(), columnSizes.end(), 0.0F) +
					static_cast<float>(columnSizes.size() - 1) * crossAxisGap;
				const auto availableWidth = clientSize.cx -
				                            padding.crossStartPad - padding.crossEndPad;
				currentX += (availableWidth - totalWidth) / 2;
				break;
			}
			case CrossAxisAlignment::End:
			{
				const auto totalWidth =
					std::accumulate(columnSizes.begin(), columnSizes.end(), 0.0F) +
					static_cast<float>(columnSizes.size() - 1) * crossAxisGap;
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

		const auto columnCount = columnSizes.size();
		for (const auto& [index, columnSize] : columnSizes | std::views::enumerate)
		{
			const auto startIndex = startIndices[index];
			const auto endIndex = static_cast<std::size_t>(index + 1) < startIndices.size()
				? startIndices[index + 1] - 1
				: GetChildWindows().size() - 1;
			RearrangeVerticalColumn(startIndex, endIndex, currentX, columnCount);
			currentX += columnSize + crossAxisGap;
		}
	}

	auto StackLayout::RearrangeHorizontalRow(
		const std::size_t startChildIndex,
		const std::size_t endChildIndex,
		float yPosition, const std::size_t rowCount) const noexcept -> void
	{
		auto currentX = padding.startPad;
		const auto height = (GetClientSize().cy -
		                     (padding.crossStartPad + padding.crossEndPad +
		                      static_cast<float>(rowCount - 1) * crossAxisGap)) /
		                    static_cast<float>(rowCount);

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalWidth =
					std::accumulate(
						GetChildWindows().begin() + startChildIndex,
						GetChildWindows().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& child)
						{
							return sum + child->GetClientSize().cx;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
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
						GetChildWindows().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& child)
						{
							return sum + child->GetClientSize().cx;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
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
			if (GetCrossAxisAlignment() == CrossAxisAlignment::Stretch)
			{
				child->Resize({ child->GetClientSize().cx, height });
			}
			currentX += child->GetClientSize().cx + mainAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalColumn(
		const std::size_t startChildIndex,
		const std::size_t endChildIndex,
		float xPosition, const std::size_t columnCount) const noexcept -> void
	{
		auto currentY = padding.startPad;
		const auto width = (GetClientSize().cx -
		                    (padding.crossStartPad + padding.crossEndPad +
		                     static_cast<float>(columnCount - 1) * crossAxisGap)) /
		                   static_cast<float>(columnCount);

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalHeight =
					std::accumulate(
						GetChildWindows().begin() + startChildIndex,
						GetChildWindows().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& child)
						{
							return sum + child->GetClientSize().cy;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
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
						GetChildWindows().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& child)
						{
							return sum + child->GetClientSize().cy;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
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
			if (GetCrossAxisAlignment() == CrossAxisAlignment::Stretch)
			{
				child->Resize({ width, child->GetClientSize().cy });
			}
			currentY += child->GetClientSize().cy + mainAxisGap;
		}
	}
}

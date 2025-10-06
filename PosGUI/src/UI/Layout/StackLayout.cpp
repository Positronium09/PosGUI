module PGUI.UI.Layout.StackLayout;

import std;

import PGUI.WindowClass;
import PGUI.ErrorHandling;

namespace PGUI::UI::Layout
{
	StackLayout::StackLayout(
		const RectF bounds,
		const LayoutOrientation orientation,
		const MainAxisAlignment mainAxisAlignment, const CrossAxisAlignment crossAxisAlignment,
		const WrapMode wrapMode, const StackLayoutPadding padding,
		const float gap, const float crossGap) noexcept :
		LayoutPanel{ bounds },
		orientation{ orientation }, mainAxisAlignment{ mainAxisAlignment },
		crossAxisAlignment{ crossAxisAlignment }, padding{ padding },
		wrapMode{ wrapMode },
		mainAxisGap{ gap }, crossAxisGap{ crossGap }
	{ }

	auto StackLayout::RearrangeItems() noexcept -> void
	{
		if (GetItemCount() == 0)
		{
			return;
		}

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
			RearrangeItems();
		}
	}

	auto StackLayout::SetCrossAxisGap(const float _crossGap) noexcept -> void
	{
		if (crossAxisGap != _crossGap)
		{
			crossAxisGap = _crossGap;
			if (wrapMode == WrapMode::Wrap)
			{
				RearrangeItems();
			}
		}
	}

	auto StackLayout::SetPadding(const StackLayoutPadding _padding) noexcept -> void
	{
		if (padding != _padding)
		{
			padding = _padding;
			RearrangeItems();
		}
	}

	auto StackLayout::SetWrapMode(const WrapMode mode) noexcept -> void
	{
		if (wrapMode != mode)
		{
			wrapMode = mode;
			RearrangeItems();
		}
	}

	auto StackLayout::SetOrientation(const LayoutOrientation _orientation) noexcept -> void
	{
		if (orientation != _orientation)
		{
			orientation = _orientation;
			RearrangeItems();
		}
	}

	auto StackLayout::SetMainAxisAlignment(const MainAxisAlignment alignment) noexcept -> void
	{
		if (mainAxisAlignment != alignment)
		{
			mainAxisAlignment = alignment;
			RearrangeItems();
		}
	}

	auto StackLayout::SetCrossAxisAlignment(const CrossAxisAlignment alignment) noexcept -> void
	{
		if (crossAxisAlignment != alignment)
		{
			crossAxisAlignment = alignment;
			RearrangeItems();
		}
	}

	auto StackLayout::RearrangeHorizontalNoWrap() noexcept -> void
	{
		const auto size = GetSize();
		const auto totalItemSize = GetTotalItemSize().cx;
		const auto requiredSpace =
			totalItemSize +
			static_cast<float>(GetItemCount() - 1) * mainAxisGap;

		auto position = padding.startPad;

		if (mainAxisAlignment == MainAxisAlignment::End)
		{
			position = size.cx - requiredSpace - padding.endingPad;
		}
		else if (mainAxisAlignment == MainAxisAlignment::Center)
		{
			position = (size.cx + padding.startPad - padding.endingPad - requiredSpace) / 2;
		}

		for (auto& item : GetItems())
		{
			const auto itemSize = MeasureItem(item);
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
					top = padding.crossStartPad + (size.cy - itemSize.cy) / 2;
					break;
				}
				case CrossAxisAlignment::End:
				{
					top = size.cy - padding.crossEndPad - itemSize.cy;
					break;
				}
				case CrossAxisAlignment::Stretch:
				{
					ResizeItem(item, { size.cy, itemSize.cx });
					break;
				}
			}

			MoveItem(item, { position, top });
			position += itemSize.cx + mainAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalNoWrap() noexcept -> void
	{
		const auto size = GetSize();
		const auto totalItemSize = GetTotalItemSize().cy;
		const auto requiredSpace =
			totalItemSize +
			static_cast<float>(GetItemCount() - 1) * mainAxisGap;

		auto position = padding.startPad;

		if (mainAxisAlignment == MainAxisAlignment::End)
		{
			position = size.cy - requiredSpace - padding.endingPad;
		}
		else if (mainAxisAlignment == MainAxisAlignment::Center)
		{
			position = (size.cy + padding.startPad - padding.endingPad - requiredSpace) / 2;
		}

		for (auto& item : GetItems())
		{
			const auto itemSize = MeasureItem(item);
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
					left = (size.cx - itemSize.cx) / 2;
					break;
				}
				case CrossAxisAlignment::End:
				{
					left = size.cx - padding.crossEndPad - itemSize.cx;
					break;
				}
				case CrossAxisAlignment::Stretch:
				{
					ResizeItem(item, { size.cx, itemSize.cy });
					break;
				}
			}

			MoveItem(item, { left, position });
			position += itemSize.cy + mainAxisGap;
		}
	}

	auto StackLayout::RearrangeHorizontalWrap() noexcept -> void
	{
		const auto clientSize = GetSize();
		const auto totalItemSize = GetTotalItemSize();
		const auto availableWidth = clientSize.cx - padding.startPad - padding.endingPad;

		if (totalItemSize.cx + (GetItemCount() - 1) * mainAxisGap <= availableWidth)
		{
			RearrangeHorizontalNoWrap();
			return;
		}

		std::vector<float> rowSizes;
		rowSizes.reserve(GetItemCount());
		std::vector<std::size_t> startIndices;
		startIndices.reserve(GetItemCount());
		startIndices.push_back(0);

		float currentRowSize = 0;
		float maxHeight = 0;

		for (const auto& [index, item] : GetItems() | std::views::enumerate)
		{
			const auto itemSize = MeasureItem(item);
			if (currentRowSize + itemSize.cx > availableWidth)
			{
				rowSizes.push_back(maxHeight);
				startIndices.push_back(index);
				currentRowSize = 0;
				maxHeight = 0;
			}
			maxHeight = std::max(maxHeight, itemSize.cy);
			currentRowSize += itemSize.cx + mainAxisGap;
		}
		while (rowSizes.size() < startIndices.size())
		{
			for (const auto& item : GetItems() | std::views::drop(startIndices.back()))
			{
				maxHeight = std::max(maxHeight, MeasureItem(item).cy);
			}
			rowSizes.push_back(maxHeight);
		}

		if (crossAxisAlignment == CrossAxisAlignment::Stretch)
		{
			const auto rowCount = rowSizes.size();
			const auto height = (GetSize().cy -
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
				: GetItemCount() - 1;
			RearrangeHorizontalRow(startIndex, endIndex, currentY, rowCount);
			currentY += rowSize + crossAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalWrap() noexcept -> void
	{
		const auto clientSize = GetSize();
		const auto totalItemSize = GetTotalItemSize();
		const auto availableHeight = clientSize.cy - padding.startPad - padding.endingPad;

		if (totalItemSize.cy + (GetItemCount() - 1) * mainAxisGap <= availableHeight)
		{
			RearrangeVerticalNoWrap();
			return;
		}

		std::vector<float> columnSizes;
		columnSizes.reserve(GetItemCount());

		std::vector<std::size_t> startIndices;
		startIndices.reserve(GetItemCount());
		startIndices.push_back(0);

		float currentColumnSize = 0;
		float maxWidth = 0;

		for (const auto& [index, item] : GetItems() | std::views::enumerate)
		{
			const auto itemSize = MeasureItem(item);
			if (currentColumnSize + itemSize.cy > availableHeight)
			{
				columnSizes.push_back(maxWidth);
				startIndices.push_back(index);
				currentColumnSize = 0;
				maxWidth = 0;
			}
			maxWidth = std::max<float>(maxWidth, itemSize.cx);
			currentColumnSize += itemSize.cy + mainAxisGap;
		}

		while (columnSizes.size() < startIndices.size())
		{
			for (const auto& item : GetItems() | std::views::drop(startIndices.back()))
			{
				maxWidth = std::max<float>(maxWidth, MeasureItem(item).cx);
			}
			columnSizes.push_back(maxWidth);
		}

		if (crossAxisAlignment == CrossAxisAlignment::Stretch)
		{
			const auto columnCount = columnSizes.size();
			const auto width = (GetSize().cx -
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
				: GetItemCount() - 1;
			RearrangeVerticalColumn(startIndex, endIndex, currentX, columnCount);
			currentX += columnSize + crossAxisGap;
		}
	}

	auto StackLayout::RearrangeHorizontalRow(
		const std::size_t startChildIndex,
		const std::size_t endChildIndex,
		float yPosition, const std::size_t rowCount) noexcept -> void
	{
		auto currentX = padding.startPad;
		const auto height = (GetSize().cy -
		                     (padding.crossStartPad + padding.crossEndPad +
		                      static_cast<float>(rowCount - 1) * crossAxisGap)) /
		                    static_cast<float>(rowCount);

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalWidth =
					std::accumulate(
						GetItems().begin() + startChildIndex,
						GetItems().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& item)
						{
							return sum + MeasureItem(item).cx;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
				const auto availableWidth = GetSize().cx -
				                            padding.startPad - padding.endingPad;
				currentX += (availableWidth - totalWidth) / 2;
				break;
			}
			case MainAxisAlignment::End:
			{
				const auto totalWidth =
					std::accumulate(
						GetItems().begin() + startChildIndex,
						GetItems().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& item)
						{
							return sum + MeasureItem(item).cx;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
				const auto availableWidth = GetSize().cx -
				                            padding.startPad - padding.endingPad;
				currentX += availableWidth - totalWidth;
				break;
			}
			default:
			{
				break;
			}
		}

		for (auto& item : GetItems() | std::views::drop(startChildIndex) |
		                  std::views::take(endChildIndex - startChildIndex + 1))
		{
			MoveItem(item, { currentX, yPosition });
			if (GetCrossAxisAlignment() == CrossAxisAlignment::Stretch)
			{
				ResizeItem(item, { MeasureItem(item).cx, height });
			}
			currentX += MeasureItem(item).cx + mainAxisGap;
		}
	}

	auto StackLayout::RearrangeVerticalColumn(
		const std::size_t startChildIndex,
		const std::size_t endChildIndex,
		float xPosition, const std::size_t columnCount) noexcept -> void
	{
		auto currentY = padding.startPad;
		const auto width = (GetSize().cx -
		                    (padding.crossStartPad + padding.crossEndPad +
		                     static_cast<float>(columnCount - 1) * crossAxisGap)) /
		                   static_cast<float>(columnCount);

		switch (GetMainAxisAlignment())
		{
			case MainAxisAlignment::Center:
			{
				const auto totalHeight =
					std::accumulate(
						GetItems().begin() + startChildIndex,
						GetItems().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& item)
						{
							return sum + MeasureItem(item).cy;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
				const auto availableHeight = GetSize().cy -
				                             padding.startPad - padding.endingPad;
				currentY += (availableHeight - totalHeight) / 2;
				break;
			}
			case MainAxisAlignment::End:
			{
				const auto totalHeight =
					std::accumulate(
						GetItems().begin() + startChildIndex,
						GetItems().begin() + endChildIndex + 1, 0.0F,
						[](float sum, const auto& item)
						{
							return sum + MeasureItem(item).cy;
						}) + static_cast<float>(endChildIndex - startChildIndex) * mainAxisGap;
				const auto availableHeight = GetSize().cy -
				                             padding.startPad - padding.endingPad;
				currentY += availableHeight - totalHeight;
				break;
			}
			default:
			{
				break;
			}
		}

		for (auto& item : GetItems() | std::views::drop(startChildIndex) |
		                  std::views::take(endChildIndex - startChildIndex + 1))
		{
			MoveItem(item, { xPosition, currentY });

			if (GetCrossAxisAlignment() == CrossAxisAlignment::Stretch)
			{
				ResizeItem(item, { width, MeasureItem(item).cy });
			}
			currentY += MeasureItem(item).cy + mainAxisGap;
		}
	}
}

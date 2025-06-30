module PGUI.UI.Layout.StackLayout;

import PGUI.Utils.WindowsUtils;
import PGUI.Window;
import PGUI.Logging;

#include <algorithm>
#include <limits>
#include <vector>

namespace PGUI::UI::Layout
{
	void StackLayout::SetChildConstraints(HWND childHwnd, const LayoutConstraints& constraints)
	{
		childConstraints[childHwnd] = constraints;
	}

	auto StackLayout::GetChildConstraints(HWND childHwnd) const -> LayoutConstraints
	{
		if (auto it = childConstraints.find(childHwnd); it != childConstraints.end())
		{
			return it->second;
		}
		return LayoutConstraints{};
	}

	void StackLayout::RemoveChildConstraints(HWND childHwnd)
	{
		childConstraints.erase(childHwnd);
	}

	auto StackLayout::Measure(
		const std::vector<HWND>& children,
		SizeF availableSize,
		float dpi
	) const -> std::unordered_map<HWND, MeasureResult>
	{
		std::unordered_map<HWND, MeasureResult> results;
		
		// Calculate available space after padding
		const auto paddingWidth = padding.left + padding.right;
		const auto paddingHeight = padding.top + padding.bottom;
		const auto availableContentSize = SizeF{
			std::max(0.0f, availableSize.cx - paddingWidth),
			std::max(0.0f, availableSize.cy - paddingHeight)
		};

		for (HWND child : children)
		{
			if (!child || !IsWindow(child))
			{
				continue;
			}

			const auto constraints = GetChildConstraints(child);
			const auto naturalSize = GetChildNaturalSize(child, dpi);
			
			// Calculate desired size including margin
			const auto marginWidth = constraints.margin.left + constraints.margin.right;
			const auto marginHeight = constraints.margin.top + constraints.margin.bottom;
			const auto desiredContentSize = SizeF{
				naturalSize.cx + marginWidth,
				naturalSize.cy + marginHeight
			};

			// Apply constraints to get min/max sizes
			const auto minSize = ApplyConstraints(naturalSize, constraints);
			const auto maxSize = constraints.maxSize.value_or(SizeF{ 
				std::numeric_limits<float>::max(), 
				std::numeric_limits<float>::max() 
			});

			results[child] = MeasureResult{
				desiredContentSize,
				SizeF{ minSize.cx + marginWidth, minSize.cy + marginHeight },
				SizeF{ maxSize.cx + marginWidth, maxSize.cy + marginHeight }
			};
		}

		return results;
	}

	void StackLayout::Arrange(
		const std::vector<HWND>& children,
		RectF finalRect,
		float dpi
	) const
	{
		if (children.empty())
		{
			return;
		}

		// Calculate content area after padding
		const auto contentRect = RectF{
			finalRect.left + padding.left,
			finalRect.top + padding.top,
			finalRect.right - padding.right,
			finalRect.bottom - padding.bottom
		};

		const auto availableSize = contentRect.Size();
		const auto measureResults = Measure(children, availableSize, dpi);

		// Calculate total space needed in the stacking direction
		float totalStackingSize = 0.0f;
		float totalSpacing = 0.0f;
		
		if (children.size() > 1)
		{
			totalSpacing = spacing * (children.size() - 1);
		}

		for (HWND child : children)
		{
			if (auto it = measureResults.find(child); it != measureResults.end())
			{
				const auto& result = it->second;
				if (orientation == Orientation::Horizontal)
				{
					totalStackingSize += result.desiredSize.cx;
				}
				else
				{
					totalStackingSize += result.desiredSize.cy;
				}
			}
		}

		const auto totalNeededSize = totalStackingSize + totalSpacing;
		
		// Determine if we need to compress or if we have extra space
		const auto availableStackingSize = (orientation == Orientation::Horizontal) 
			? availableSize.cx 
			: availableSize.cy;
		
		const auto compressionRatio = (totalNeededSize > availableStackingSize && totalNeededSize > 0) 
			? availableStackingSize / totalNeededSize 
			: 1.0f;

		// Position children
		float currentPos = (orientation == Orientation::Horizontal) 
			? contentRect.left 
			: contentRect.top;

		for (HWND child : children)
		{
			if (!child || !IsWindow(child))
			{
				continue;
			}

			auto it = measureResults.find(child);
			if (it == measureResults.end())
			{
				continue;
			}

			const auto& result = it->second;
			const auto constraints = GetChildConstraints(child);

			// Calculate size in stacking direction
			auto stackingSize = (orientation == Orientation::Horizontal) 
				? result.desiredSize.cx 
				: result.desiredSize.cy;
			
			if (compressionRatio < 1.0f)
			{
				stackingSize *= compressionRatio;
			}

			// Calculate size in cross direction
			const auto crossSize = (orientation == Orientation::Horizontal) 
				? availableSize.cy 
				: availableSize.cx;

			auto childAlignment = constraints.alignmentOverride.value_or(alignment);
			
			// Handle stretch alignment
			SizeF childSize;
			if (orientation == Orientation::Horizontal)
			{
				childSize.cx = stackingSize - constraints.margin.left - constraints.margin.right;
				
				if (childAlignment == Alignment::Stretch)
				{
					childSize.cy = crossSize - constraints.margin.top - constraints.margin.bottom;
				}
				else
				{
					childSize.cy = result.desiredSize.cy - constraints.margin.top - constraints.margin.bottom;
				}
			}
			else
			{
				childSize.cy = stackingSize - constraints.margin.top - constraints.margin.bottom;
				
				if (childAlignment == Alignment::Stretch)
				{
					childSize.cx = crossSize - constraints.margin.left - constraints.margin.right;
				}
				else
				{
					childSize.cx = result.desiredSize.cx - constraints.margin.left - constraints.margin.right;
				}
			}

			// Apply constraints to final size
			childSize = ApplyConstraints(childSize, constraints);

			// Calculate position
			PointF childPos;
			if (orientation == Orientation::Horizontal)
			{
				childPos.x = currentPos + constraints.margin.left;
				
				// Calculate cross-axis position based on alignment
				const auto availableCrossSize = crossSize - constraints.margin.top - constraints.margin.bottom;
				const auto crossPos = CalculateAlignedPosition(childSize.cy, availableCrossSize, childAlignment);
				childPos.y = contentRect.top + constraints.margin.top + crossPos;
			}
			else
			{
				childPos.y = currentPos + constraints.margin.top;
				
				// Calculate cross-axis position based on alignment  
				const auto availableCrossSize = crossSize - constraints.margin.left - constraints.margin.right;
				const auto crossPos = CalculateAlignedPosition(childSize.cx, availableCrossSize, childAlignment);
				childPos.x = contentRect.left + constraints.margin.left + crossPos;
			}

			// Position and size the child window
			const auto childRect = RectF{ childPos, childSize };
			SetWindowPos(child, nullptr,
				static_cast<int>(childRect.left),
				static_cast<int>(childRect.top),
				static_cast<int>(childRect.Width()),
				static_cast<int>(childRect.Height()),
				SWP_NOZORDER | SWP_NOACTIVATE);

			// Move to next position
			currentPos += stackingSize + spacing;
		}
	}

	void StackLayout::Layout(
		const std::vector<HWND>& children,
		RectF finalRect,
		float dpi
	) const
	{
		Arrange(children, finalRect, dpi);
	}

	auto StackLayout::GetChildNaturalSize(HWND childHwnd, float dpi) const -> SizeF
	{
		if (!childHwnd || !IsWindow(childHwnd))
		{
			return SizeF{};
		}

		RECT rect;
		if (GetWindowRect(childHwnd, &rect))
		{
			return SizeF{
				static_cast<float>(rect.right - rect.left),
				static_cast<float>(rect.bottom - rect.top)
			};
		}

		// Fallback size
		return SizeF{ 100.0f, 30.0f };
	}

	constexpr auto StackLayout::ApplyConstraints(
		SizeF size,
		const LayoutConstraints& constraints
	) const noexcept -> SizeF
	{
		auto result = size;

		// Apply minimum size constraints
		if (constraints.minSize.has_value())
		{
			const auto& minSize = constraints.minSize.value();
			result.cx = std::max(result.cx, minSize.cx);
			result.cy = std::max(result.cy, minSize.cy);
		}

		// Apply maximum size constraints
		if (constraints.maxSize.has_value())
		{
			const auto& maxSize = constraints.maxSize.value();
			result.cx = std::min(result.cx, maxSize.cx);
			result.cy = std::min(result.cy, maxSize.cy);
		}

		return result;
	}

	constexpr auto StackLayout::CalculateAlignedPosition(
		float childSize,
		float availableSize,
		Alignment alignment
	) const noexcept -> float
	{
		switch (alignment)
		{
		case Alignment::Start:
			return 0.0f;
		case Alignment::Center:
			return (availableSize - childSize) / 2.0f;
		case Alignment::End:
			return availableSize - childSize;
		case Alignment::Stretch:
			return 0.0f; // Stretch is handled by sizing, not positioning
		default:
			return 0.0f;
		}
	}
}
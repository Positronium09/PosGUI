module PGUI.UI.Layout.StackLayout;

import PGUI.WindowClass;

namespace PGUI::UI::Layout
{
	StackLayout::StackLayout(
		const LayoutOrientation orientation,
		const MainAxisAlignment mainAxisAlignment,
		const CrossAxisAlignment crossAxisAlignment,
		const StackLayoutPadding padding,
		const long gap) noexcept :
		StackLayout{
			L"PGUI_StackLayout",
			orientation,
			mainAxisAlignment,
			crossAxisAlignment,
			padding,
			gap
		}
	{ }

	auto StackLayout::RearrangeChildren() noexcept -> void
	{
		if (orientation == LayoutOrientation::Horizontal)
		{
			RearrangeHorizontal();
			return;
		}
		RearrangeVertical();
	}

	auto StackLayout::SetGap(const long _gap) noexcept -> void
	{
		gap = _gap;
		RearrangeChildren();
	}

	auto StackLayout::SetPadding(const StackLayoutPadding _padding) noexcept -> void
	{
		padding = _padding;
		RearrangeChildren();
	}

	auto StackLayout::SetOrientation(const LayoutOrientation _orientation) noexcept -> void
	{
		orientation = _orientation;
		RearrangeChildren();
	}

	auto StackLayout::SetMainAxisAlignment(const MainAxisAlignment alignment) noexcept -> void
	{
		mainAxisAlignment = alignment;
		RearrangeChildren();
	}

	auto StackLayout::SetCrossAxisAlignment(const CrossAxisAlignment alignment) noexcept -> void
	{
		crossAxisAlignment = alignment;
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

	StackLayout::StackLayout(
		const std::wstring_view className,
		const LayoutOrientation orientation,
		const MainAxisAlignment mainAxisAlignment, const CrossAxisAlignment crossAxisAlignment,
		const StackLayoutPadding padding,
		const long gap) noexcept :
		LayoutPanel{ WindowClass::Create(className) },
		orientation{ orientation }, mainAxisAlignment{ mainAxisAlignment },
		crossAxisAlignment{ crossAxisAlignment }, padding{ padding }, gap{ gap }
	{ }

	auto StackLayout::RearrangeHorizontal() const noexcept -> void
	{
		const auto size = GetClientSize();
		const auto childrenSize = GetTotalChildrenSize().cx;
		const auto requiredSpace =
			childrenSize +
			static_cast<long>(GetChildWindows().size() - 1) * gap;

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
			position += childSize.cx + gap;
		}
	}

	auto StackLayout::RearrangeVertical() const noexcept -> void
	{
		const auto size = GetClientSize();
		const auto childrenSize = GetTotalChildrenSize().cy;
		const auto requiredSpace =
			childrenSize +
			static_cast<long>(GetChildWindows().size() - 1) * gap;

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
			position += childSize.cy + gap;
		}
	}
}

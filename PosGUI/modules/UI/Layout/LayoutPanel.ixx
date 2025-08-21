module;
#include <Windows.h>

export module PGUI.UI.Layout.LayoutPanel;

import std;

import PGUI.WindowClass;
import PGUI.UI.UIComponent;

export namespace PGUI::UI::Layout
{
	class LayoutPanel : public UIComponent
	{
		public:
		LayoutPanel() noexcept :
			LayoutPanel{ WindowClass::Create(L"PGUI_LayoutPanel") }
		{ }

		explicit LayoutPanel(const WindowClassPtr& wndClass) noexcept :
			UIComponent{ wndClass }
		{ }

		virtual auto RearrangeChildren() noexcept -> void = 0;

		auto GetTotalChildrenSize() const noexcept
		{
			SizeL totalSize;
			for (const auto& child : GetChildWindows())
			{
				const auto size = child->GetClientSize();
				totalSize += size;
			}

			return totalSize;
		}

		auto GetChildrenSizeUpToIndex(const std::size_t index) const noexcept
		{
			if (index >= GetChildWindows().size())
			{
				return SizeL{ 0, 0 };
			}

			SizeL totalSize;
			for (const auto& child : GetChildWindows() | std::views::take(index + 1))
			{
				const auto size = child->GetClientSize();
				totalSize += size;
			}
			return totalSize;
		}

		auto GetChildrenSizeBetweenIndices(
			const std::size_t startIndex, const std::size_t endIndex) const noexcept
		{
			const auto end = std::clamp(endIndex, 0ULL, GetChildWindows().size() - 1);

			if (startIndex >= GetChildWindows().size() || startIndex > end)
			{
				return SizeL{ 0, 0 };
			}

			SizeL totalSize;

			for (const auto& child : GetChildWindows() |
				std::views::drop(startIndex) | std::views::take(end - startIndex + 1))
			{
				const auto size = child->GetClientSize();
				totalSize += size;
			}
			return totalSize;
		}

		protected:
		auto OnSizeChanged(const SizeL newSize) -> void override
		{
			UIComponent::OnSizeChanged(newSize);
			RearrangeChildren();
		}

		private:
		auto OnChildAdded(const WindowPtr<Window>&) -> void override
		{
			RearrangeChildren();
		}
		auto OnChildRemoved(HWND) -> void override
		{
			RearrangeChildren();
		}
	};
}

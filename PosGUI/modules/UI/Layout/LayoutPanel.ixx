﻿export module PGUI.UI.Layout.LayoutPanel;

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
	};
}

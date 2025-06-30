module;
#include <Windows.h>
#include <vector>

export module PGUI.UI.Layout.StackPanel;

import PGUI.UI.UIComponent;
import PGUI.UI.Layout.StackLayout;
import PGUI.UI.Layout.LayoutEnums;
import PGUI.Shape2D;

export namespace PGUI::UI::Layout
{
	/// <summary>
	/// A panel that arranges child elements in a single row or column
	/// </summary>
	class StackPanel : public UIComponent
	{
	public:
		StackPanel();
		explicit StackPanel(const WindowClassPtr& wndClass);
		explicit StackPanel(Orientation orientation);
		StackPanel(const WindowClassPtr& wndClass, Orientation orientation);

		/// <summary>
		/// Gets the orientation of the stack panel
		/// </summary>
		[[nodiscard]] constexpr auto GetOrientation() const noexcept -> Orientation
		{
			return stackLayout.GetOrientation();
		}

		/// <summary>
		/// Sets the orientation of the stack panel
		/// </summary>
		void SetOrientation(Orientation orientation) noexcept
		{
			stackLayout.SetOrientation(orientation);
			InvalidateLayout();
		}

		/// <summary>
		/// Gets the default alignment for children
		/// </summary>
		[[nodiscard]] constexpr auto GetAlignment() const noexcept -> Alignment
		{
			return stackLayout.GetAlignment();
		}

		/// <summary>
		/// Sets the default alignment for children
		/// </summary>
		void SetAlignment(Alignment alignment) noexcept
		{
			stackLayout.SetAlignment(alignment);
			InvalidateLayout();
		}

		/// <summary>
		/// Gets the spacing between children
		/// </summary>
		[[nodiscard]] constexpr auto GetSpacing() const noexcept -> float
		{
			return stackLayout.GetSpacing();
		}

		/// <summary>
		/// Sets the spacing between children
		/// </summary>
		void SetSpacing(float spacing) noexcept
		{
			stackLayout.SetSpacing(spacing);
			InvalidateLayout();
		}

		/// <summary>
		/// Gets the padding of the panel
		/// </summary>
		[[nodiscard]] constexpr auto GetPadding() const noexcept -> PGUI::RectF
		{
			return stackLayout.GetPadding();
		}

		/// <summary>
		/// Sets the padding of the panel
		/// </summary>
		void SetPadding(PGUI::RectF padding) noexcept
		{
			stackLayout.SetPadding(padding);
			InvalidateLayout();
		}

		/// <summary>
		/// Sets layout constraints for a specific child window
		/// </summary>
		void SetChildConstraints(HWND childHwnd, const LayoutConstraints& constraints)
		{
			stackLayout.SetChildConstraints(childHwnd, constraints);
			InvalidateLayout();
		}

		/// <summary>
		/// Gets layout constraints for a specific child window
		/// </summary>
		[[nodiscard]] auto GetChildConstraints(HWND childHwnd) const -> LayoutConstraints
		{
			return stackLayout.GetChildConstraints(childHwnd);
		}

		/// <summary>
		/// Removes layout constraints for a specific child window
		/// </summary>
		void RemoveChildConstraints(HWND childHwnd)
		{
			stackLayout.RemoveChildConstraints(childHwnd);
			InvalidateLayout();
		}

		/// <summary>
		/// Forces layout update
		/// </summary>
		void UpdateLayout()
		{
			PerformLayout();
		}

	protected:
		/// <summary>
		/// Called when a child window is added
		/// </summary>
		void OnChildAdded(const WindowPtr<Window>& child) override
		{
			InvalidateLayout();
		}

		/// <summary>
		/// Called when a child window is removed
		/// </summary>
		void OnChildRemoved(HWND childHwnd) override
		{
			stackLayout.RemoveChildConstraints(childHwnd);
			InvalidateLayout();
		}

		/// <summary>
		/// Handles window size changes
		/// </summary>
		auto OnSize(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult override
		{
			// Call base class implementation first
			auto result = UIComponent::OnSize(msg, wParam, lParam);
			
			// Perform layout after size change
			PerformLayout();
			
			return result;
		}

	private:
		/// <summary>
		/// Invalidates the current layout and schedules an update
		/// </summary>
		void InvalidateLayout() noexcept
		{
			layoutInvalid = true;
			// Schedule layout update for next message loop iteration
			PostMsg(WM_USER_LAYOUT_UPDATE, 0, 0);
		}

		/// <summary>
		/// Performs the actual layout operation
		/// </summary>
		void PerformLayout()
		{
			if (!layoutInvalid)
			{
				return;
			}

			layoutInvalid = false;

			// Get child windows as HWNDs
			std::vector<HWND> childHwnds;
			const auto& children = GetChildWindows();
			childHwnds.reserve(children.size());
			
			for (const auto& child : children)
			{
				if (child && child->Hwnd())
				{
					childHwnds.push_back(child->Hwnd());
				}
			}

			// Get client area for layout
			const auto clientRect = GetClientRect();
			const auto layoutRect = PGUI::RectF{
				static_cast<float>(clientRect.left),
				static_cast<float>(clientRect.top),
				static_cast<float>(clientRect.right),
				static_cast<float>(clientRect.bottom)
			};

			// Perform layout with current DPI
			const auto dpi = static_cast<float>(GetDPI());
			stackLayout.Layout(childHwnds, layoutRect, dpi);
		}

		/// <summary>
		/// Handles custom layout update message
		/// </summary>
		auto OnLayoutUpdate(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult
		{
			PerformLayout();
			return 0;
		}

		static constexpr UINT WM_USER_LAYOUT_UPDATE = WM_USER + 1;

		StackLayout stackLayout;
		bool layoutInvalid = true;
	};
}
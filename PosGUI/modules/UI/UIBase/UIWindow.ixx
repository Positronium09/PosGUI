module;
#include <Windows.h>

export module PGUI.UI.UIBase:UIWindow;

import std;

import :Interface;
import :UIElement;
import :UIContainer;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	class UIWindow : public DirectXCompositionWindow
	{
		public:
		UIWindow();
		explicit UIWindow(const WindowClassPtr&);

		[[nodiscard]] auto& GetChildren() noexcept
		{
			return childrenContainer;
		}
		[[nodiscard]] const auto& GetChildren() const noexcept
		{
			return childrenContainer;
		}

		auto EnableTabStop() noexcept
		{
			tabStopEnabled = true;
		}
		auto DisableTabStop() noexcept
		{
			tabStopEnabled = false;
		}
		[[nodiscard]] auto IsTabStopEnabled() const noexcept
		{
			return tabStopEnabled;
		}

		[[nodiscard]] auto GetFocusedElement() const noexcept
		{
			return focusedElement;
		}
		[[nodiscard]] auto GetHoveredElement() const noexcept
		{
			return hoveredElement;
		}

		[[nodiscard]] auto LockFocusedElement() const noexcept
		{
			return focusedElement.lock();
		}
		[[nodiscard]] auto LockHoveredElement() const noexcept
		{
			return hoveredElement.lock();
		}

		protected:
		virtual auto Render(const Graphics&) -> void
		{
			/*  */
		}

		auto CreateDeviceResources() -> void override;
		auto DiscardDeviceResources() -> void override;

		private:
		bool tabStopEnabled = false;
		UIContainer childrenContainer;
		//If focused or hovered elements are destroyed crash due to dangling pointer
		//TODO Fix it when it happens prolly use weak_ptr but too lazy rn

		WeakUIElementPtr focusedElement;
		WeakUIElementPtr hoveredElement;

		auto ChangeFocusedElement(const RawUIElementPtr newFocused) noexcept -> void
		{
			if (!newFocused)
			{
				if (const auto focused = LockFocusedElement();
					focused != nullptr)
				{
					focused->RemoveFocus();
					focusedElement.reset();
				}
				return;
			}

			if (!newFocused->IsFocusable() || !*newFocused->IsEnabled())
			{
				return;
			}

			const auto focused = LockFocusedElement();
			if (focused.get() == newFocused)
			{
				return;
			}
			if (!focusedElement.expired())
			{
				focused->RemoveFocus();
			}
			if (newFocused)
			{
				newFocused->SetFocus();
			}

			focusedElement = newFocused->weak_from_this();
		}

		auto Draw(Graphics graphics) -> void override;
		auto OnSizeChanged(SizeL) -> void override;

		auto OnNCCreate(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;

		auto OnFocusChanged(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;

		auto OnChar(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;
		auto OnKey(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;

		// ReSharper disable CommentTypo
		// TODO Maybe implement WM_SYSKEYDOWN, WM_SYSKEYUP, WM_SYSDEADCHAR and WM_HOTKEY
		// ReSharper restore CommentTypo

		auto OnMouseMove(UINT, WPARAM, LPARAM) -> MessageHandlerResult;
		auto OnMouseLeave(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnMouseHover(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;
		auto OnMouseWheel(UINT, WPARAM, LPARAM) const noexcept -> MessageHandlerResult;
		auto OnMouseButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnMouseButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnMouseDoubleClick(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
	};
}

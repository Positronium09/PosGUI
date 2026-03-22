export module PGUI.UI.UICore:UIHost;

import :Interface;
import :UIElement;
import :UIContainer;
import PGUI.UI.DirectXCompositionWindow;
import PGUI.Shape2D;
import PGUI.Window;
import PGUI.WindowClass;
import PGUI.Event;

export namespace PGUI::UI
{
	class UIHost : public DirectXCompositionWindow
	{
		public:
		UIHost();
		explicit UIHost(const WindowClassPtr& windowClass);

		template <typename Self>
		[[nodiscard]] auto&& RedrawRequestedEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.redrawRequestedEvent);
		}

		template <typename Self>
		[[nodiscard]] auto&& RootContainer(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.rootContainer);
		}

		template <typename Self>
		[[nodiscard]] auto&& HoveredElement(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.hoveredElement);
		}

		template <typename Self>
		[[nodiscard]] auto&& FocusedElement(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.focusedElement);
		}

		protected:
		virtual auto Render(const Graphics&) noexcept -> void
		{
			/*  */
		}

		auto CreateDeviceResources() -> void override;
		auto DiscardDeviceResources() -> void override;

		private:
		auto OnNCCreate(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnSize(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;

		auto OnFocusChanged(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		
		auto OnChar(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		
		auto OnMouseMove(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnMouseHover(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnMouseLeave(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnMouseWheel(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnMouseButtonDown(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnMouseButtonUp(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;
		auto OnMouseDoubleClick(MessageID msg, Argument1 arg1, Argument2 arg2) noexcept -> MessageHandlerResult;

		auto RedrawRequested(RawUIElementPtr<> element) noexcept -> void;

		auto Draw(const Graphics& graphics) noexcept -> void final;

		Event<RawUIElementPtr<>> redrawRequestedEvent;
		RawUIElementPtr<> hoveredElement;
		RawUIElementPtr<> focusedElement;
		UIContainerPtr<> rootContainer;
	};
}

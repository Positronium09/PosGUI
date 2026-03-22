export module PGUI.UI.UICore:UIContainer;

import :Interface;
import :UIElement;
import PGUI.Event;
import PGUI.ErrorHandling;
import PGUI.UI.Layout;

import std;

export namespace PGUI::UI
{
	class UIContainer final : public UIElement
	{
		struct ChildAssociatedData
		{
			CallbackId zIndexCallbackId;
			CallbackId redrawRequestCallbackId;
		};

		public:
		explicit UIContainer(const RectF& rect) noexcept;

		template <UIElementType T, typename... Args>
		auto CreateChildElement(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
			-> Result<RawUIElementPtr<T>>
		{
			auto element = std::make_unique<T>(std::forward<Args>(args)...);
			auto elementPtr = element.get();
			if (elementPtr == nullptr)
			{
				return Unexpected{
					Error{
						ErrorCode::NullPointer
					}.SetCustomMessage(L"Failed to create the new element")
				};
			}

			children.push_back(UIElementPtr<>{ std::move(element) });
			ChildAddedEvent().Invoke(elementPtr);
			return elementPtr;
		}

		template <UIElementType T>
		auto AddChildElement(UIElementPtr<T>& element) -> Result<RawUIElementPtr<T>>
		{
			auto elementPtr = element.get();
			if (elementPtr == nullptr)
			{
				return Unexpected{
					Error{
						ErrorCode::NullPointer
					}.SetCustomMessage(L"Cannot add a nullptr as a child element")
				};
			}

			children.push_back(std::move(element));
			ChildAddedEvent().Invoke(elementPtr);
			return elementPtr;
		}

		template <typename Self>
		[[nodiscard]] auto&& GetChildElements(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.children);
		}

		[[nodiscard]] auto GetChildElementCount() const noexcept -> std::size_t
		{
			return children.size();
		}

		auto RemoveChildElement(RawUIElementPtr<> element) -> Result<UIElementPtr<>>;
		auto RemoveChildElement(std::size_t index) -> Result<UIElementPtr<>>;

		[[nodiscard]] auto GetElementIndex(RawUIElementPtr<> element) const noexcept -> Result<std::size_t>;
		[[nodiscard]] auto GetElementLinearIndex(RawUIElementPtr<> element) const noexcept -> Result<std::size_t>;

		[[nodiscard]] auto GetElementAtIndex(std::size_t index) const noexcept -> Result<RawUIElementPtr<>>;

		[[nodiscard]] auto GetLinearElementCount() const noexcept -> std::size_t;
		[[nodiscard]] auto GetElementAtLinearIndex(std::size_t index) const noexcept -> Result<RawUIElementPtr<>>;

		[[nodiscard]] auto GetElementAtPosition(PointF point) const noexcept -> Result<RawUIElementPtr<>>;

		auto IsChildElementVisible(RawUIElementPtr<> element) const noexcept -> bool;

		auto ClipRendering(const bool value) noexcept
		{
			clipRendering = value;
		}
		auto IsRenderingClipped() const noexcept { return clipRendering; }

		auto Render(const Graphics&) noexcept -> void override;
		auto HitTest(PointF point) noexcept -> bool override;
		auto CreateDeviceResources() noexcept -> void override;
		auto DiscardDeviceResources() noexcept -> void override;

		template <typename Self>
		[[nodiscard]] auto&& ChildAddedEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.childAdded);
		}

		template <typename Self>
		[[nodiscard]] auto&& ChildRemovedEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.childRemoved);
		}

		private:
		auto OnChildAddedEvent(RawUIElementPtr<> element) noexcept -> void;
		auto OnChildRemovedEvent(RawUIElementPtr<> element) noexcept -> void;
		auto OnChildZIndexChangedEvent(RawUIElementPtr<> element, ZIndex newZIndex) noexcept -> void;
		auto OnChildRedrawRequestedEvent(RawUIElementPtr<> element) noexcept -> void;

		auto EnsureZOrder() noexcept -> void;

		bool clipRendering = false;
		bool isZOrderDirty = false;
		Event<RawUIElementPtr<>> childAdded;
		Event<RawUIElementPtr<>> childRemoved;
		std::vector<UIElementPtr<>> children;
		std::unordered_map<RawUIElementPtr<>, ChildAssociatedData> childAssociatedData;
		std::unique_ptr<Layout::LayoutPanel> layoutPanel;
	};
}

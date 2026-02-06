export module PGUI.UI.UIBase:UIContainer;

import std;

import :Interface;
import :UIElement;
import PGUI.ErrorHandling;
import PGUI.UI.Layout.LayoutPanel;

export namespace PGUI::UI
{
	class UIContainer : public UIElement
	{
		public:
		template <UIElementType T, typename... Args>
		auto CreateAndAdd(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		{
			auto newElement = std::make_shared<T>(std::forward<Args>(args)...);

			elements.emplace_back(newElement);
			newElement->SetParent(this);

			zOrderDirty = true;

			if (layoutPanel)
			{
				layoutPanel->AddItem(*newElement);
			}

			return newElement;
		}
		auto AddElement(const UIElementPtr& ptr) noexcept -> void;

		[[nodiscard]] const auto& GetElements() const noexcept
		{
			return elements;
		}
		[[nodiscard]] auto& GetElements() noexcept
		{
			return elements;
		}
		[[nodiscard]] auto GetElementCount() const noexcept
		{
			return elements.size();
		}

		auto GetElementAtIndex(std::size_t index) const noexcept -> Result<UIElementPtr>;
		auto GetElementAtIndex(std::size_t index) noexcept -> Result<UIElementPtr>;

		auto RemoveElement(std::size_t index) noexcept -> Result<UIElementPtr>;
		auto RemoveElement(RawUIElementPtr ptr) noexcept -> Result<UIElementPtr>;

		auto GetElementIndex(RawCUIElementPtr ptr) const noexcept -> Result<std::size_t>;

		auto GetElementAtPosition(PointF point) const noexcept -> Result<RawCUIElementPtr>;
		auto GetElementAtPosition(PointF point) noexcept -> Result<RawUIElementPtr>;

		auto GetLinearElementCount() const noexcept -> std::size_t;
		auto GetElementAtLinearIndex(std::size_t index) const noexcept -> Result<RawUIElementPtr>;
		auto GetElementLinearIndex(RawCUIElementPtr ptr) const noexcept -> Result<std::size_t>;

		auto HandleEvent(UIEvent&) -> void override;

		auto EnsureZOrder() noexcept -> void;

		auto ChildRemoved(RawCUIElementPtr ptr) noexcept -> void override
		{
			std::erase_if(
				elements, 
				[&ptr](const auto& elem)
				{
					return elem.get() == ptr;
				});
		}

		[[nodiscard]] auto GetPosition() const noexcept -> PointF override
		{
			return rect.TopLeft();
		}
		[[nodiscard]] auto GetRect() const noexcept -> RectF override
		{
			return rect;
		}
		[[nodiscard]] auto GetSize() const noexcept -> SizeF override
		{
			return rect.Size();
		}

		[[nodiscard]] auto HitTest(PointF point) noexcept -> bool override;

		auto Render(const Graphics& graphics) -> void override;

		auto CreateDeviceResources() -> void override;
		auto DiscardDeviceResources() -> void override;

		auto Move(const PointF position) -> void override
		{
			PositionChangedEvent positionChangedEvent;
			positionChangedEvent.type = EventType::PositionChanged;
			positionChangedEvent.oldPosition = rect.TopLeft();
			positionChangedEvent.newPosition = position;

			const auto offset = position - rect.TopLeft();
			rect = RectF{ position, rect.Size() };
			EnsureElementsAreRelative(offset);

			HandleEvent(positionChangedEvent);
		}
		auto Resize(const SizeF size) -> void override
		{
			SizeChangedEvent sizeChangedEvent;
			sizeChangedEvent.type = EventType::SizeChanged;
			sizeChangedEvent.oldSize = rect.Size();
			sizeChangedEvent.newSize = size;

			rect = RectF{ rect.TopLeft(), size };

			HandleEvent(sizeChangedEvent);
		}
		auto MoveAndResize(const RectF newRect) -> void override
		{
			RectChangedEvent rectChangedEvent;
			rectChangedEvent.type = EventType::RectChanged;
			rectChangedEvent.oldRect = rect;
			rectChangedEvent.newRect = newRect;

			const auto offset = newRect.TopLeft() - rect.TopLeft();
			rect = newRect;
			EnsureElementsAreRelative(offset);
			HandleEvent(rectChangedEvent);
		}
		auto MoveAndResize(const PointF position, const SizeF size) -> void override
		{
			MoveAndResize(RectF{ position, size });
		}

		[[nodiscard]] auto& GetLayoutPanel() noexcept
		{
			return *layoutPanel;
		}
		[[nodiscard]] const auto& GetLayoutPanel() const noexcept
		{
			return *layoutPanel;
		}
		auto SetLayoutPanel(std::unique_ptr<Layout::LayoutPanel> newLayoutPanel) noexcept -> void
		{
			layoutPanel = std::move(newLayoutPanel);
			if (!layoutPanel)
			{
				return;
			}
			layoutPanel->MoveAndResize(GetRect());
			for (auto& element : elements)
			{
				layoutPanel->AddItem(*element);
			}
		}
		auto RemoveLayoutPanel() noexcept -> void
		{
			layoutPanel.reset();
		}

		auto SetCropRendering(const bool val) noexcept -> void
		{
			cropRendering = val;
		}

		[[nodiscard]] auto IsCropRenderingEnabled() const noexcept
		{
			return cropRendering;
		}

		private:
		bool zOrderDirty = false;
		bool cropRendering = true;
		RectF rect;
		std::vector<UIElementPtr> elements;
		std::unique_ptr<Layout::LayoutPanel> layoutPanel;

		auto EnsureElementsAreRelative(const PointF offset) const noexcept -> void
		{
			if (offset.IsZero())
			{
				return;
			}
			for (const auto& element : elements)
			{
				element->Move(element->GetPosition() + offset);
			}
		}
	};
}

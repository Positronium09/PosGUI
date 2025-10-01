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
		const auto& CreateAndAdd(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		{
			elements.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
			const auto& element = elements.back();
			element->SetParent(this);

			zOrderDirty = true;

			return element;
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

		auto GetElement(std::size_t index) const noexcept -> Result<UIElementPtr>;
		auto GetElement(std::size_t index) noexcept -> Result<UIElementPtr>;

		auto RemoveElement(std::size_t index) noexcept -> Result<UIElementPtr>;
		auto RemoveElement(RawUIElementPtr ptr) noexcept -> Result<UIElementPtr>;

		auto GetElementIndex(RawCUIElementPtr ptr) const noexcept -> Result<std::size_t>;

		auto GetElementAtPosition(PointF point) const noexcept -> Result<RawCUIElementPtr>;
		auto GetElementAtPosition(PointF point) noexcept -> Result<RawUIElementPtr>;

		auto HandleEvent(UIEvent&) -> void override;

		auto EnsureZOrder() noexcept -> void;

		auto ChildRemoved(RawCUIElementPtr ptr) noexcept -> void override
		{
			const auto [begin, end] = std::ranges::remove_if(
				elements, 
				[&ptr](const auto& elem)
				{
					return elem.get() == ptr;
				});

			elements.erase(begin, end);
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

		auto Render(Graphics graphics) -> void override;

		auto Move(const PointF position) -> void override
		{
			rect = RectF{ position, rect.Size() };
		}
		auto Resize(const SizeF size) -> void override
		{
			rect = RectF{ rect.TopLeft(), size };
		}
		auto MoveAndResize(const PointF position, const SizeF size) -> void override
		{
			rect = RectF{ position, size };
		}
		auto MoveAndResize(const RectF newRect) -> void override
		{
			rect = newRect;
		}

		private:
		bool zOrderDirty = false;
		RectF rect;
		std::vector<UIElementPtr> elements;
	};
}

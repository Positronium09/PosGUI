export module PGUI.UI.UIBase:UIElement;

import :Interface;
import :UIEvent;

import PGUI.DataBinding.Property;
import PGUI.UI.Graphics;
import PGUI.Event;
import PGUI.Shape2D;

export namespace PGUI::UI
{
	class UIElement : public std::enable_shared_from_this<UIElement>
	{
		public:
		template <UIElementType T, typename... Args>
		[[nodiscard]] static auto Create(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		{
			auto element = std::make_shared<T>(std::forward<Args>(args)...);

			return element;
		}

		virtual ~UIElement() = default;

		virtual auto Render(const Graphics&) -> void = 0;
		virtual auto Invalidate() const noexcept -> void
		{
			if (parent != nullptr)
			{
				parent->Invalidate();
			}
			invalidateRequest.Invoke(this);
		}

		virtual auto HandleEvent(UIEvent&) -> void
		{
			/*  */
		}

		virtual auto Move(PointF position) -> void = 0;
		virtual auto Resize(SizeF size) -> void = 0;
		virtual auto MoveAndResize(PointF position, SizeF size) -> void = 0;
		virtual auto MoveAndResize(RectF rect) -> void = 0;

		[[nodiscard]] virtual auto GetRect() const noexcept -> RectF = 0;
		[[nodiscard]] virtual auto GetPosition() const noexcept -> PointF = 0;
		[[nodiscard]] virtual auto GetSize() const noexcept -> SizeF = 0;

		[[nodiscard]] virtual auto HitTest(PointF point) noexcept -> bool = 0;

		[[nodiscard]] auto ConvertToRelative(const RectF rect) const noexcept
		{
			const auto pos = GetPosition();
			return rect.Shifted(pos);
		}
		[[nodiscard]] auto ConvertToAbsolute(const RectF rect) const noexcept
		{
			const auto pos = GetPosition();
			return rect.Shifted(-pos);
		}
		[[nodiscard]] auto ConvertToRelative(const PointF point) const noexcept
		{
			const auto pos = GetPosition();
			return point.Shifted(pos);
		}
		[[nodiscard]] auto ConvertToAbsolute(const PointF point) const noexcept
		{
			const auto pos = GetPosition();
			return point.Shifted(-pos);
		}

		[[nodiscard]] auto& InvalidateRequest() noexcept
		{
			return invalidateRequest;
		}
		[[nodiscard]] const auto& InvalidateRequest() const noexcept
		{
			return invalidateRequest;
		}

		auto SetFocus() noexcept -> void
		{
			if (!focusable)
			{
				return;
			}
			hasFocus = true;
			UIEvent focusEvent;
			focusEvent.type = EventType::GotFocus;
			HandleEvent(focusEvent);
		}
		auto RemoveFocus() noexcept
		{
			if (!focusable)
			{
				return;
			}
			hasFocus = false;
			UIEvent focusEvent;
			focusEvent.type = EventType::LostFocus;
			HandleEvent(focusEvent);
		}
		[[nodiscard]] auto HasFocus() const noexcept
		{
			return *hasFocus;
		}
		[[nodiscard]] auto IsFocusable() const noexcept
		{
			return focusable;
		}

		virtual auto ChildRemoved(RawCUIElementPtr) noexcept -> void
		{
			// Do nothing by default
		}
		auto SetParent(const RawUIElementPtr newParent) noexcept
		{
			if (parent == newParent)
			{
				return;
			}

			if (parent != nullptr)
			{
				parent->ChildRemoved(this);
			}
			parent = newParent;
		}
		[[nodiscard]] auto HasParent() const noexcept
		{
			return parent != nullptr;
		}
		[[nodiscard]] auto GetParent() const noexcept
		{
			return parent;
		}
		[[nodiscard]] auto GetRoot() const noexcept
		{
			auto current = parent;
			while (current && current->HasParent())
			{
				current = current->GetParent();
			}
			return current;
		}

		[[nodiscard]] const auto& GetZIndex() const noexcept
		{
			return zIndex;
		}
		[[nodiscard]] auto& GetZIndex() noexcept
		{
			return zIndex;
		}
		auto SetZIndex(const ZIndex newZIndex) noexcept
		{
			zIndex = newZIndex;
		}

		auto SetTabStop(const bool tabStop) noexcept
		{
			isTabStop = tabStop;
		}
		[[nodiscard]] auto IsTabStop() const noexcept
		{
			return isTabStop;
		}

		[[nodiscard]] const auto& IsEnabled() const noexcept
		{
			return enabled;
		}
		[[nodiscard]] auto& IsEnabled() noexcept
		{
			return enabled;
		}
		auto SetEnabled(const bool isEnabled) noexcept -> void
		{
			enabled = isEnabled;
		}
		auto Enable() noexcept -> void
		{
			if (!*enabled)
			{
				SetEnabled(true);
			}
		}
		auto Disable() noexcept -> void
		{
			if (*enabled)
			{
				SetEnabled(false);
			}
		}

		[[nodiscard]] auto GetID() const noexcept -> std::uint64_t
		{
			return std::bit_cast<std::uintptr_t>(this);
		}

		[[nodiscard]] auto IsVisible() const noexcept -> bool
		{
			if (parent == nullptr)
			{
				return true;
			}
			return parent->GetRect().Intersects(GetRect());
		}

		auto operator==(const UIElement& other) const noexcept -> bool
		{
			return this == &other;
		}

		protected:
		auto SetFocusable(const bool isFocusable) noexcept -> void
		{
			focusable = isFocusable;
		}

		private:
		RawUIElementPtr parent = nullptr;
		bool focusable = false;
		bool isTabStop = false;
		EventSRWM<RawCUIElementPtr> invalidateRequest;
		DataBinding::PropertyNM<bool> hasFocus{ false };
		DataBinding::PropertyNM<bool> enabled{ true };
		DataBinding::PropertyNM<ZIndex> zIndex{ ZIndices::Normal };
	};
}

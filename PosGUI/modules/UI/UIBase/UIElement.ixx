export module PGUI.UI.UIBase:UIElement;

import :Interface;

import PGUI.DataBinding.Property;
import PGUI.UI.Graphics;
import PGUI.Shape2D;

export namespace PGUI::UI
{
	class UIElement
	{
		public:
		template <UIElementType T, typename... Args>
		[[nodiscard]] static auto Create(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		{
			auto element = std::make_shared<T>(std::forward<Args>(args)...);

			return element;
		}

		virtual ~UIElement() = default;

		virtual auto Render(Graphics) -> void = 0;
		virtual auto Invalidate() const noexcept -> void
		{
			if (parent != nullptr)
			{
				parent->Invalidate();
			}
		}

		virtual auto Move(PointF position) -> void = 0;
		virtual auto Resize(SizeF size) -> void = 0;
		virtual auto MoveAndResize(PointF position, SizeF size) -> void = 0;
		virtual auto MoveAndResize(RectF rect) -> void = 0;

		[[nodiscard]] virtual auto GetRect() const noexcept -> RectF = 0;
		[[nodiscard]] virtual auto GetPosition() const noexcept -> PointF = 0;
		[[nodiscard]] virtual auto GetSize() const noexcept -> SizeF = 0;

		[[nodiscard]] virtual auto HitTest(PointF point) noexcept -> bool = 0;

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
		[[nodiscard]] auto GetAncestor() const noexcept
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
			if (*enabled)
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

		private:
		RawUIElementPtr parent = nullptr;
		DataBinding::PropertyNM<bool> enabled = true;
		DataBinding::PropertyNM<ZIndex> zIndex = ZIndices::Normal;
	};
}

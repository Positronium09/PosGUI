module;
#include <dcomp.h>

export module PGUI.UI.UICore:UIElement;

import std;

import :Interface;
import :UIEvent;

import PGUI.ComPtr;
import PGUI.DataBinding;
import PGUI.ErrorHandling;
import PGUI.Shape2D;
import PGUI.Event;
import PGUI.UI.Graphics;

export namespace PGUI::UI
{
	using ZIndex = std::uint64_t;

	namespace ZIndices
	{
		// ReSharper disable CppUseAutoForNumeric

		constexpr ZIndex Normal = 0ULL;
		constexpr ZIndex Tooltip = 10000ULL;
		constexpr ZIndex Debug = std::numeric_limits<ZIndex>::max();
		
		// ReSharper restore CppUseAutoForNumeric
	}

	class UIElement
	{
		friend UIHost;
		friend UIContainer;

		public:
		explicit UIElement(const RectF& rect) noexcept : 
			rect{ rect }
		{
		}
		virtual ~UIElement() noexcept = default;
		UIElement(const UIElement&) = delete;
		auto operator=(const UIElement&) -> UIElement& = delete;
		UIElement(UIElement&&) noexcept = default;
		auto operator=(UIElement&&) noexcept -> UIElement& = default;

		template <UIElementType T, typename... Args>
		[[nodiscard]] static auto Create(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		[[nodiscard]] auto GetParent() const noexcept { return parent; }

		[[nodiscard]] auto IsFocusable() const noexcept { return canHaveFocus; }
		[[nodiscard]] auto IsTabStop() const noexcept { return isTabStop; }

		[[nodiscard]] auto IsEnabled() const noexcept { return isEnabled.Get(); }
		[[nodiscard]] auto HasFocus() const noexcept { return hasFocus.Get(); }
		[[nodiscard]] auto SetFocus() noexcept { hasFocus.Set(true); }
		[[nodiscard]] auto RemoveFocus() noexcept { hasFocus.Set(false); }
		[[nodiscard]] auto Enable() noexcept { isEnabled.Set(true); }
		[[nodiscard]] auto Disable() noexcept { isEnabled.Set(false); }

		[[nodiscard]] auto GetZIndex() const noexcept { return zIndex.Get(); }

		[[nodiscard]] auto IsComposed() const noexcept
		{
			return compositionVisual.get() != nullptr && compositionSurface.get() != nullptr;
		}

		template <typename Self>
		[[nodiscard]] auto&& IsEnabledEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.isEnabled);
		}
		template <typename Self>
		[[nodiscard]] auto&& HasFocusEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.hasFocus);
		}
		template <typename Self>
		[[nodiscard]] auto&& ZIndexEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.zIndex);
		}

		[[nodiscard]] virtual auto GetRect() const noexcept -> RectF
		{
			return rect;
		}
		[[nodiscard]] virtual auto GetSize() const noexcept -> SizeF
		{
			return rect.Size();
		}
		[[nodiscard]] virtual auto GetPosition() const noexcept -> PointF
		{
			return rect.TopLeft();
		}
		virtual auto MoveAndResize(const RectF newRect) noexcept -> void
		{
			rect = newRect;
		}
		virtual auto MoveAndResize(const PointF point, const SizeF size) noexcept -> void
		{
			rect = RectF{ point, size };
		}
		virtual auto Move(const PointF point) noexcept -> void
		{
			rect = RectF{ point, GetSize() };
		}
		virtual auto Resize(const SizeF size) noexcept -> void
		{
			rect = RectF{ GetPosition(), size };
		}

		virtual auto Render(const Graphics&) noexcept -> void
		{
			/*  */
		}
		virtual auto HitTest(const PointF point) noexcept -> bool
		{
			return rect.Contains(point);
		}
		virtual auto HandleEvent(const UIEvent&) noexcept -> void
		{
			/*  */
		}
		virtual auto CreateDeviceResources() noexcept -> void
		{
			/*  */
		}
		virtual auto DiscardDeviceResources() noexcept -> void
		{
			/*  */
		}

		protected:
		UIElement() noexcept = default;

		auto SetParent(RawUIElementPtr<> newParent) noexcept;

		auto GetHost() const noexcept { return host; }

		auto AllowFocus() noexcept { canHaveFocus = true; }
		auto DisallowFocus() noexcept;

		auto SetZIndex(const ZIndex value) noexcept { zIndex.Set(value); }

		auto SetTabStop(const bool value) noexcept { isTabStop = value; }

		auto RequestRedraw() noexcept -> void;

		template <typename Self>
		[[nodiscard]] auto&& RedrawRequestedEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.redrawRequestedEvent);
		}

		auto EnableComposition() noexcept -> Error
		{
			return CompositionModeChanged(true);
		}
		auto DisableComposition() noexcept -> Error
		{
			return CompositionModeChanged(false);
		}

		private:
		RectF rect;
		bool isTabStop = false;
		bool canHaveFocus = false;
		Event<RawUIElementPtr<>> redrawRequestedEvent;
		DataBinding::PropertyNM<ZIndex> zIndex{ ZIndices::Normal };
		DataBinding::PropertyNM<bool> isEnabled{ true };
		DataBinding::PropertyNM<bool> hasFocus{ false };
		RawUIElementPtr<> parent = nullptr;
		RawUIHostPtr<> host = nullptr;
		ComPtr<IDCompositionVisual3> compositionVisual = nullptr;
		ComPtr<IDCompositionVirtualSurface> compositionSurface = nullptr;

		auto CompositionModeChanged(bool enabled) noexcept -> Error;
	};
}

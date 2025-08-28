module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module PGUI.UI.UIBase:UIComponent;

import std;

import :Interface;
import PGUI.Window;
import PGUI.Event;
import PGUI.Shape2D;
import PGUI.WindowClass;
import PGUI.UI.Clip;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	enum class UIComponentState
	{
		Hidden = 1 << 0,
		Visible = 1 << 1,
		Normal = 1 << 2,
		Hovered = 1 << 3,
		Focused = 1 << 4,
		Pressed = 1 << 5,
		Disabled = 1 << 6
	};
	DEFINE_ENUM_FLAG_OPERATORS(UIComponentState);

	using ZIndex = int;
	namespace ZIndices
	{
		constexpr auto Background = -1000;
		constexpr auto Normal = 0;
		constexpr auto Elevated = 100;
		constexpr auto Floating = 500;
		constexpr auto Modal = 1000;
		constexpr auto Tooltip = 2000;
		constexpr auto Notification = 5000;
		constexpr auto Debug = 10000;
	}

	class UIComponent
	{
		friend class UIWindow;

		public:
#pragma region Events

		EventSRWM<> clipChanged;
		EventSRWM<const RectF&, const RectF&> boundsChanged;
		EventSRWM<PointF> clickedEvent;
		EventSRWM<UIComponentState, UIComponentState> stateChanged;
		EventSRWM<ZIndex> zIndexChanged;

#pragma endregion

		explicit UIComponent(RectF bounds);

		virtual ~UIComponent() = default;

#pragma region Properties

		auto SetClip(const ClipParameters& params) noexcept -> void;
		[[nodiscard]] const auto& GetClip() const noexcept { return clip; }
		auto ClearClip() noexcept -> void;

		auto SetHitTestClipGeometry(const bool enable) noexcept -> void { hitTestClip = enable; }
		[[nodiscard]] auto GetHitTestClipGeometryEnabled() const noexcept { return hitTestClip; }

		auto SetAdjustClipOnSize(const bool enable) noexcept -> void { adjustClipOnSize = enable; }
		[[nodiscard]] auto GetAdjustClipOnSizeEnabled() const noexcept { return adjustClipOnSize; }

		auto SetBounds(const RectF& rect) noexcept -> void;
		[[nodiscard]] auto GetBounds() const noexcept { return bounds; }

		auto SetZIndex(const ZIndex z) noexcept { zIndex = z; zIndexChanged.Invoke(zIndex); }
		[[nodiscard]] auto GetZIndex() const noexcept { return zIndex; }

		auto Move(const PointF point) noexcept -> void
		{
			SetBounds(RectF{ point, bounds.Size() });
		}
		auto Resize(const SizeF size) noexcept -> void
		{
			SetBounds(RectF{ bounds.TopLeft(), size });
		}
		auto MoveAndResize(const RectF rect) noexcept -> void
		{
			SetBounds(rect);
		}
		auto MoveAndResize(const PointF point, const SizeF size) noexcept -> void
		{
			SetBounds(RectF{ point, size });
		}
		[[nodiscard]] auto GetPosition() const noexcept { return bounds.TopLeft(); }
		[[nodiscard]] auto GetSize() const noexcept { return bounds.Size(); }

#pragma endregion

		protected:
		auto AdjustClip() noexcept -> void;

		private:
		Clip clip;
		RectF bounds;
		ZIndex zIndex = ZIndices::Normal;
		RawUIWindowPtr<> parent = nullptr;
		std::vector<UIComponentPtr<>> children;
		bool hitTestClip = true;
		bool adjustClipOnSize = true;
		UIComponentState state = UIComponentState::Normal | UIComponentState::Visible;
	};
}

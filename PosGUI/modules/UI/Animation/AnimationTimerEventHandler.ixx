module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTimerEventHandler;

import std;
import PGUI.ComPtr;
import PGUI.Event;

namespace PGUI::UI::Animation
{
	class AnimationTimerEventHandlerRouter final :
		public Implements<AnimationTimerEventHandlerRouter, IUIAnimationTimerEventHandler>
	{
		using UpdateHandler = std::function<HRESULT()>;
		using SlowRenderingHandler = std::function<HRESULT(UINT32)>;

		public:
		AnimationTimerEventHandlerRouter() noexcept = default;
		AnimationTimerEventHandlerRouter(
			const UpdateHandler& postUpdateHandler,
			const UpdateHandler& preUpdateHandler,
			const SlowRenderingHandler& renderingTooSlowHandler) noexcept;
		
		auto __stdcall OnPostUpdate() -> HRESULT override;
		auto __stdcall OnPreUpdate() -> HRESULT override;
		auto __stdcall OnRenderingTooSlow(UINT32 fps) -> HRESULT override;

		auto SetPostUpdateHandler(const UpdateHandler& handler) noexcept -> void;
		auto SetPreUpdateHandler(const UpdateHandler& handler) noexcept -> void;
		auto SetRenderingTooSlowHandler(const SlowRenderingHandler& handler) noexcept -> void;

		auto ClearPostUpdateHandler() noexcept -> void;
		auto ClearPreUpdateHandler() noexcept -> void;
		auto ClearRenderingTooSlowHandler() noexcept -> void;
		
		private:
		std::mutex postUpdateHandlerMutex;
		std::mutex preUpdateHandlerMutex;
		std::mutex renderingTooSlowHandlerMutex;
		UpdateHandler postUpdateHandler;
		UpdateHandler preUpdateHandler;
		SlowRenderingHandler renderingTooSlowHandler;
	};
}

export namespace PGUI::UI::Animation
{
	class AnimationTimerEventHandler
	{
		public:
		AnimationTimerEventHandler() noexcept;

		virtual ~AnimationTimerEventHandler() noexcept;

		[[nodiscard]] auto& GetRouter() noexcept { return router; }
		[[nodiscard]] const auto& GetRouter() const noexcept { return router; }
		
		virtual auto OnPostUpdate() -> void = 0;
		virtual auto OnPreUpdate() -> void = 0;
		virtual auto OnRenderingTooSlow(UINT32 framesPerSecond) -> void = 0;

		private:
		ComPtr<AnimationTimerEventHandlerRouter> router;

		auto CallPostUpdateHandler() noexcept -> HRESULT;
		auto CallPreUpdateHandler() noexcept -> HRESULT;
		auto CallRenderingTooSlowHandler(UINT32 fps) noexcept -> HRESULT;
	};

	class AnimationTimerEvent final : public AnimationTimerEventHandler
	{
		[[nodiscard]] auto& PostUpdateEvent() noexcept { return postUpdateEvent; }
		[[nodiscard]] const auto& PostUpdateEvent() const noexcept { return postUpdateEvent; }
		[[nodiscard]] auto& PreUpdateEvent() noexcept { return preUpdateEvent; }
		[[nodiscard]] const auto& PreUpdateEvent() const noexcept { return preUpdateEvent; }
		[[nodiscard]] auto& RenderingTooSlowEvent() noexcept { return renderingTooSlowEvent; }
		[[nodiscard]] const auto& RenderingTooSlowEvent() const noexcept { return renderingTooSlowEvent; }

		private:
		Event<> postUpdateEvent;
		Event<> preUpdateEvent;
		Event<UINT32> renderingTooSlowEvent;

		auto OnPostUpdate() -> void override;
		auto OnPreUpdate() -> void override;
		auto OnRenderingTooSlow(UINT32 framesPerSecond) -> void override;
	};
}

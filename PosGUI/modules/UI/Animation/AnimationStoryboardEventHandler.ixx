module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationStoryboardEventHandler;

import std;

import PGUI.Event;
import PGUI.UI.Animation.AnimationEnums;

namespace PGUI::UI::Animation
{
	export class Storyboard;
	class AnimationStoryboardEventHandlerRouter final : public IUIAnimationStoryboardEventHandler2
	{
		using StoryboardStatusChangedHandler = std::function<HRESULT(Storyboard, StoryboardStatus, StoryboardStatus)>;
		using StoryboardUpdatedHandler = std::function<HRESULT(Storyboard)>;

		public:
		AnimationStoryboardEventHandlerRouter() noexcept = default;
		AnimationStoryboardEventHandlerRouter(
			const StoryboardStatusChangedHandler& statusChangedHandler,
			const StoryboardUpdatedHandler& updatedHandler) noexcept;

		auto __stdcall QueryInterface(const IID& iid, void** obj) -> HRESULT override;
		auto __stdcall AddRef() -> ULONG override;
		auto __stdcall Release() -> ULONG override;

		auto __stdcall OnStoryboardStatusChanged(IUIAnimationStoryboard2* storyboard,
			UI_ANIMATION_STORYBOARD_STATUS newStatus,
			UI_ANIMATION_STORYBOARD_STATUS previousStatus) -> HRESULT override;
		auto __stdcall OnStoryboardUpdated(IUIAnimationStoryboard2* storyboard) -> HRESULT override;

		void SetStatusChangedHandler(const StoryboardStatusChangedHandler& handler) noexcept;
		void ClearStatusChangedHandler() noexcept;
		void SetUpdatedHandler(const StoryboardUpdatedHandler& handler) noexcept;
		void ClearUpdatedHandler() noexcept;

		private:
		std::mutex updateHandlerMutex;
		std::mutex statusChangedHandlerMutex;
		StoryboardUpdatedHandler updatedHandler{ };
		StoryboardStatusChangedHandler statusChangedHandler{ };
		LONG referenceCount = 1;
	};
}

export namespace PGUI::UI::Animation
{
	class AnimationStoryboardEventHandler
	{
		public:
		AnimationStoryboardEventHandler() noexcept;

		[[nodiscard]] auto& GetRouter() noexcept { return router; }
		[[nodiscard]] const auto& GetRouter() const noexcept { return router; }

		virtual void OnStoryBoardStatusChanged(Storyboard storyboard,
			StoryboardStatus newStatus, StoryboardStatus previousStatus) = 0;
		virtual void OnStoryBoardUpdated(Storyboard storyboard) = 0;

		private:
		AnimationStoryboardEventHandlerRouter router;

		auto CallStoryBoardStatusChangedHandler(Storyboard storyboard,
			StoryboardStatus newStatus, StoryboardStatus previousStatus) noexcept -> HRESULT;
		auto CallStoryBoardUpdatedHandler(Storyboard storyboard) noexcept -> HRESULT;
	};

	class AnimationStoryboardEvent : public AnimationStoryboardEventHandler
	{
		public:
		[[nodiscard]] const auto& StoryboardStatusChanged() const noexcept { return storyboardStatusChangedEvent; }
		[[nodiscard]] auto& StoryboardStatusChanged() noexcept { return storyboardStatusChangedEvent; }
		[[nodiscard]] const auto& StoryboardUpdated() const noexcept { return storyboardUpdatedEvent; }
		[[nodiscard]] auto& StoryboardUpdated() noexcept { return storyboardUpdatedEvent; }

		private:
		Event<Storyboard, 
			StoryboardStatus, StoryboardStatus> storyboardStatusChangedEvent;
		Event<Storyboard> storyboardUpdatedEvent;

		void OnStoryBoardStatusChanged(Storyboard storyboard, 
			StoryboardStatus newStatus, StoryboardStatus previousStatus) override;
		void OnStoryBoardUpdated(Storyboard storyboard) override;
	};
}

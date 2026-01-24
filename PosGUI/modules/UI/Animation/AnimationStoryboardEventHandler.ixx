module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationStoryboardEventHandler;

import std;

import PGUI.Event;
import PGUI.ComPtr;
import :AnimationInterface;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	class AnimationStoryboardEventHandlerRouter final : 
		public Implements<AnimationStoryboardEventHandlerRouter, IUIAnimationStoryboardEventHandler2>
	{
		using StoryboardStatusChangedHandler = std::function<HRESULT(Storyboard, StoryboardStatus, StoryboardStatus)>;
		using StoryboardUpdatedHandler = std::function<HRESULT(Storyboard)>;

		public:
		AnimationStoryboardEventHandlerRouter() noexcept = default;

		AnimationStoryboardEventHandlerRouter(
			const StoryboardStatusChangedHandler& statusChangedHandler,
			const StoryboardUpdatedHandler& updatedHandler) noexcept;

		auto __stdcall OnStoryboardStatusChanged(
			IUIAnimationStoryboard2* storyboard,
			UI_ANIMATION_STORYBOARD_STATUS newStatus,
			UI_ANIMATION_STORYBOARD_STATUS previousStatus)->HRESULT override;

		auto __stdcall OnStoryboardUpdated(IUIAnimationStoryboard2* storyboard) -> HRESULT override;

		auto SetStatusChangedHandler(const StoryboardStatusChangedHandler& handler) noexcept -> void;

		auto ClearStatusChangedHandler() noexcept -> void;

		auto SetUpdatedHandler(const StoryboardUpdatedHandler& handler) noexcept -> void;

		auto ClearUpdatedHandler() noexcept -> void;

		private:
		std::mutex updateHandlerMutex;
		std::mutex statusChangedHandlerMutex;
		StoryboardUpdatedHandler updatedHandler{ };
		StoryboardStatusChangedHandler statusChangedHandler{ };
	};
}

export namespace PGUI::UI::Animation
{
	class AnimationStoryboardEventHandler
	{
		public:
		AnimationStoryboardEventHandler() noexcept;

		virtual ~AnimationStoryboardEventHandler() = default;

		[[nodiscard]] auto& GetRouter() noexcept { return router; }
		[[nodiscard]] const auto& GetRouter() const noexcept { return router; }

		virtual auto OnStoryBoardStatusChanged(
			Storyboard storyboard,
			StoryboardStatus newStatus, StoryboardStatus previousStatus) -> void = 0;

		virtual auto OnStoryBoardUpdated(Storyboard storyboard) -> void = 0;

		private:
		ComPtr<AnimationStoryboardEventHandlerRouter> router;

		auto CallStoryBoardStatusChangedHandler(
			const Storyboard& storyboard,
			StoryboardStatus newStatus,
			StoryboardStatus previousStatus) noexcept -> HRESULT;

		auto CallStoryBoardUpdatedHandler(const Storyboard& storyboard) noexcept -> HRESULT;
	};

	class AnimationStoryboardEvent final : public AnimationStoryboardEventHandler
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

		auto OnStoryBoardStatusChanged(
			Storyboard storyboard,
			StoryboardStatus newStatus, StoryboardStatus previousStatus) -> void override;

		auto OnStoryBoardUpdated(Storyboard storyboard) -> void override;
	};
}

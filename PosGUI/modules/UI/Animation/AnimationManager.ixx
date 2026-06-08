module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationManager;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationInterface;
import :AnimationEnums;
import :AnimationTimeTypes;

export namespace PGUI::UI::Animation
{
	class AnimationManager : public ComPtrHolder<IUIAnimationManager2>
	{
		public:
		[[nodiscard]] static auto GetGlobalInstance() -> const AnimationManager&;

		AnimationManager();

		[[nodiscard]] auto AbandonAllStoryboards() noexcept -> Result<void>;

		[[nodiscard]] auto Pause() const noexcept -> Result<void>;

		[[nodiscard]] auto Resume() const noexcept -> Result<void>;

		[[nodiscard]] auto Shutdown() const noexcept -> Result<void>;

		[[nodiscard]] auto Update(Seconds timeNow) const noexcept -> Result<AnimationUpdateResult>;

		[[nodiscard]] auto CreateAnimationVariable(double initialValue) const noexcept -> Result<AnimationVariable>;

		[[nodiscard]] auto CreateAnimationVariable(std::span<const double> initialValues) const noexcept -> Result<AnimationVariable>;

		[[nodiscard]] auto CreateStoryboard() const noexcept -> Result<Storyboard>;

		[[nodiscard]] auto GetStatus() const noexcept -> Result<AnimationManagerStatus>;

		[[nodiscard]] auto EstimateNextEventTime() const noexcept -> Result<Seconds>;

		[[nodiscard]] auto SetAnimationMode(AnimationMode mode) const noexcept -> Result<void>;

		[[nodiscard]] auto SetDefaultLongestAcceptableDelay(Seconds delay) const noexcept -> Result<void>;

		[[nodiscard]] auto GetStoryboardFromTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Result<Storyboard>;

		[[nodiscard]] auto GetAnimationVariableFromTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Result<AnimationVariable>;

		[[nodiscard]] auto ScheduleTransition(
			const AnimationVariable& variable,
			const AnimationTransition& transition, double currentTime) const noexcept -> Result<void>;

		[[nodiscard]] auto SetManagerEventHandler(
			AnimationManagerEventHandler& eventHandler,
			bool registerForNext = false) const noexcept -> Result<void>;

		[[nodiscard]] auto ClearManagerEventHandler(bool registerForNext = false) const noexcept -> Result<void>;

		//TODO SetCancelPriorityComparison
		//TODO SetCompressPriorityComparison 
		//TODO SetConcludePriorityComparison
		//TODO SetTrimPriorityComparison
	};
}

module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationManager;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	class AnimationManager : public ComPtrHolder<IUIAnimationManager2>
	{
		public:
		[[nodiscard]] static auto GetGlobalInstance() -> const AnimationManager&;

		AnimationManager();

		auto AbandonAllStoryboards() noexcept -> Error;

		auto Pause() const noexcept -> Error;

		auto Resume() const noexcept -> Error;

		auto Shutdown() const noexcept -> Error;

		[[nodiscard]] auto Update(double timeNow) const noexcept -> Result<AnimationUpdateResult>;

		[[nodiscard]] auto CreateAnimationVariable(double initialValue) const noexcept -> Result<AnimationVariable>;

		[[nodiscard]] auto CreateAnimationVariable(std::span<const double> initialValues) const noexcept -> Result<AnimationVariable>;

		[[nodiscard]] auto CreateStoryboard() const noexcept -> Result<Storyboard>;

		[[nodiscard]] auto GetStatus() const noexcept -> Result<AnimationManagerStatus>;

		[[nodiscard]] auto EstimateNextEventTime() const noexcept -> Result<double>;

		auto SetAnimationMode(AnimationMode mode) const noexcept -> Error;

		auto SetDefaultLongestAcceptableDelay(double delay) const noexcept -> Error;

		[[nodiscard]] auto GetStoryboardFromTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Result<Storyboard>;

		[[nodiscard]] auto GetAnimationVariableFromTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Result<AnimationVariable>;

		auto ScheduleTransition(
			const AnimationVariable& variable,
			AnimationTransition transition, double currentTime) const noexcept -> Error;

		auto SetManagerEventHandler(
			AnimationManagerEventHandler& eventHandler,
			bool registerForNext = false) const noexcept -> Error;

		auto ClearManagerEventHandler(bool registerForNext = false) const noexcept -> Error;

		//TODO SetCancelPriorityComparison
		//TODO SetCompressPriorityComparison 
		//TODO SetConcludePriorityComparison
		//TODO SetTrimPriorityComparison

		private:
		inline static AnimationManager* instance;
	};
}

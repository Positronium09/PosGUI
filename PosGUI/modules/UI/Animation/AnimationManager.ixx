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
		[[nodiscard]] static auto GetInstance() -> const AnimationManager&;

		static auto AbandonAllStoryboards() noexcept -> Error;

		static auto Pause() noexcept -> Error;

		static auto Resume() noexcept -> Error;

		static auto Shutdown() noexcept -> Error;

		[[nodiscard]] static auto Update(double timeNow) noexcept -> Result<AnimationUpdateResult>;

		[[nodiscard]] static auto CreateAnimationVariable(double initialValue) noexcept -> Result<AnimationVariable>;

		[[nodiscard]] static auto CreateAnimationVariable(std::span<const double> initialValues) noexcept -> Result<AnimationVariable>;

		[[nodiscard]] static auto CreateStoryboard() noexcept -> Result<Storyboard>;

		[[nodiscard]] static auto GetStatus() noexcept -> Result<AnimationManagerStatus>;

		[[nodiscard]] static auto EstimateNextEventTime() noexcept -> Result<double>;

		static auto SetAnimationMode(AnimationMode mode) noexcept -> Error;

		static auto SetDefaultLongestAcceptableDelay(double delay) noexcept -> Error;

		[[nodiscard]] static auto GetStoryboardFromTag(const ComPtr<IUnknown>& obj, UINT32 id) noexcept -> Result<Storyboard>;

		[[nodiscard]] static auto GetAnimationVariableFromTag(const ComPtr<IUnknown>& obj, UINT32 id) noexcept -> Result<AnimationVariable>;

		static auto ScheduleTransition(
			const AnimationVariable& variable,
			AnimationTransition transition, double currentTime) noexcept -> Error;

		auto SetManagerEventHandler(
			AnimationManagerEventHandler& eventHandler,
			bool registerForNext = false) const noexcept -> Error;

		auto ClearManagerEventHandler(bool registerForNext = false) const noexcept -> Error;

		//TODO SetCancelPriorityComparison
		//TODO SetCompressPriorityComparison 
		//TODO SetConcludePriorityComparison
		//TODO SetTrimPriorityComparison

		private:
		AnimationManager();

		inline static AnimationManager* instance;
	};
}

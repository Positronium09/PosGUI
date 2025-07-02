module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationManager;

import std;

import PGUI.ComPtr;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	class AnimationManager : public ComPtrHolder<IUIAnimationManager2>
	{
		public:
		[[nodiscard]] static auto GetInstance() -> const AnimationManager&;

		static auto AbandonAllStoryboards() -> void;

		static auto Pause() -> void;

		static auto Resume() -> void;

		static auto Shutdown() -> void;

		[[nodiscard]] static auto Update(double timeNow) -> AnimationUpdateResult;

		[[nodiscard]] static auto CreateAnimationVariable(double initialValue) -> AnimationVariable;

		[[nodiscard]] static auto CreateAnimationVariable(std::span<const double> initialValues) -> AnimationVariable;

		[[nodiscard]] static auto CreateStoryboard() -> Storyboard;

		[[nodiscard]] static auto GetStatus() -> AnimationManagerStatus;

		[[nodiscard]] static auto EstimateNextEventTime() -> double;

		static auto SetAnimationMode(AnimationMode mode) -> void;

		static auto SetDefaultLongestAcceptableDelay(double delay) -> void;

		[[nodiscard]] static auto GetStoryboardFromTag(ComPtr<IUnknown> obj, UINT32 id) -> Storyboard;

		[[nodiscard]] static auto GetAnimationVariableFromTag(ComPtr<IUnknown> obj, UINT32 id) -> AnimationVariable;

		static auto ScheduleTransition(const AnimationVariable& variable,
		                               AnimationTransition transition, double currentTime) -> void;

		auto SetManagerEventHandler(
			AnimationManagerEventHandler& eventHandler,
			bool registerForNext = false) const -> void;

		auto ClearManagerEventHandler(bool registerForNext = false) const -> void;

		//TODO SetCancelPriorityComparison
		//TODO SetCompressPriorityComparison 
		//TODO SetConcludePriorityComparison
		//TODO SetTrimPriorityComparison

		private:
		AnimationManager();

		inline static AnimationManager* instance;
	};
}

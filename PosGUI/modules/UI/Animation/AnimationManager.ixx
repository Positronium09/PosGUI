module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationManager;

import std;

import PGUI.ComPtr;
import PGUI.UI.Animation.AnimationEnums;
import PGUI.UI.Animation.Storyboard;
import PGUI.UI.Animation.AnimationVariable;
import PGUI.UI.Animation.AnimationTransition;
import PGUI.UI.Animation.AnimationManagerEventHandler;

export namespace PGUI::UI::Animation
{
	class AnimationManager : public ComPtrHolder<IUIAnimationManager2>
	{
		public:
		[[nodiscard]] static auto GetInstance() -> const AnimationManager&;

		static void AbandonAllStoryboards();

		static void Pause();
		static void Resume();
		static void Shutdown();
		[[nodiscard]] static auto Update(double timeNow) -> AnimationUpdateResult;

		[[nodiscard]] static auto CreateAnimationVariable(double initialValue) -> AnimationVariable;
		[[nodiscard]] static auto CreateAnimationVariable(std::span<const double> initialValues) -> AnimationVariable;
		[[nodiscard]] static auto CreateStoryboard() -> Storyboard;

		[[nodiscard]] static auto GetStatus() -> AnimationManagerStatus;

		[[nodiscard]] static auto EstimateNextEventTime() -> double;

		static void SetAnimationMode(AnimationMode mode);

		static void SetDefaultLongestAcceptableDelay(double delay);

		[[nodiscard]] static auto GetStoryboardFromTag(ComPtr<IUnknown> obj, UINT32 id) -> Storyboard;
		[[nodiscard]] static auto GetAnimationVariableFromTag(ComPtr<IUnknown> obj, UINT32 id) -> AnimationVariable;

		static void ScheduleTransition(const AnimationVariable& variable,
			AnimationTransition transition, double currentTime);

		void SetManagerEventHandler(
			AnimationManagerEventHandler& eventHandler,
			bool registerForNext = false) const;
		void ClearManagerEventHandler(bool registerForNext = false) const;
		//TODO SetCancelPriorityComparison
		//TODO SetCompressPriorityComparison 
		//TODO SetConcludePriorityComparison
		//TODO SetTrimPriorityComparison

		private:
		AnimationManager();

		inline static AnimationManager* instance;
	};
}

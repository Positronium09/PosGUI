module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.Storyboard;

import PGUI.ComPtr;
import PGUI.UI.Animation.AnimationEnums;

export namespace  PGUI::UI::Animation
{
	class AnimationVariable;
	class AnimationTransition;

	using KeyFrame = UI_ANIMATION_KEYFRAME;
	const auto StartKeyFrame = KeyFrame(-1);

	class Storyboard : public ComPtrHolder<IUIAnimationStoryboard2>
	{
		public:
		Storyboard() noexcept = default;
		Storyboard(ComPtr<IUIAnimationStoryboard2> ptr) noexcept;

		auto Schedule(double timeNow) const -> AnimationSchedulingResult;
		void Abandon() const;
		void Conclude() const;
		void Finish(double completionDeadline) const;

		void SetSkipDuration(double duration) const;
		void SetLongestAcceptableDelay(double delay) const;

		void HoldVariable(const AnimationVariable& variable) const;

		[[nodiscard]] auto AddKeyframeAfterTransition(const AnimationTransition& transition) const -> KeyFrame;
		[[nodiscard]] auto AddKeyframeAtOffset(KeyFrame keyFrame, double durationOffset) const -> KeyFrame;

		void AddTransition(const AnimationVariable& variable, AnimationTransition transition) const;
		void AddTransitionAtKeyframe(const AnimationVariable& variable, AnimationTransition transition, KeyFrame keyFrame) const;
		void AddTransitionBetweenKeyframes(const AnimationVariable& variable, AnimationTransition transition, 
			KeyFrame startKeyFrame, KeyFrame endKeyFrame) const;

		[[nodiscard]] auto GetStatus() const -> StoryboardStatus;
		[[nodiscard]] auto GetElapsedTime() const -> double;

		void SetTag(ComPtr<IUnknown> obj, UINT32 id) const;
		[[nodiscard]] auto GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>;

		//TODO SetStoryboardEventHandler
	};
}

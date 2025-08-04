module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:Storyboard;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	using KeyFrame = UI_ANIMATION_KEYFRAME;
	const auto StartKeyFrame = reinterpret_cast<KeyFrame>(-1);

	constexpr double RepeatIndefinitely = UI_ANIMATION_REPEAT_INDEFINITELY;

	class Storyboard : public ComPtrHolder<IUIAnimationStoryboard2>
	{
		public:
		Storyboard() noexcept = default;

		explicit(false) Storyboard(const ComPtr<IUIAnimationStoryboard2>& ptr) noexcept;

		auto Schedule(double timeNow) const noexcept -> Result<AnimationSchedulingResult>;

		auto Abandon() const noexcept -> Error;

		auto Conclude() const noexcept -> Error;

		auto Finish(double completionDeadline) const noexcept -> Error;

		auto SetSkipDuration(double duration) const noexcept -> Error;

		auto SetLongestAcceptableDelay(double delay) const noexcept -> Error;

		auto HoldVariable(const AnimationVariable& variable) const noexcept -> Error;

		[[nodiscard]] auto AddKeyframeAfterTransition(
			const AnimationTransition& transition) noexcept -> Result<KeyFrame>;

		[[nodiscard]] auto AddKeyframeAtOffset(
			KeyFrame keyFrame, 
			double durationOffset) noexcept -> Result<KeyFrame>;

		auto AddTransition(
			const AnimationVariable& variable, 
			AnimationTransition transition) const noexcept -> Error;

		auto AddTransitionAtKeyframe(
			const AnimationVariable& variable, AnimationTransition transition,
			KeyFrame keyFrame) const noexcept -> Error;

		auto AddTransitionBetweenKeyframes(
			const AnimationVariable& variable, AnimationTransition transition,
			KeyFrame startKeyFrame, KeyFrame endKeyFrame) const noexcept -> Error;

		auto RepeatBetweenKeyframes(
			KeyFrame startKeyframe, KeyFrame endKeyframe,
			double iterationCount, AnimationRepeatMode repeatMode,
			/* iterationChangeHandler, id */ bool registerForNext = false) const noexcept -> Error;

		[[nodiscard]] auto GetStatus() const noexcept -> Result<StoryboardStatus>;

		[[nodiscard]] auto GetElapsedTime() const noexcept -> Result<double>;

		auto SetTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Error;

		[[nodiscard]] auto GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>;

		auto SetStoryboardEventHandler(AnimationStoryboardEventHandler& eventHandler) const noexcept -> Error;
	};
}

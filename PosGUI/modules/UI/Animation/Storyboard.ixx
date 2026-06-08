module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:Storyboard;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationTimeTypes;
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

		auto Schedule(Seconds timeNow) const noexcept -> Result<AnimationSchedulingResult>;

		[[nodiscard]] auto Abandon() const noexcept -> Result<void>;

		[[nodiscard]] auto Conclude() const noexcept -> Result<void>;

		[[nodiscard]] auto Finish(Seconds completionDeadline) const noexcept -> Result<void>;

		[[nodiscard]] auto SetSkipDuration(Seconds duration) const noexcept -> Result<void>;

		[[nodiscard]] auto SetLongestAcceptableDelay(Seconds delay) const noexcept -> Result<void>;

		[[nodiscard]] auto HoldVariable(const AnimationVariable& variable) const noexcept -> Result<void>;

		[[nodiscard]] auto AddKeyframeAfterTransition(
			const AnimationTransition& transition) noexcept -> Result<KeyFrame>;

		[[nodiscard]] auto AddKeyframeAtOffset(
			KeyFrame keyFrame, 
			Seconds durationOffset) noexcept -> Result<KeyFrame>;

		[[nodiscard]] auto AddTransition(
			const AnimationVariable& variable, 
			const AnimationTransition& transition) const noexcept -> Result<void>;

		[[nodiscard]] auto AddTransitionAtKeyframe(
			const AnimationVariable& variable, const AnimationTransition& transition,
			KeyFrame keyFrame) const noexcept -> Result<void>;

		[[nodiscard]] auto AddTransitionBetweenKeyframes(
			const AnimationVariable& variable, const AnimationTransition& transition,
			KeyFrame startKeyFrame, KeyFrame endKeyFrame) const noexcept -> Result<void>;

		[[nodiscard]] auto RepeatBetweenKeyframes(
			KeyFrame startKeyFrame, KeyFrame endKeyFrame,
			double iterationCount, AnimationRepeatMode repeatMode,
			/* iterationChangeHandler, id */ bool registerForNext = false) const noexcept -> Result<void>;

		[[nodiscard]] auto GetStatus() const noexcept -> Result<StoryboardStatus>;

		[[nodiscard]] auto GetElapsedTime() const noexcept -> Result<Seconds>;

		[[nodiscard]] auto SetTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Result<void>;

		[[nodiscard]] auto GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>;

		[[nodiscard]] auto SetStoryboardEventHandler(AnimationStoryboardEventHandler& eventHandler) const noexcept -> Result<void>;
	};
}

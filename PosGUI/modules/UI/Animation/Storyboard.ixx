module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:Storyboard;

import std;

import PGUI.ComPtr;
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

		auto Schedule(double timeNow) const -> AnimationSchedulingResult;

		auto Abandon() const -> void;

		auto Conclude() const -> void;

		auto Finish(double completionDeadline) const -> void;

		auto SetSkipDuration(double duration) const -> void;

		auto SetLongestAcceptableDelay(double delay) const -> void;

		auto HoldVariable(const AnimationVariable& variable) const -> void;

		[[nodiscard]] auto AddKeyframeAfterTransition(const AnimationTransition& transition) -> KeyFrame;

		[[nodiscard]] auto AddKeyframeAtOffset(KeyFrame keyFrame, double durationOffset) -> KeyFrame;

		auto AddTransition(const AnimationVariable& variable, AnimationTransition transition) const -> void;

		auto AddTransitionAtKeyframe(
			const AnimationVariable& variable, AnimationTransition transition,
			KeyFrame keyFrame) const -> void;

		auto AddTransitionBetweenKeyframes(
			const AnimationVariable& variable, AnimationTransition transition,
			KeyFrame startKeyFrame, KeyFrame endKeyFrame) const -> void;

		auto RepeatBetweenKeyframes(
			KeyFrame startKeyframe, KeyFrame endKeyframe,
			double iterationCount, AnimationRepeatMode repeatMode,
			/* iterationChangeHandler, id */ bool registerForNext = false) const -> void;

		[[nodiscard]] auto GetStatus() const -> StoryboardStatus;

		[[nodiscard]] auto GetElapsedTime() const -> double;

		auto SetTag(ComPtr<IUnknown> obj, UINT32 id) const -> void;

		[[nodiscard]] auto GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>;

		auto SetStoryboardEventHandler(AnimationStoryboardEventHandler& eventHandler) const -> void;
	};
}

module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:Storyboard;
import :Storyboard;

import PGUI.ComPtr;
import PGUI.Exceptions;
import :AnimationVariable;
import :AnimationTransition;
import :AnimationStoryboardEventHandler;

namespace PGUI::UI::Animation
{
	Storyboard::Storyboard(const ComPtr<IUIAnimationStoryboard2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{
	}

	auto Storyboard::Schedule(const double timeNow) const -> AnimationSchedulingResult
	{
		UI_ANIMATION_SCHEDULING_RESULT result;
		const auto hr = Get()->Schedule(timeNow, &result);
		ThrowFailed(hr);

		return static_cast<AnimationSchedulingResult>(result);
	}

	auto Storyboard::Abandon() const -> void
	{
		const auto hr = Get()->Abandon();
		ThrowFailed(hr);
	}

	auto Storyboard::Conclude() const -> void
	{
		const auto hr = Get()->Conclude();
		ThrowFailed(hr);
	}

	auto Storyboard::Finish(const double completionDeadline) const -> void
	{
		const auto hr = Get()->Finish(completionDeadline);
		ThrowFailed(hr);
	}

	auto Storyboard::SetSkipDuration(const double duration) const -> void
	{
		const auto hr = Get()->SetSkipDuration(duration);
		ThrowFailed(hr);
	}

	auto Storyboard::SetLongestAcceptableDelay(const double delay) const -> void
	{
		const auto hr = Get()->SetLongestAcceptableDelay(delay);
		ThrowFailed(hr);
	}

	auto Storyboard::HoldVariable(const AnimationVariable& variable) const -> void
	{
		const auto hr = Get()->HoldVariable(variable.GetRaw());
		ThrowFailed(hr);
	}

	auto Storyboard::AddKeyframeAfterTransition(const AnimationTransition& transition) -> KeyFrame
	{
		KeyFrame keyFrame;
		const auto hr = Get()->AddKeyframeAfterTransition(transition.GetRaw(), &keyFrame);
		ThrowFailed(hr);

		return keyFrame;
	}

	auto Storyboard::AddKeyframeAtOffset(
		const KeyFrame keyFrame, const double durationOffset) -> KeyFrame
	{
		KeyFrame newKeyFrame;
		const auto hr = Get()->AddKeyframeAtOffset(keyFrame, durationOffset, &newKeyFrame);
		ThrowFailed(hr);

		return newKeyFrame;
	}

	auto Storyboard::AddTransition(const AnimationVariable& variable, AnimationTransition transition) const -> void
	{
		const auto hr = Get()->AddTransition(variable.GetRaw(), transition.GetRaw());
		ThrowFailed(hr);
	}

	auto Storyboard::AddTransitionAtKeyframe(
		const AnimationVariable& variable,
		AnimationTransition transition, const KeyFrame keyFrame) const -> void
	{
		const auto hr = Get()->AddTransitionAtKeyframe(
			variable.GetRaw(),
			transition.GetRaw(), keyFrame);
		ThrowFailed(hr);
	}

	auto Storyboard::AddTransitionBetweenKeyframes(
		const AnimationVariable& variable, AnimationTransition transition,
		const KeyFrame startKeyFrame, const KeyFrame endKeyFrame) const -> void
	{
		const auto hr = Get()->AddTransitionBetweenKeyframes(
			variable.GetRaw(),
			transition.GetRaw(), startKeyFrame, endKeyFrame);
		ThrowFailed(hr);
	}

	auto Storyboard::RepeatBetweenKeyframes(
		const KeyFrame startKeyframe, const KeyFrame endKeyframe,
		const double iterationCount, AnimationRepeatMode repeatMode,
		const bool registerForNext) const -> void
	{
		const auto hr = Get()->RepeatBetweenKeyframes(
			startKeyframe, endKeyframe,
			iterationCount, static_cast<UI_ANIMATION_REPEAT_MODE>(repeatMode),
			nullptr, 0, registerForNext);
		ThrowFailed(hr);
	}

	auto Storyboard::GetStatus() const -> StoryboardStatus
	{
		UI_ANIMATION_STORYBOARD_STATUS status;
		const auto hr = Get()->GetStatus(&status);
		ThrowFailed(hr);

		return static_cast<StoryboardStatus>(status);
	}

	auto Storyboard::GetElapsedTime() const -> double
	{
		auto elapsedTime = 0.0;
		const auto hr = Get()->GetElapsedTime(&elapsedTime);
		ThrowFailed(hr);

		return elapsedTime;
	}

	auto Storyboard::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const -> void
	{
		const auto hr = Get()->SetTag(obj.Get(), id);
		ThrowFailed(hr);
	}

	auto Storyboard::GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		const auto hr = Get()->GetTag(&obj, &tag);
		ThrowFailed(hr);

		return { obj, tag };
	}

	auto Storyboard::SetStoryboardEventHandler(AnimationStoryboardEventHandler& eventHandler) const -> void
	{
		const auto hr = Get()->SetStoryboardEventHandler(
			&eventHandler.GetRouter());
		ThrowFailed(hr);
	}
}

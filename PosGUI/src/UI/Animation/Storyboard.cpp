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

namespace  PGUI::UI::Animation
{
	Storyboard::Storyboard(const ComPtr<IUIAnimationStoryboard2>& ptr) noexcept : 
		ComPtrHolder{ ptr }
	{
	}

	auto Storyboard::Schedule(double timeNow) const -> AnimationSchedulingResult
	{
		UI_ANIMATION_SCHEDULING_RESULT result;
		auto hr = Get()->Schedule(timeNow, &result); ThrowFailed(hr);
		
		return static_cast<AnimationSchedulingResult>(result);
	}

	void Storyboard::Abandon() const
	{
		auto hr = Get()->Abandon(); ThrowFailed(hr);
	}

	void Storyboard::Conclude() const
	{
		auto hr = Get()->Conclude(); ThrowFailed(hr);
	}

	void Storyboard::Finish(double completionDeadline) const
	{
		auto hr = Get()->Finish(completionDeadline); ThrowFailed(hr);
	}

	void Storyboard::SetSkipDuration(double duration) const
	{
		auto hr = Get()->SetSkipDuration(duration); ThrowFailed(hr);
	}

	void Storyboard::SetLongestAcceptableDelay(double delay) const
	{
		auto hr = Get()->SetLongestAcceptableDelay(delay); ThrowFailed(hr);
	}

	void Storyboard::HoldVariable(const AnimationVariable& variable) const
	{
		auto hr = Get()->HoldVariable(variable.GetRaw()); ThrowFailed(hr);
	}

	auto Storyboard::AddKeyframeAfterTransition(const AnimationTransition& transition) -> KeyFrame
	{
		KeyFrame keyFrame;
		auto hr = Get()->AddKeyframeAfterTransition(transition.GetRaw(), &keyFrame); ThrowFailed(hr);
		
		return keyFrame;
	}
	auto Storyboard::AddKeyframeAtOffset(KeyFrame keyFrame, double durationOffset) -> KeyFrame
	{
		KeyFrame newKeyFrame;
		auto hr = Get()->AddKeyframeAtOffset(keyFrame, durationOffset, &newKeyFrame); ThrowFailed(hr);
		
		return newKeyFrame;
	}

	void Storyboard::AddTransition(const AnimationVariable& variable, AnimationTransition transition) const
	{
		auto hr = Get()->AddTransition(variable.GetRaw(), transition.GetRaw()); ThrowFailed(hr);
	}
	void Storyboard::AddTransitionAtKeyframe(const AnimationVariable& variable, 
		AnimationTransition transition, KeyFrame keyFrame) const
	{
		auto hr = Get()->AddTransitionAtKeyframe(variable.GetRaw(), 
			transition.GetRaw(), keyFrame); ThrowFailed(hr);
	}
	void Storyboard::AddTransitionBetweenKeyframes(const AnimationVariable& variable, AnimationTransition transition, 
		KeyFrame startKeyFrame, KeyFrame endKeyFrame) const
	{
		auto hr = Get()->AddTransitionBetweenKeyframes(variable.GetRaw(), 
			transition.GetRaw(), startKeyFrame, endKeyFrame); ThrowFailed(hr);
	}

	void Storyboard::RepeatBetweenKeyframes(KeyFrame startKeyframe, KeyFrame endKeyframe, 
		double iterationCount, AnimationRepeatMode repeatMode, 
		bool registerForNext) const
	{
		auto hr = Get()->RepeatBetweenKeyframes(startKeyframe, endKeyframe, 
			iterationCount, static_cast<UI_ANIMATION_REPEAT_MODE>(repeatMode), 
			nullptr, 0, registerForNext); ThrowFailed(hr);
	}

	auto Storyboard::GetStatus() const -> StoryboardStatus
	{
		UI_ANIMATION_STORYBOARD_STATUS status;
		auto hr = Get()->GetStatus(&status); ThrowFailed(hr);
		
		return static_cast<StoryboardStatus>(status);
	}

	auto Storyboard::GetElapsedTime() const -> double
	{
		double elapsedTime = 0.0;
		auto hr = Get()->GetElapsedTime(&elapsedTime); ThrowFailed(hr);
		
		return elapsedTime;
	}

	void Storyboard::SetTag(ComPtr<IUnknown> obj, UINT32 id) const
	{
		auto hr = Get()->SetTag(obj.Get(), id); ThrowFailed(hr);
	}
	auto Storyboard::GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		auto hr = Get()->GetTag(&obj, &tag); ThrowFailed(hr);

		return { obj, tag };
	}

	void Storyboard::SetStoryboardEventHandler(AnimationStoryboardEventHandler& eventHandler) const
	{
		auto hr = Get()->SetStoryboardEventHandler(
			static_cast<IUIAnimationStoryboardEventHandler2*>(
				&eventHandler.GetRouter()
				)); ThrowFailed(hr);
	}
}

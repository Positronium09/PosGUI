module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:Storyboard;
import :Storyboard;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationVariable;
import :AnimationTransition;
import :AnimationStoryboardEventHandler;

namespace PGUI::UI::Animation
{
	Storyboard::Storyboard(const ComPtr<IUIAnimationStoryboard2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{
	}

	auto Storyboard::Schedule(const double timeNow) const noexcept -> Result<AnimationSchedulingResult>
	{
		UI_ANIMATION_SCHEDULING_RESULT result;

		if (const auto hr = Get()->Schedule(timeNow, &result);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return static_cast<AnimationSchedulingResult>(result);
	}

	auto Storyboard::Abandon() const noexcept -> Error
	{
		return Error{
			Get()->Abandon()
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::Conclude() const noexcept -> Error
	{
		return Error{
			Get()->Conclude()
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::Finish(const double completionDeadline) const noexcept -> Error
	{
		return Error{
			Get()->Finish(completionDeadline)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::SetSkipDuration(const double duration) const noexcept -> Error
	{
		return Error{
			Get()->SetSkipDuration(duration)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::SetLongestAcceptableDelay(const double delay) const noexcept -> Error
	{
		return Error{
			Get()->SetLongestAcceptableDelay(delay)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::HoldVariable(const AnimationVariable& variable) const noexcept -> Error
	{
		return Error{
			Get()->HoldVariable(variable.GetRaw())
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::AddKeyframeAfterTransition(
		const AnimationTransition& transition) noexcept -> Result<KeyFrame>
	{
		KeyFrame keyFrame;
		if (const auto hr = Get()->AddKeyframeAfterTransition(transition.GetRaw(), &keyFrame);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return keyFrame;
	}

	auto Storyboard::AddKeyframeAtOffset(
		const KeyFrame keyFrame, const double durationOffset) noexcept -> Result<KeyFrame>
	{
		KeyFrame newKeyFrame;
		if (const auto hr = Get()->AddKeyframeAtOffset(keyFrame, durationOffset, &newKeyFrame);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return newKeyFrame;
	}

	auto Storyboard::AddTransition(
		const AnimationVariable& variable, AnimationTransition transition) const noexcept -> Error
	{
		return Error{
			Get()->AddTransition(variable.GetRaw(), transition.GetRaw())
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::AddTransitionAtKeyframe(
		const AnimationVariable& variable,
		AnimationTransition transition, const KeyFrame keyFrame) const noexcept -> Error
	{
		return Error{
			Get()->AddTransitionAtKeyframe(
				variable.GetRaw(),
				transition.GetRaw(), keyFrame)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::AddTransitionBetweenKeyframes(
		const AnimationVariable& variable, AnimationTransition transition,
		const KeyFrame startKeyFrame, const KeyFrame endKeyFrame) const noexcept -> Error
	{
		return Error{
			Get()->AddTransitionBetweenKeyframes(
			variable.GetRaw(),
			transition.GetRaw(), startKeyFrame, endKeyFrame)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::RepeatBetweenKeyframes(
		const KeyFrame startKeyframe, const KeyFrame endKeyframe,
		const double iterationCount, AnimationRepeatMode repeatMode,
		const bool registerForNext) const noexcept -> Error
	{
		return Error{
			Get()->RepeatBetweenKeyframes(
				startKeyframe, endKeyframe,
				iterationCount, static_cast<UI_ANIMATION_REPEAT_MODE>(repeatMode),
				nullptr, 0, registerForNext)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::GetStatus() const noexcept -> Result<StoryboardStatus>
	{
		UI_ANIMATION_STORYBOARD_STATUS status;

		if (const auto hr = Get()->GetStatus(&status);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return static_cast<StoryboardStatus>(status);
	}

	auto Storyboard::GetElapsedTime() const noexcept -> Result<double>
	{
		auto elapsedTime = 0.0;
		if (const auto hr = Get()->GetElapsedTime(&elapsedTime);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return elapsedTime;
	}

	auto Storyboard::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Error
	{
		return Error{
			Get()->SetTag(obj.Get(), id)
		}.AddTag(ErrorTags::Animation);
	}

	auto Storyboard::GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		if (const auto hr = Get()->GetTag(&obj, &tag);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return std::pair{ obj, tag };
	}

	auto Storyboard::SetStoryboardEventHandler(
		AnimationStoryboardEventHandler& eventHandler) const noexcept -> Error
	{
		return Error{
			Get()->SetStoryboardEventHandler(&eventHandler.GetRouter())
		}.AddTag(ErrorTags::Animation);
	}
}

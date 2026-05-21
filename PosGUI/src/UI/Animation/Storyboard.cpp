module;
#include <UIAnimation.h>

module PGUI.UI.Animation:Storyboard;
import :Storyboard;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationVariable;
import :AnimationTimeTypes;
import :AnimationTransition;
import :AnimationStoryboardEventHandler;

namespace PGUI::UI::Animation
{
	Storyboard::Storyboard(const ComPtr<IUIAnimationStoryboard2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{
	}

	auto Storyboard::Schedule(const Seconds timeNow) const noexcept -> Result<AnimationSchedulingResult>
	{
		UI_ANIMATION_SCHEDULING_RESULT result;

		if (const auto hr = Get()->Schedule(ToWAM(timeNow), &result);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Scheduling failed");
			return Unexpected{ error };
		}

		return static_cast<AnimationSchedulingResult>(result);
	}

	auto Storyboard::Abandon() const noexcept -> Result<void>
	{
		Error error{
			Get()->Abandon()
		};
		LogIfFailed(error, L"Abandoning failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::Conclude() const noexcept -> Result<void>
	{
		Error error{
			Get()->Conclude()
		};
		LogIfFailed(error, L"Concluding failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::Finish(const Seconds completionDeadline) const noexcept -> Result<void>
	{
		Error error{
			Get()->Finish(ToWAM(completionDeadline))
		};
		error
				.AddDetail(L"Completion Deadline", std::format(L"{:.10F}", completionDeadline.count()));
		LogIfFailed(error, L"Finish failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::SetSkipDuration(const Seconds duration) const noexcept -> Result<void>
	{
		Error error{
			Get()->SetSkipDuration(ToWAM(duration))
		};
		error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration.count()));
		LogIfFailed(error, L"Setting skip duration failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::SetLongestAcceptableDelay(const Seconds delay) const noexcept -> Result<void>
	{
		Error error{
			Get()->SetLongestAcceptableDelay(ToWAM(delay))
		};
		error
				.AddDetail(L"Delay", std::format(L"{:.10F}", delay.count()));
		LogIfFailed(error, L"Setting longest acceptable delay failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::HoldVariable(const AnimationVariable& variable) const noexcept -> Result<void>
	{
		Error error{
			Get()->HoldVariable(variable.GetRaw())
		};
		LogIfFailed(error, L"Hold variable failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::AddKeyframeAfterTransition(
		const AnimationTransition& transition) noexcept -> Result<KeyFrame>
	{
		KeyFrame keyFrame;
		if (const auto hr = Get()->AddKeyframeAfterTransition(transition.GetRaw(), &keyFrame);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"AddKeyframeAfterTransition failed");
			return Unexpected{ error };
		}

		return keyFrame;
	}

	auto Storyboard::AddKeyframeAtOffset(
		const KeyFrame keyFrame, const Seconds durationOffset) noexcept -> Result<KeyFrame>
	{
		KeyFrame newKeyFrame;
		if (const auto hr = Get()->AddKeyframeAtOffset(keyFrame, ToWAM(durationOffset), &newKeyFrame);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"AddKeyframeAtOffset failed");
			return Unexpected{ error };
		}

		return newKeyFrame;
	}

	auto Storyboard::AddTransition(
		const AnimationVariable& variable, const AnimationTransition& transition) const noexcept -> Result<void>
	{
		Error error{
			Get()->AddTransition(variable.GetRaw(), transition.GetRaw())
		};
		LogIfFailed(error, L"AddTransition failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::AddTransitionAtKeyframe(
		const AnimationVariable& variable,
		const AnimationTransition& transition, const KeyFrame keyFrame) const noexcept -> Result<void>
	{
		Error error{
			Get()->AddTransitionAtKeyframe(
				variable.GetRaw(),
				transition.GetRaw(), keyFrame)
		};
		LogIfFailed(error, L"AddTransitionAtKeyframe failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::AddTransitionBetweenKeyframes(
		const AnimationVariable& variable, const AnimationTransition& transition,
		const KeyFrame startKeyFrame, const KeyFrame endKeyFrame) const noexcept -> Result<void>
	{
		Error error{
			Get()->AddTransitionBetweenKeyframes(
			variable.GetRaw(),
			transition.GetRaw(), startKeyFrame, endKeyFrame)
		};
		LogIfFailed(error, L"AddTransitionBetweenKeyframes failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::RepeatBetweenKeyframes(
		const KeyFrame startKeyFrame, const KeyFrame endKeyFrame,
		const double iterationCount, AnimationRepeatMode repeatMode,
		const bool registerForNext) const noexcept -> Result<void>
	{
		Error error{
			Get()->RepeatBetweenKeyframes(
				startKeyFrame, endKeyFrame,
				iterationCount, static_cast<UI_ANIMATION_REPEAT_MODE>(repeatMode),
				nullptr, 0, registerForNext)
		};
		error
			.AddDetail(L"Iteration Count", std::format(L"{:.10F}", iterationCount))
			.AddDetail(L"Register for Next", std::format(L"{}", registerForNext));
		LogIfFailed(error, L"RepeatBetweenKeyframes failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::GetStatus() const noexcept -> Result<StoryboardStatus>
	{
		UI_ANIMATION_STORYBOARD_STATUS status;

		if (const auto hr = Get()->GetStatus(&status);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"GetStatus failed");
			return Unexpected{ error };
		}

		return static_cast<StoryboardStatus>(status);
	}

	auto Storyboard::GetElapsedTime() const noexcept -> Result<Seconds>
	{
		auto elapsedTime = 0.0;
		if (const auto hr = Get()->GetElapsedTime(&elapsedTime);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"GetElapsedTime failed");
			return Unexpected{ error };
		}

		return FromWAM(elapsedTime);
	}

	auto Storyboard::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Result<void>
	{
		Error error{
			Get()->SetTag(obj.get(), id)
		};
		error
			.AddDetail(L"ID", std::to_wstring(id));
		LogIfFailed(error, L"SetTag failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Storyboard::GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		if (const auto hr = Get()->GetTag(&obj, &tag);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"GetTag failed");
			return Unexpected{ error };
		}

		return std::pair{ obj, tag };
	}

	auto Storyboard::SetStoryboardEventHandler(
		AnimationStoryboardEventHandler& eventHandler) const noexcept -> Result<void>
	{
		Error error{
			Get()->SetStoryboardEventHandler(eventHandler.GetRouter().get())
		};
		LogIfFailed(error, L"SetStoryboardEventHandler failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}
}

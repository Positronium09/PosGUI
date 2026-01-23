module;
#include <UIAnimation.h>

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
			Error error{ hr };
			Logger::Error(error, L"Scheduling failed");
			return Unexpected{ error };
		}

		return static_cast<AnimationSchedulingResult>(result);
	}

	auto Storyboard::Abandon() const noexcept -> Error
	{
		Error error{
			Get()->Abandon()
		};
		LogIfFailed(error, L"Abandoning failed");
		return error;
	}

	auto Storyboard::Conclude() const noexcept -> Error
	{
		Error error{
			Get()->Conclude()
		};
		LogIfFailed(error, L"Concluding failed");
		return error;
	}

	auto Storyboard::Finish(const double completionDeadline) const noexcept -> Error
	{
		Error error{
			Get()->Finish(completionDeadline)
		};
		error
			.AddDetail(L"Completion Deadline", std::format(L"{:.10F}", completionDeadline));
		LogIfFailed(error, L"Finish failed");
		return error;
	}

	auto Storyboard::SetSkipDuration(const double duration) const noexcept -> Error
	{
		Error error{
			Get()->SetSkipDuration(duration)
		};
		error
			.AddDetail(L"Duration", std::format(L"{:.10F}", duration));
		LogIfFailed(error, L"Setting skip duration failed");
		return error;
	}

	auto Storyboard::SetLongestAcceptableDelay(const double delay) const noexcept -> Error
	{
		Error error{
			Get()->SetLongestAcceptableDelay(delay)
		};
		error
			.AddDetail(L"Delay", std::format(L"{:.10F}", delay));
		LogIfFailed(error, L"Setting longest acceptable delay failed");
		return error;
	}

	auto Storyboard::HoldVariable(const AnimationVariable& variable) const noexcept -> Error
	{
		Error error{
			Get()->HoldVariable(variable.GetRaw())
		};
		LogIfFailed(error, L"Hold variable failed");
		return error;
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
		const KeyFrame keyFrame, const double durationOffset) noexcept -> Result<KeyFrame>
	{
		KeyFrame newKeyFrame;
		if (const auto hr = Get()->AddKeyframeAtOffset(keyFrame, durationOffset, &newKeyFrame);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"AddKeyframeAtOffset failed");
			return Unexpected{ error };
		}

		return newKeyFrame;
	}

	auto Storyboard::AddTransition(
		const AnimationVariable& variable, AnimationTransition transition) const noexcept -> Error
	{
		Error error{
			Get()->AddTransition(variable.GetRaw(), transition.GetRaw())
		};
		LogIfFailed(error, L"AddTransition failed");
		return error;
	}

	auto Storyboard::AddTransitionAtKeyframe(
		const AnimationVariable& variable,
		AnimationTransition transition, const KeyFrame keyFrame) const noexcept -> Error
	{
		Error error{
			Get()->AddTransitionAtKeyframe(
				variable.GetRaw(),
				transition.GetRaw(), keyFrame)
		};
		LogIfFailed(error, L"AddTransitionAtKeyframe failed");
		return error;
	}

	auto Storyboard::AddTransitionBetweenKeyframes(
		const AnimationVariable& variable, AnimationTransition transition,
		const KeyFrame startKeyFrame, const KeyFrame endKeyFrame) const noexcept -> Error
	{
		Error error{
			Get()->AddTransitionBetweenKeyframes(
			variable.GetRaw(),
			transition.GetRaw(), startKeyFrame, endKeyFrame)
		};
		LogIfFailed(error, L"AddTransitionBetweenKeyframes failed");
		return error;
	}

	auto Storyboard::RepeatBetweenKeyframes(
		const KeyFrame startKeyFrame, const KeyFrame endKeyFrame,
		const double iterationCount, AnimationRepeatMode repeatMode,
		const bool registerForNext) const noexcept -> Error
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
		return error;
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

	auto Storyboard::GetElapsedTime() const noexcept -> Result<double>
	{
		auto elapsedTime = 0.0;
		if (const auto hr = Get()->GetElapsedTime(&elapsedTime);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"GetElapsedTime failed");
			return Unexpected{ error };
		}

		return elapsedTime;
	}

	auto Storyboard::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Error
	{
		Error error{
			Get()->SetTag(obj.get(), id)
		};
		error
			.AddDetail(L"ID", std::to_wstring(id));
		LogIfFailed(error, L"SetTag failed");
		return error;
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
		AnimationStoryboardEventHandler& eventHandler) const noexcept -> Error
	{
		Error error{
			Get()->SetStoryboardEventHandler(&eventHandler.GetRouter())
		};
		LogIfFailed(error, L"SetStoryboardEventHandler failed");
		return error;
	}
}

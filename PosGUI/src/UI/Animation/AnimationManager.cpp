module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationManager;
import :AnimationManager;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :Storyboard;
import :AnimationVariable;
import :AnimationTransition;
import :AnimationManagerEventHandler;

namespace PGUI::UI::Animation
{
	AnimationManager::AnimationManager()
	{
		if (const auto hr = CoCreateInstance(
			CLSID_UIAnimationManager2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationManager2),
			GetVoidAddress());
			FAILED(hr))
		{
			throw Exception{
				Error{ hr }
				.AddTag(ErrorTags::Initialization)
				.AddTag(ErrorTags::Animation),
				L"Cannot create animation manager"
			};
		}
	}

	auto AnimationManager::GetInstance() -> const AnimationManager&
	{
		if (instance == nullptr)
		{
			instance = new AnimationManager{ };
		}
		return *instance;
	}

	auto AnimationManager::AbandonAllStoryboards() noexcept -> Error
	{
		Error error{
			instance->Get()->AbandonAllStoryboards()
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"AbandonAllStoryboards failed {}", error);
		}

		return error;
	}

	auto AnimationManager::Pause() noexcept -> Error
	{
		Error error{
			instance->Get()->Pause()
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Pause failed {}", error);
		}

		return error;
	}

	auto AnimationManager::Resume() noexcept -> Error
	{
		Error error{
			instance->Get()->Resume()
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Resume failed {}", error);
		}

		return error;
	}

	auto AnimationManager::Shutdown() noexcept -> Error
	{
		Error error{
			instance->Get()->Shutdown()
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Shutdown failed {}", error);
		}

		return error;
	}

	auto AnimationManager::Update(const double timeNow) noexcept -> Result<AnimationUpdateResult>
	{
		UI_ANIMATION_UPDATE_RESULT result;
		if (const auto hr = instance->Get()->Update(timeNow, &result);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);

			Logger::Error(L"Update failed with Error: {}", error);
			return Unexpected{ error };
		}

		return static_cast<AnimationUpdateResult>(result);
	}

	auto AnimationManager::CreateAnimationVariable(const double initialValue) noexcept -> Result<AnimationVariable>
	{
		AnimationVariable variable;
		if (const auto hr = instance->Get()->CreateAnimationVariable(initialValue, variable.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);

			Logger::Error(L"CreateAnimationVariable failed with Error: {}", error);
			return Unexpected{ error };
		}

		return variable;
	}

	auto AnimationManager::CreateAnimationVariable(
		const std::span<const double> initialValues) noexcept -> Result<AnimationVariable>
	{
		AnimationVariable variable;

		if (const auto hr = instance->Get()->CreateAnimationVectorVariable(
			initialValues.data(),
			static_cast<UINT>(initialValues.size()),
			variable.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);

			Logger::Error(L"CreateAnimationVariable failed with Error: {}", error);
			return Unexpected{ error };
		}
		

		return variable;
	}

	auto AnimationManager::CreateStoryboard() noexcept -> Result<Storyboard>
	{
		Storyboard storyboard;

		if (const auto hr = instance->Get()->CreateStoryboard(storyboard.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(L"CreateStoryboard failed with Error: {}", error);
			return Unexpected{ error };
		}

		return storyboard;
	}

	auto AnimationManager::GetStatus() noexcept -> Result<AnimationManagerStatus>
	{
		UI_ANIMATION_MANAGER_STATUS status;
		if (const auto hr = instance->Get()->GetStatus(&status);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(L"GetStatus failed with Error: {}", error);
			return Unexpected{ error };
		}

		return static_cast<AnimationManagerStatus>(status);
	}

	auto AnimationManager::EstimateNextEventTime() noexcept -> Result<double>
	{
		double nextEventTime;
		if (const auto hr = instance->Get()->EstimateNextEventTime(&nextEventTime);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(L"EstimateNextEventTime failed with Error: {}", error);
			return Unexpected{ error };
		}

		return nextEventTime;
	}

	auto AnimationManager::SetAnimationMode(AnimationMode mode) noexcept -> Error
	{
		Error error{
			instance->Get()->SetAnimationMode(static_cast<UI_ANIMATION_MODE>(mode))
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetAnimationMode failed {}", error);
		}

		return error;
	}

	auto AnimationManager::SetDefaultLongestAcceptableDelay(const double delay) noexcept -> Error
	{
		Error error{
			instance->Get()->SetDefaultLongestAcceptableDelay(delay)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetDefaultLongestAcceptableDelay failed {}", error);
		}

		return error;
	}

	auto AnimationManager::GetStoryboardFromTag(
		const ComPtr<IUnknown>& obj, const UINT32 id) noexcept -> Result<Storyboard>
	{
		Storyboard storyboard;
		if (const auto hr = instance->Get()->GetStoryboardFromTag(obj.Get(), id, storyboard.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(L"GetStoryboardFromTag failed with Error: {}", error);
			return Unexpected{ error };
		}

		return storyboard;
	}

	auto AnimationManager::GetAnimationVariableFromTag(
		const ComPtr<IUnknown>& obj,
		const UINT32 id) noexcept -> Result<AnimationVariable>
	{
		AnimationVariable variable;
		if (const auto hr = instance->Get()->GetVariableFromTag(obj.Get(), id, variable.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(L"GetAnimationVariableFromTag failed with Error: {}", error);
			return Unexpected{ error };
		}

		return variable;
	}

	auto AnimationManager::ScheduleTransition(
		const AnimationVariable& variable,
		AnimationTransition transition, const double currentTime) noexcept -> Error
	{
		Error error{
			instance->Get()->ScheduleTransition(
				variable.GetRaw(),
				transition.GetRaw(),
				currentTime)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"ScheduleTransition failed {}", error);
		}

		return error;
	}

	auto AnimationManager::SetManagerEventHandler(
		AnimationManagerEventHandler& eventHandler, const bool registerForNext) const noexcept -> Error
	{
		Error error{
			Get()->SetManagerEventHandler(
			&eventHandler.GetRouter(),
			registerForNext)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetManagerEventHandler failed {}", error);
		}

		return error;
	}

	auto AnimationManager::ClearManagerEventHandler(const bool registerForNext) const noexcept -> Error
	{
		Error error{
			Get()->SetManagerEventHandler(nullptr, registerForNext)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetManagerEventHandler failed {}", error);
		}

		return error;
	}
}

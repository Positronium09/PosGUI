module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationManager;
import :AnimationManager;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :Storyboard;
import :AnimationTimeTypes;
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
				Error{ hr },
				L"Cannot create animation manager"
			};
		}
	}

	auto AnimationManager::GetGlobalInstance() -> const AnimationManager&
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
			Get()->AbandonAllStoryboards()
		};
		LogIfFailed(error, L"AbandonAllStoryboards failed");
		return error;
	}

	auto AnimationManager::Pause() const noexcept -> Error
	{
		Error error{
			Get()->Pause()
		};
		LogIfFailed(error, L"Pause failed");
		return error;
	}

	auto AnimationManager::Resume() const noexcept -> Error
	{
		Error error{
			Get()->Resume()
		};
		LogIfFailed(error, L"Resume failed");
		return error;
	}

	auto AnimationManager::Shutdown() const noexcept -> Error
	{
		Error error{
			Get()->Shutdown()
		};
		LogIfFailed(error, L"Shutdown failed");
		return error;
	}

	auto AnimationManager::Update(const double timeNow) const noexcept -> Result<AnimationUpdateResult>
	{
		UI_ANIMATION_UPDATE_RESULT result;
		if (const auto hr = Get()->Update(timeNow, &result);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"Update failed with Error: {}", error);
			return Unexpected{ error };
		}

		return static_cast<AnimationUpdateResult>(result);
	}

	auto AnimationManager::CreateAnimationVariable(
		const double initialValue) const noexcept -> Result<AnimationVariable>
	{
		AnimationVariable variable;
		if (const auto hr = Get()->CreateAnimationVariable(initialValue, variable.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"CreateAnimationVariable failed with Error: {}", error);
			return Unexpected{ error };
		}

		return variable;
	}

	auto AnimationManager::CreateAnimationVariable(
		const std::span<const double> initialValues) const noexcept -> Result<AnimationVariable>
	{
		AnimationVariable variable;

		if (const auto hr = Get()->CreateAnimationVectorVariable(
			initialValues.data(),
			static_cast<UINT>(initialValues.size()),
			variable.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"CreateAnimationVariable failed with Error: {}", error);
			return Unexpected{ error };
		}
		

		return variable;
	}

	auto AnimationManager::CreateStoryboard() const noexcept -> Result<Storyboard>
	{
		Storyboard storyboard;

		if (const auto hr = Get()->CreateStoryboard(storyboard.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"CreateStoryboard failed with Error: {}", error);
			return Unexpected{ error };
		}

		return storyboard;
	}

	auto AnimationManager::GetStatus() const noexcept -> Result<AnimationManagerStatus>
	{
		UI_ANIMATION_MANAGER_STATUS status;
		if (const auto hr = Get()->GetStatus(&status);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"GetStatus failed with Error: {}", error);
			return Unexpected{ error };
		}

		return static_cast<AnimationManagerStatus>(status);
	}

	auto AnimationManager::EstimateNextEventTime() const noexcept -> Result<double>
	{
		double nextEventTime;
		if (const auto hr = Get()->EstimateNextEventTime(&nextEventTime);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"EstimateNextEventTime failed with Error: {}", error);
			return Unexpected{ error };
		}

		return nextEventTime;
	}

	auto AnimationManager::SetAnimationMode(AnimationMode mode) const noexcept -> Error
	{
		Error error{
			Get()->SetAnimationMode(static_cast<UI_ANIMATION_MODE>(mode))
		};
		LogIfFailed(error, L"SetAnimationMode failed");
		return error;
	}

	auto AnimationManager::SetDefaultLongestAcceptableDelay(const Seconds delay) const noexcept -> Error
	{
		Error error{
			Get()->SetDefaultLongestAcceptableDelay(delay.count())
		};
		LogIfFailed(error, L"SetDefaultLongestAcceptableDelay failed");
		return error;
	}

	auto AnimationManager::GetStoryboardFromTag(
		const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Result<Storyboard>
	{
		Storyboard storyboard;
		if (const auto hr = Get()->GetStoryboardFromTag(obj.Get(), id, storyboard.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"GetStoryboardFromTag failed with Error: {}", error);
			return Unexpected{ error };
		}

		return storyboard;
	}

	auto AnimationManager::GetAnimationVariableFromTag(
		const ComPtr<IUnknown>& obj,
		const UINT32 id) const noexcept -> Result<AnimationVariable>
	{
		AnimationVariable variable;
		if (const auto hr = Get()->GetVariableFromTag(obj.Get(), id, variable.GetAddress());
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(L"GetAnimationVariableFromTag failed with Error: {}", error);
			return Unexpected{ error };
		}

		return variable;
	}

	auto AnimationManager::ScheduleTransition(
		const AnimationVariable& variable,
		AnimationTransition transition, const double currentTime) const noexcept -> Error
	{
		Error error{
			Get()->ScheduleTransition(
				variable.GetRaw(),
				transition.GetRaw(),
				currentTime)
		};
		LogIfFailed(error, L"ScheduleTransition failed");
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
		LogIfFailed(error, L"SetManagerEventHandler failed");
		return error;
	}

	auto AnimationManager::ClearManagerEventHandler(const bool registerForNext) const noexcept -> Error
	{
		Error error{
			Get()->SetManagerEventHandler(nullptr, registerForNext)
		};
		LogIfFailed(error, L"SetManagerEventHandler failed");
		return error;
	}
}

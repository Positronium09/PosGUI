module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationManager;
import :AnimationManager;

import PGUI.ComPtr;
import PGUI.Exceptions;
import :Storyboard;
import :AnimationVariable;
import :AnimationTransition;
import :AnimationManagerEventHandler;

namespace PGUI::UI::Animation
{
	AnimationManager::AnimationManager()
	{
		auto hr = CoCreateInstance(
			CLSID_UIAnimationManager2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationManager2),
			GetVoidAddress()); ThrowFailed(hr);
	}

	auto AnimationManager::GetInstance() -> const AnimationManager&
	{
		if (instance == nullptr)
		{
			instance = new AnimationManager{ };
		}
		return *instance;
	}

	void AnimationManager::AbandonAllStoryboards()
	{
		auto hr = instance->Get()->AbandonAllStoryboards(); ThrowFailed(hr);
	}

	void AnimationManager::Pause()
	{
		auto hr = instance->Get()->Pause(); ThrowFailed(hr);
	}
	void AnimationManager::Resume()
	{
		auto hr = instance->Get()->Resume(); ThrowFailed(hr);
	}
	void AnimationManager::Shutdown()
	{
		auto hr = instance->Get()->Shutdown(); ThrowFailed(hr);
	}

	auto AnimationManager::Update(double timeNow) -> AnimationUpdateResult
	{
		UI_ANIMATION_UPDATE_RESULT result;
		auto hr = instance->Get()->Update(timeNow, &result); ThrowFailed(hr);

		return static_cast<AnimationUpdateResult>(result);
	}

	auto AnimationManager::CreateAnimationVariable(double initialValue) -> AnimationVariable
	{
		AnimationVariable variable;
		auto hr = instance->Get()->CreateAnimationVariable(initialValue, variable.GetAddress()); ThrowFailed(hr);

		return variable;
	}
	auto AnimationManager::CreateAnimationVariable(std::span<const double> initialValues) -> AnimationVariable
	{
		AnimationVariable variable;
		auto hr = instance->Get()->CreateAnimationVectorVariable(
			initialValues.data(),
			static_cast<UINT>(initialValues.size()),
			variable.GetAddress()); ThrowFailed(hr);

		return variable;
	}

	auto AnimationManager::CreateStoryboard() -> Storyboard
	{
		Storyboard storyboard;
		auto hr = instance->Get()->CreateStoryboard(storyboard.GetAddress()); ThrowFailed(hr);

		return storyboard;
	}

	auto AnimationManager::GetStatus() -> AnimationManagerStatus
	{
		UI_ANIMATION_MANAGER_STATUS status;
		auto hr = instance->Get()->GetStatus(&status); ThrowFailed(hr);

		return static_cast<AnimationManagerStatus>(status);
	}

	auto AnimationManager::EstimateNextEventTime() -> double
	{
		double nextEventTime;
		auto hr = instance->Get()->EstimateNextEventTime(&nextEventTime); ThrowFailed(hr);

		return nextEventTime;
	}

	void AnimationManager::SetAnimationMode(AnimationMode mode)
	{
		auto hr = instance->Get()->SetAnimationMode(static_cast<UI_ANIMATION_MODE>(mode)); ThrowFailed(hr);
	}

	void AnimationManager::SetDefaultLongestAcceptableDelay(double delay)
	{
		auto hr = instance->Get()->SetDefaultLongestAcceptableDelay(delay); ThrowFailed(hr);
	}

	auto AnimationManager::GetStoryboardFromTag(ComPtr<IUnknown> obj, UINT32 id) -> Storyboard
	{
		Storyboard storyboard;
		auto hr = instance->Get()->GetStoryboardFromTag(obj.Get(), id, storyboard.GetAddress()); ThrowFailed(hr);

		return storyboard;
	}
	auto AnimationManager::GetAnimationVariableFromTag(ComPtr<IUnknown> obj, UINT32 id) -> AnimationVariable
	{
		AnimationVariable variable;
		auto hr = instance->Get()->GetVariableFromTag(obj.Get(), id, variable.GetAddress()); ThrowFailed(hr);

		return variable;
	}
	void AnimationManager::ScheduleTransition(const AnimationVariable& variable, 
		AnimationTransition transition, double currentTime)
	{
		auto hr = instance->Get()->ScheduleTransition(
			variable.GetRaw(),
			transition.GetRaw(),
			currentTime); ThrowFailed(hr);
	}

	void AnimationManager::SetManagerEventHandler(
		AnimationManagerEventHandler& eventHandler, bool registerForNext) const
	{
		auto hr = Get()->SetManagerEventHandler(
			static_cast<IUIAnimationManagerEventHandler2*>(
				&eventHandler.GetRouter()), 
			registerForNext); ThrowFailed(hr);
	}
	void AnimationManager::ClearManagerEventHandler(bool registerForNext) const
	{
		auto hr = Get()->SetManagerEventHandler(nullptr, registerForNext); ThrowFailed(hr);
	}
}

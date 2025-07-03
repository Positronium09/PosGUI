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
		const auto hr = CoCreateInstance(
			CLSID_UIAnimationManager2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationManager2),
			GetVoidAddress());
		ThrowFailed(hr);
	}

	auto AnimationManager::GetInstance() -> const AnimationManager&
	{
		if (instance == nullptr)
		{
			instance = new AnimationManager{ };
		}
		return *instance;
	}

	auto AnimationManager::AbandonAllStoryboards() -> void
	{
		const auto hr = instance->Get()->AbandonAllStoryboards();
		ThrowFailed(hr);
	}

	auto AnimationManager::Pause() -> void
	{
		const auto hr = instance->Get()->Pause();
		ThrowFailed(hr);
	}

	auto AnimationManager::Resume() -> void
	{
		const auto hr = instance->Get()->Resume();
		ThrowFailed(hr);
	}

	auto AnimationManager::Shutdown() -> void
	{
		const auto hr = instance->Get()->Shutdown();
		ThrowFailed(hr);
	}

	auto AnimationManager::Update(const double timeNow) -> AnimationUpdateResult
	{
		UI_ANIMATION_UPDATE_RESULT result;
		const auto hr = instance->Get()->Update(timeNow, &result);
		ThrowFailed(hr);

		return static_cast<AnimationUpdateResult>(result);
	}

	auto AnimationManager::CreateAnimationVariable(const double initialValue) -> AnimationVariable
	{
		AnimationVariable variable;
		const auto hr = instance->Get()->CreateAnimationVariable(initialValue, variable.GetAddress());
		ThrowFailed(hr);

		return variable;
	}

	auto AnimationManager::CreateAnimationVariable(const std::span<const double> initialValues) -> AnimationVariable
	{
		AnimationVariable variable;
		const auto hr = instance->Get()->CreateAnimationVectorVariable(
			initialValues.data(),
			static_cast<UINT>(initialValues.size()),
			variable.GetAddress());
		ThrowFailed(hr);

		return variable;
	}

	auto AnimationManager::CreateStoryboard() -> Storyboard
	{
		Storyboard storyboard;
		const auto hr = instance->Get()->CreateStoryboard(storyboard.GetAddress());
		ThrowFailed(hr);

		return storyboard;
	}

	auto AnimationManager::GetStatus() -> AnimationManagerStatus
	{
		UI_ANIMATION_MANAGER_STATUS status;
		const auto hr = instance->Get()->GetStatus(&status);
		ThrowFailed(hr);

		return static_cast<AnimationManagerStatus>(status);
	}

	auto AnimationManager::EstimateNextEventTime() -> double
	{
		double nextEventTime;
		const auto hr = instance->Get()->EstimateNextEventTime(&nextEventTime);
		ThrowFailed(hr);

		return nextEventTime;
	}

	auto AnimationManager::SetAnimationMode(AnimationMode mode) -> void
	{
		const auto hr = instance->Get()->SetAnimationMode(static_cast<UI_ANIMATION_MODE>(mode));
		ThrowFailed(hr);
	}

	auto AnimationManager::SetDefaultLongestAcceptableDelay(const double delay) -> void
	{
		const auto hr = instance->Get()->SetDefaultLongestAcceptableDelay(delay);
		ThrowFailed(hr);
	}

	auto AnimationManager::GetStoryboardFromTag(const ComPtr<IUnknown>& obj, const UINT32 id) -> Storyboard
	{
		Storyboard storyboard;
		const auto hr = instance->Get()->GetStoryboardFromTag(obj.Get(), id, storyboard.GetAddress());
		ThrowFailed(hr);

		return storyboard;
	}

	auto AnimationManager::GetAnimationVariableFromTag(
		const ComPtr<IUnknown>& obj,
		const UINT32 id) -> AnimationVariable
	{
		AnimationVariable variable;
		const auto hr = instance->Get()->GetVariableFromTag(obj.Get(), id, variable.GetAddress());
		ThrowFailed(hr);

		return variable;
	}

	auto AnimationManager::ScheduleTransition(
		const AnimationVariable& variable,
		AnimationTransition transition, const double currentTime) -> void
	{
		const auto hr = instance->Get()->ScheduleTransition(
			variable.GetRaw(),
			transition.GetRaw(),
			currentTime);
		ThrowFailed(hr);
	}

	auto AnimationManager::SetManagerEventHandler(
		AnimationManagerEventHandler& eventHandler, const bool registerForNext) const -> void
	{
		const auto hr = Get()->SetManagerEventHandler(
			&eventHandler.GetRouter(),
			registerForNext);
		ThrowFailed(hr);
	}

	auto AnimationManager::ClearManagerEventHandler(const bool registerForNext) const -> void
	{
		const auto hr = Get()->SetManagerEventHandler(nullptr, registerForNext);
		ThrowFailed(hr);
	}
}

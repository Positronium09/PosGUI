module;
#include <wrl.h>
#include <UIAnimation.h>

module PGUI.UI.Animation.AnimationManager;

import PGUI.ComPtr;
import PGUI.Exceptions;

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

	void AnimationManager::SetAnimationMode(AnimationMode mode)
	{
		auto hr = instance.Get()->SetAnimationMode(static_cast<UI_ANIMATION_MODE>(mode)); ThrowFailed(hr);
	}

	void AnimationManager::AbandonAllStoryboards()
	{
		auto hr = instance.Get()->AbandonAllStoryboards(); ThrowFailed(hr);
	}

	void AnimationManager::Pause()
	{
		auto hr = instance.Get()->Pause(); ThrowFailed(hr);
	}
	void AnimationManager::Resume()
	{
		auto hr = instance.Get()->Resume(); ThrowFailed(hr);
	}
	void AnimationManager::Shutdown()
	{
		auto hr = instance.Get()->Shutdown(); ThrowFailed(hr);
	}

	auto AnimationManager::GetStatus() -> AnimationManagerStatus
	{
		UI_ANIMATION_MANAGER_STATUS status;
		auto hr = instance.Get()->GetStatus(&status); ThrowFailed(hr);

		return static_cast<AnimationManagerStatus>(status);
	}
}

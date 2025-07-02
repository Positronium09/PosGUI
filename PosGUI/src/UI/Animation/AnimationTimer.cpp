module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTimer;
import :AnimationTimer;

import PGUI.ComPtr;
import PGUI.Exceptions;

namespace  PGUI::UI::Animation
{
	AnimationTimer::AnimationTimer()
	{
		auto hr = CoCreateInstance(
			CLSID_UIAnimationTimer,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationTimer),
			GetVoidAddress()); ThrowFailed(hr);
	}

	AnimationTimer::AnimationTimer(ComPtr<IUIAnimationTimer> ptr) noexcept : 
		ComPtrHolder{ ptr }
	{
	}

	void AnimationTimer::Enable() const
	{
		auto hr = Get()->Enable(); ThrowFailed(hr);
	}
	void AnimationTimer::Disable() const
	{
		auto hr = Get()->Disable(); ThrowFailed(hr);
	}
	auto AnimationTimer::IsEnabled() const -> bool
	{
		auto hr = Get()->IsEnabled();
		if (hr != S_OK || hr == S_FALSE)
		{
			ThrowFailed(hr);
		}

		return hr == S_OK;
	}

	auto AnimationTimer::GetTime() const -> double
	{
		double time{ };
		auto hr = Get()->GetTime(&time); ThrowFailed(hr);

		return time;
	}

	void AnimationTimer::SetFrameRateThreshold(UINT32 threshold) const
	{
		auto hr = Get()->SetFrameRateThreshold(threshold); ThrowFailed(hr);
	}
}

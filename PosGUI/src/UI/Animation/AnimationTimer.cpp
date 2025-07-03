module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTimer;
import :AnimationTimer;

import PGUI.ComPtr;
import PGUI.Exceptions;

namespace PGUI::UI::Animation
{
	AnimationTimer::AnimationTimer()
	{
		const auto hr = CoCreateInstance(
			CLSID_UIAnimationTimer,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationTimer),
			GetVoidAddress());
		ThrowFailed(hr);
	}

	AnimationTimer::AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{ }

	auto AnimationTimer::Enable() const -> void
	{
		const auto hr = Get()->Enable();
		ThrowFailed(hr);
	}

	auto AnimationTimer::Disable() const -> void
	{
		const auto hr = Get()->Disable();
		ThrowFailed(hr);
	}

	auto AnimationTimer::IsEnabled() const -> bool
	{
		const auto hr = Get()->IsEnabled();
		if (hr != S_OK || hr == S_FALSE)
		{
			ThrowFailed(hr);
		}

		return hr == S_OK;
	}

	auto AnimationTimer::GetTime() const -> double
	{
		double time{ };
		const auto hr = Get()->GetTime(&time);
		ThrowFailed(hr);

		return time;
	}

	auto AnimationTimer::SetFrameRateThreshold(const UINT32 threshold) const -> void
	{
		const auto hr = Get()->SetFrameRateThreshold(threshold);
		ThrowFailed(hr);
	}
}

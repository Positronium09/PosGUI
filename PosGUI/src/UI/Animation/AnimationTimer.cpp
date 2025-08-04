module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTimer;
import :AnimationTimer;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::UI::Animation
{
	AnimationTimer::AnimationTimer()
	{
		if (const auto hr = CoCreateInstance(
			CLSID_UIAnimationTimer,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationTimer),
			GetVoidAddress());
			FAILED(hr))
		{
			throw Exception{
				Error{ hr }
				.AddTag(ErrorTags::Initialization)
				.AddTag(ErrorTags::Animation),
				L"Cannot create animation timer"
			};
		}
	}

	AnimationTimer::AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{ }

	auto AnimationTimer::Enable() const noexcept -> Error
	{
		Error error{
			Get()->Enable()
		};
		error.AddTag(ErrorTags::Animation);
		LogIfFailed(error, L"Enable failed");
		return error;
	}

	auto AnimationTimer::Disable() const noexcept -> Error
	{
		Error error{
			Get()->Disable()
		};
		error.AddTag(ErrorTags::Animation);
		LogIfFailed(error, L"Disable failed");
		return error;
	}

	auto AnimationTimer::IsEnabled() const noexcept -> Result<bool>
	{
		const auto hr = Get()->IsEnabled();
		if (hr != S_OK || hr == S_FALSE)
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(error, L"Failed to check if animation timer is enabled");
			return Unexpected{ error };
		}

		return hr == S_OK;
	}

	auto AnimationTimer::GetTime() const noexcept -> Result<double>
	{
		double time{ };
		if (const auto hr = Get()->GetTime(&time);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Animation);
			Logger::Error(error, L"Failed to get animation timer time");
			return Unexpected{ error };
		}

		return time;
	}

	auto AnimationTimer::SetFrameRateThreshold(const UINT32 threshold) const noexcept -> Error
	{
		Error error{
			Get()->SetFrameRateThreshold(threshold)
		};
		error.AddTag(ErrorTags::Animation);
		LogIfFailed(error, L"Set frame rate threshold failed");
		return error;
	}
}

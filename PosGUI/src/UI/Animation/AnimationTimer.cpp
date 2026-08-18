module;
#include <UIAnimation.h>

module PGUI.UI.Animation:AnimationTimer;
import :AnimationTimer;

import :AnimationTimeTypes;
import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::UI::Animation
{
	AnimationTimer::AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{ }

	auto AnimationTimer::Create() noexcept -> Result<AnimationTimer>
	{
		ComPtr<IUIAnimationTimer> timer;
		if (Error error{
				CoCreateInstance(
					CLSID_UIAnimationTimer,
					nullptr,
					CLSCTX_INPROC_SERVER,
					GetIID(timer),
					timer.put_void())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return AnimationTimer{ timer };
	}

	auto AnimationTimer::GetGlobalInstance() noexcept -> Result<std::reference_wrapper<const AnimationTimer>>
	{
		static const auto instance = Create();
		if (!instance.has_value())
		{
			return Unexpected{ instance.error() };
		}
		return std::cref(instance.value());
	}

	auto AnimationTimer::Enable() const noexcept -> Result<void>
	{
		Error error{
			Get()->Enable()
		};
		LogIfFailed(error, L"Enable failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto AnimationTimer::Disable() const noexcept -> Result<void>
	{
		Error error{
			Get()->Disable()
		};
		LogIfFailed(error, L"Disable failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto AnimationTimer::IsEnabled() const noexcept -> Result<bool>
	{
		const auto hr = Get()->IsEnabled();
		if (FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to check if animation timer is enabled");
			return Unexpected{ error };
		}

		return hr == S_OK;
	}

	auto AnimationTimer::GetTime() const noexcept -> Result<Seconds>
	{
		double time{ };
		if (const auto hr = Get()->GetTime(&time);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get animation timer time");
			return Unexpected{ error };
		}

		return FromWAM(time);
	}

	auto AnimationTimer::SetFrameRateThreshold(const UINT32 threshold) const noexcept -> Result<void>
	{
		Error error{
			Get()->SetFrameRateThreshold(threshold)
		};
		LogIfFailed(error, L"Set frame rate threshold failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto AnimationTimer::SetTimerEventHandler(AnimationTimerEventHandler& handler) const noexcept -> Result<void>
	{
		Error error{
			Get()->SetTimerEventHandler(handler.GetRouter().get())
		};
		LogIfFailed(error, L"SetTimerEventHandler failed");
		if (error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}
}

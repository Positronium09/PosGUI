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

		if (error.IsFailure())
		{
			Logger::Error(L"Enable failed {}", error);
		}

		return error;
	}

	auto AnimationTimer::Disable() const noexcept -> Error
	{
		Error error{
			Get()->Disable()
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Disable failed {}", error);
		}

		return error;
	}

	auto AnimationTimer::IsEnabled() const noexcept -> Result<bool>
	{
		const auto hr = Get()->IsEnabled();
		if (hr != S_OK || hr == S_FALSE)
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return hr == S_OK;
	}

	auto AnimationTimer::GetTime() const noexcept -> Result<double>
	{
		double time{ };
		if (const auto hr = Get()->GetTime(&time);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return time;
	}

	auto AnimationTimer::SetFrameRateThreshold(const UINT32 threshold) const noexcept -> Error
	{
		Error error{
			Get()->SetFrameRateThreshold(threshold)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetFrameRateThreshold failed {}", error);
		}

		return error;
	}
}

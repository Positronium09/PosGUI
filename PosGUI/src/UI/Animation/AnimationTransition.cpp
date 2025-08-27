module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTransition;
import :AnimationTransition;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::UI::Animation
{
	AnimationTransition::AnimationTransition(const ComPtr<IUIAnimationTransition2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{
	}

	auto AnimationTransition::GetDimension() const noexcept -> Result<UINT>
	{
		UINT dimension;
		if (const auto hr = Get()->GetDimension(&dimension);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get dimension of animation transition");
			return Unexpected{ error };
		}

		return dimension;
	}

	auto AnimationTransition::GetDuration() const noexcept -> Result<double>
	{
		double duration;
		if (const auto hr = Get()->GetDuration(&duration);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get duration of animation transition");
			return Unexpected{ error };
		}

		return duration;
	}

	auto AnimationTransition::IsDurationKnown() const noexcept -> bool
	{
		return Get()->IsDurationKnown() == S_OK;
	}

	auto AnimationTransition::SetInitialValue(const double value) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialValue(value)
		};
		LogIfFailed(error, L"Failed to set initial value");
		return error;
	}

	auto AnimationTransition::SetInitialValue(const std::span<const double> values) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialVectorValue(
				values.data(),
				static_cast<UINT>(values.size()))
		};
		LogIfFailed(error, L"Failed to set initial vector value");
		return error;
	}

	auto AnimationTransition::SetInitialVelocity(const double velocity) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialVelocity(velocity)
		};
		LogIfFailed(error, L"Failed to set initial velocity");
		return error;
	}

	auto AnimationTransition::SetInitialVelocity(const std::span<const double> velocities) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialVectorVelocity(
				velocities.data(),
				static_cast<UINT>(velocities.size()))
		};
		LogIfFailed(error, L"Failed to set initial vector velocity");
		return error;
	}
}

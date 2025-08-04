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
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return dimension;
	}

	auto AnimationTransition::GetDuration() const noexcept -> Result<double>
	{
		double duration;
		if (const auto hr = Get()->GetDuration(&duration);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetInitialValue failed {}", error);
		}

		return error;
	}

	auto AnimationTransition::SetInitialValue(const std::span<const double> values) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialVectorValue(
				values.data(),
				static_cast<UINT>(values.size()))
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetInitialValue failed {}", error);
		}

		return error;
	}

	auto AnimationTransition::SetInitialVelocity(const double velocity) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialVelocity(velocity)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetInitialVelocity failed {}", error);
		}

		return error;
	}

	auto AnimationTransition::SetInitialVelocity(const std::span<const double> velocities) const noexcept -> Error
	{
		Error error{
			Get()->SetInitialVectorVelocity(
				velocities.data(),
				static_cast<UINT>(velocities.size()))
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"SetInitialVelocity failed {}", error);
		}

		return error;
	}
}

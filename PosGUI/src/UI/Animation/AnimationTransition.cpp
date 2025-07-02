module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTransition;
import :AnimationTransition;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;

namespace  PGUI::UI::Animation
{
	AnimationTransition::AnimationTransition(ComPtr<IUIAnimationTransition2> ptr) noexcept : 
		ComPtrHolder{ ptr }
	{
	}

	auto AnimationTransition::GetDimension() const -> UINT
	{
		UINT dimension;
		auto hr = Get()->GetDimension(&dimension); ThrowFailed(hr);

		return dimension;
	}
	auto AnimationTransition::GetDuration() const -> double
	{
		double duration;
		auto hr = Get()->GetDuration(&duration); ThrowFailed(hr);

		return duration;
	}
	auto AnimationTransition::IsDurationKnown() const noexcept -> bool
	{
		return Get()->IsDurationKnown() == S_OK;
	}

	void AnimationTransition::SetInitialValue(double value) const
	{
		auto hr = Get()->SetInitialValue(value); ThrowFailed(hr);
	}
	void AnimationTransition::SetInitialValue(std::span<const double> values) const
	{
		auto hr = Get()->SetInitialVectorValue(values.data(), 
			static_cast<UINT>(values.size())); ThrowFailed(hr);
	}
	void AnimationTransition::SetInitialVelocity(double velocity) const
	{
		auto hr = Get()->SetInitialVelocity(velocity); ThrowFailed(hr);
	}
	void AnimationTransition::SetInitialVelocity(std::span<const double> velocities) const
	{
		auto hr = Get()->SetInitialVectorVelocity(velocities.data(),
			static_cast<UINT>(velocities.size())); ThrowFailed(hr);
	}
}

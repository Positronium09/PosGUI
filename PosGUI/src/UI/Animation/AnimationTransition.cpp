module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTransition;
import :AnimationTransition;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;

namespace PGUI::UI::Animation
{
	AnimationTransition::AnimationTransition(const ComPtr<IUIAnimationTransition2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{
	}

	auto AnimationTransition::GetDimension() const -> UINT
	{
		UINT dimension;
		const auto hr = Get()->GetDimension(&dimension);
		ThrowFailed(hr);

		return dimension;
	}

	auto AnimationTransition::GetDuration() const -> double
	{
		double duration;
		const auto hr = Get()->GetDuration(&duration);
		ThrowFailed(hr);

		return duration;
	}

	auto AnimationTransition::IsDurationKnown() const noexcept -> bool
	{
		return Get()->IsDurationKnown() == S_OK;
	}

	auto AnimationTransition::SetInitialValue(const double value) const -> void
	{
		const auto hr = Get()->SetInitialValue(value);
		ThrowFailed(hr);
	}

	auto AnimationTransition::SetInitialValue(const std::span<const double> values) const -> void
	{
		const auto hr = Get()->SetInitialVectorValue(
			values.data(),
			static_cast<UINT>(values.size()));
		ThrowFailed(hr);
	}

	auto AnimationTransition::SetInitialVelocity(const double velocity) const -> void
	{
		const auto hr = Get()->SetInitialVelocity(velocity);
		ThrowFailed(hr);
	}

	auto AnimationTransition::SetInitialVelocity(const std::span<const double> velocities) const -> void
	{
		const auto hr = Get()->SetInitialVectorVelocity(
			velocities.data(),
			static_cast<UINT>(velocities.size()));
		ThrowFailed(hr);
	}
}

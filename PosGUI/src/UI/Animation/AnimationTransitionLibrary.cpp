module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationTransitionLibrary;
import :AnimationTransitionLibrary;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Exceptions;
import :AnimationTransition;

namespace PGUI::UI::Animation
{
	AnimationTransitionLibrary::AnimationTransitionLibrary()
	{
		const auto hr = CoCreateInstance(
			CLSID_UIAnimationTransitionLibrary2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationTransitionLibrary2),
			GetVoidAddress());
		ThrowFailed(hr);
	}

	auto AnimationTransitionLibrary::GetInstance() -> const AnimationTransitionLibrary&
	{
		if (instance == nullptr)
		{
			instance = new AnimationTransitionLibrary{ };
		}
		return *instance;
	}

	auto AnimationTransitionLibrary::AccelerateDecelerateTransition(
		const double duration, const double finalValue,
		const double accelerationRatio, const double decelerationRatio) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateAccelerateDecelerateTransition(
			duration, finalValue,
			accelerationRatio, decelerationRatio,
			transition.GetAddress()
		);
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::ConstantTransition(const double duration) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateConstantTransition(
			duration,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::CubicBezierLinearTransition(
		const double duration, const double finalValue,
		const Point<double> p1, const Point<double> p2) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateCubicBezierLinearTransition(
			duration, finalValue,
			p1.x, p1.y, p2.x, p2.y,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::CubicBezierLinearTransition(
		const double duration,
		const std::span<const double> finalValues,
		const Point<double> p1, const Point<double> p2) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateCubicBezierLinearVectorTransition(
			duration, finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			p1.x, p1.y, p2.x, p2.y,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::CubicTransition(
		const double duration,
		const double finalValue,
		const double finalVelocity) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateCubicTransition(
			duration,
			finalValue, finalVelocity,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::CubicTransition(
		const double duration,
		const std::span<const double> finalValues, const std::span<const double> finalVelocities) -> AnimationTransition
	{
		if (finalValues.size() != finalVelocities.size())
		{
			throw std::invalid_argument{ "finalValues and finalVelocities must have the same number of elements" };
		}

		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateCubicVectorTransition(
			duration,
			finalValues.data(), finalVelocities.data(),
			static_cast<UINT>(finalValues.size()),
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::DiscreteTransition(
		const double duration,
		const double finalValue, const double hold) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateDiscreteTransition(
			duration,
			finalValue, hold,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::DiscreteTransition(
		const double duration,
		const std::span<const double> finalValues, const double hold) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateDiscreteVectorTransition(
			duration,
			finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			hold,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::InstantaneousTransition(const double finalValue) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateInstantaneousTransition(
			finalValue,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::InstantaneousTransition(
		const std::span<const double> finalValues) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateInstantaneousVectorTransition(
			finalValues.data(), static_cast<UINT>(finalValues.size()),
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransition(
		const double duration, const double finalValue) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateLinearTransition(
			duration,
			finalValue,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransition(
		const double duration,
		const std::span<const double> finalValues) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateLinearVectorTransition(
			duration,
			finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransitionFromSpeed(
		const double speed,
		const double finalValue) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateLinearTransitionFromSpeed(
			speed,
			finalValue,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransitionFromSpeed(
		const double speed,
		const std::span<const double> finalValues) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateLinearVectorTransitionFromSpeed(
			speed,
			finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::ParabolicTransitionFromAcceleration(
		const double finalValue,
		const double finalVelocity, const double acceleration) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateParabolicTransitionFromAcceleration(
			finalValue,
			finalVelocity,
			acceleration,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::ReversalTransition(const double duration) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateReversalTransition(
			duration,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::SinusoidalTransitionFromRange(
		const double duration, const double minimumValue, const double maximumValue,
		const double period, AnimationSlope slope) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateSinusoidalTransitionFromRange(
			duration, minimumValue, maximumValue,
			period, static_cast<UI_ANIMATION_SLOPE>(slope),
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::SinusoidalTransitionFromVelocity(
		const double duration, const double period) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateSinusoidalTransitionFromVelocity(
			duration, period,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}

	auto AnimationTransitionLibrary::SmoothStopTransition(
		const double maximumDuration, const double finalValue) -> AnimationTransition
	{
		AnimationTransition transition{ };

		const auto hr = instance->Get()->CreateSmoothStopTransition(
			maximumDuration, finalValue,
			transition.GetAddress());
		ThrowFailed(hr);

		return transition;
	}
}

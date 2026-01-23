module;
#include <UIAnimation.h>

module PGUI.UI.Animation:AnimationTransitionLibrary;
import :AnimationTransitionLibrary;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import :AnimationTransition;

namespace PGUI::UI::Animation
{
	AnimationTransitionLibrary::AnimationTransitionLibrary()
	{
		if (const auto hr = CoCreateInstance(
			CLSID_UIAnimationTransitionLibrary2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IUIAnimationTransitionLibrary2),
			PutVoid());
			FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create animation transition library"
			};
		}
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
		const double accelerationRatio, const double decelerationRatio) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateAccelerateDecelerateTransition(
			duration, finalValue,
			accelerationRatio, decelerationRatio,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue))
				.AddDetail(L"Acceleration Ratio", std::format(L"{:.10F}", accelerationRatio))
				.AddDetail(L"Deceleration Ratio", std::format(L"{:.10F}", decelerationRatio));
			Logger::Error(error, L"Failed to create AccelerateDecelerateTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::ConstantTransition(const double duration) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateConstantTransition(
			duration,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration));
			Logger::Error(error, L"Failed to create ConstantTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::CubicBezierLinearTransition(
		const double duration, const double finalValue,
		const Point<double> p1, const Point<double> p2) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateCubicBezierLinearTransition(
			duration, finalValue,
			p1.x, p1.y, p2.x, p2.y,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue))
				.AddDetail(L"P1", std::format(L"{}", p1))
				.AddDetail(L"P2", std::format(L"{}", p2));
			Logger::Error(error, L"Failed to create CubicBezierLinearTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::CubicBezierLinearTransition(
		const double duration,
		const std::span<const double> finalValues,
		const Point<double> p1, const Point<double> p2) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateCubicBezierLinearVectorTransition(
			duration, finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			p1.x, p1.y, p2.x, p2.y,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Values", std::format(L"{}", finalValues))
				.AddDetail(L"P1", std::format(L"{}", p1))
				.AddDetail(L"P2", std::format(L"{}", p2));
			Logger::Error(error, L"Failed to create CubicBezierLinearVectorTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::CubicTransition(
		const double duration,
		const double finalValue,
		const double finalVelocity) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateCubicTransition(
			duration,
			finalValue, finalVelocity,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue))
				.AddDetail(L"Final Velocity", std::format(L"{:.10F}", finalVelocity));
			Logger::Error(error, L"Failed to create CubicTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::CubicTransition(
		const double duration,
		const std::span<const double> finalValues,
		const std::span<const double> finalVelocities) noexcept -> Result<AnimationTransition>
	{
		if (finalValues.size() != finalVelocities.size())
		{
			Error error{ std::errc::invalid_argument };
			error
				.SuggestFix(L"finalValues and finalVelocities must have the same number of elements");
			Logger::Error(error, L"Failed to create CubicTransition: finalValues and finalVelocities size mismatch");
			return Unexpected{ error };
		}

		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateCubicVectorTransition(
			duration,
			finalValues.data(), finalVelocities.data(),
			static_cast<UINT>(finalValues.size()),
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Values", std::format(L"{}", finalValues))
				.AddDetail(L"Final Velocities", std::format(L"{}", finalVelocities));
			Logger::Error(error, L"Failed to create CubicVectorTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::DiscreteTransition(
		const double duration,
		const double finalValue, const double hold) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateDiscreteTransition(
			duration,
			finalValue, hold,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue))
				.AddDetail(L"Hold", std::format(L"{:.10F}", hold));
			Logger::Error(error, L"Failed to create DiscreteTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::DiscreteTransition(
		const double duration,
		const std::span<const double> finalValues, const double hold) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateDiscreteVectorTransition(
			duration,
			finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			hold,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Values", std::format(L"{}", finalValues))
				.AddDetail(L"Hold", std::format(L"{:.10F}", hold));
			Logger::Error(error, L"Failed to create DiscreteVectorTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::InstantaneousTransition(
		const double finalValue) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateInstantaneousTransition(
			finalValue,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue));
			Logger::Error(error, L"Failed to create InstantaneousTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::InstantaneousTransition(
		const std::span<const double> finalValues) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateInstantaneousVectorTransition(
			finalValues.data(), static_cast<UINT>(finalValues.size()),
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Final Values", std::format(L"{}", finalValues));
			Logger::Error(error, L"Failed to create InstantaneousVectorTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransition(
		const double duration, const double finalValue) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateLinearTransition(
			duration,
			finalValue,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue));
			Logger::Error(error, L"Failed to create LinearTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransition(
		const double duration,
		const std::span<const double> finalValues) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateLinearVectorTransition(
			duration,
			finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Final Values", std::format(L"{}", finalValues));
			Logger::Error(error, L"Failed to create LinearVectorTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransitionFromSpeed(
		const double speed,
		const double finalValue) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateLinearTransitionFromSpeed(
			speed,
			finalValue,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Speed", std::format(L"{:.10F}", speed))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue));
			Logger::Error(error, L"Failed to create LinearTransitionFromSpeed");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::LinearTransitionFromSpeed(
		const double speed,
		const std::span<const double> finalValues) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateLinearVectorTransitionFromSpeed(
			speed,
			finalValues.data(),
			static_cast<UINT>(finalValues.size()),
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Speed", std::format(L"{:.10F}", speed))
				.AddDetail(L"Final Values", std::format(L"{}", finalValues));
			Logger::Error(error, L"Failed to create LinearVectorTransitionFromSpeed");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::ParabolicTransitionFromAcceleration(
		const double finalValue,
		const double finalVelocity, const double acceleration) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateParabolicTransitionFromAcceleration(
			finalValue,
			finalVelocity,
			acceleration,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue))
				.AddDetail(L"Final Velocity", std::format(L"{:.10F}", finalVelocity))
				.AddDetail(L"Acceleration", std::format(L"{:.10F}", acceleration));
			Logger::Error(error, L"Failed to create ParabolicTransitionFromAcceleration");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::ReversalTransition(const double duration) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateReversalTransition(
			duration,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration));
			Logger::Error(error, L"Failed to create ReversalTransition");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::SinusoidalTransitionFromRange(
		const double duration, const double minimumValue, const double maximumValue,
		const double period, AnimationSlope slope) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateSinusoidalTransitionFromRange(
			duration, minimumValue, maximumValue,
			period, static_cast<UI_ANIMATION_SLOPE>(slope),
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Minimum Value", std::format(L"{:.10F}", minimumValue))
				.AddDetail(L"Maximum Value", std::format(L"{:.10F}", maximumValue))
				.AddDetail(L"Period", std::format(L"{:.10F}", period));
			Logger::Error(error, L"Failed to create SinusoidalTransitionFromRange");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::SinusoidalTransitionFromVelocity(
		const double duration, const double period) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateSinusoidalTransitionFromVelocity(
			duration, period,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Duration", std::format(L"{:.10F}", duration))
				.AddDetail(L"Period", std::format(L"{:.10F}", period));
			Logger::Error(error, L"Failed to create SinusoidalTransitionFromVelocity");
			return Unexpected{ error };
		}

		return transition;
	}

	auto AnimationTransitionLibrary::SmoothStopTransition(
		const double maximumDuration, const double finalValue) noexcept -> Result<AnimationTransition>
	{
		AnimationTransition transition{ };

		if (const auto hr = instance->Get()->CreateSmoothStopTransition(
			maximumDuration, finalValue,
			transition.Put());
			FAILED(hr))
		{
			Error error{ hr };
			error
				.AddDetail(L"Maximum Duration", std::format(L"{:.10F}", maximumDuration))
				.AddDetail(L"Final Value", std::format(L"{:.10F}", finalValue));
			Logger::Error(error, L"Failed to create SmoothStopTransition");
			return Unexpected{ error };
		}

		return transition;
	}
}

module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTransitionLibrary;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import :AnimationTimeTypes;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	class AnimationTransitionLibrary : public ComPtrHolder<IUIAnimationTransitionLibrary2>
	{
		public:
		[[nodiscard]] static auto GetInstance() -> const AnimationTransitionLibrary&;

		[[nodiscard]] static auto AccelerateDecelerateTransition(
			Seconds duration, double finalValue,
			double accelerationRatio, double decelerationRatio) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ConstantTransition(Seconds duration) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			Seconds duration, double finalValue,
			Point<double> p1, Point<double> p2) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			Seconds duration, std::span<const double> finalValues,
			Point<double> p1, Point<double> p2) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			Seconds duration, double finalValue,
			double finalVelocity) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			Seconds duration, std::span<const double> finalValues,
			std::span<const double> finalVelocities) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			Seconds duration, double finalValue,
			double hold) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			Seconds duration, std::span<const double> finalValues,
			double hold) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			Seconds duration,
			double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			Seconds duration,
			std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransitionFromSpeed(
			double speed,
			double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransitionFromSpeed(
			double speed,
			std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ParabolicTransitionFromAcceleration(
			double finalValue, double finalVelocity, double acceleration
		) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ReversalTransition(Seconds duration) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromRange(
			Seconds duration,
			double minimumValue, double maximumValue,
			double period, AnimationSlope slope) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromVelocity(
			Seconds duration, double period
		) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SmoothStopTransition(
			Seconds maximumDuration,
			double finalValue) noexcept -> Result<AnimationTransition>;

		private:
		AnimationTransitionLibrary();

		inline static AnimationTransitionLibrary* instance = nullptr;
	};
}

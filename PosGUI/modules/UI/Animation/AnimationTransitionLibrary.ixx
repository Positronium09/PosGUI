module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTransitionLibrary;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	class AnimationTransitionLibrary : public ComPtrHolder<IUIAnimationTransitionLibrary2>
	{
		public:
		[[nodiscard]] static auto GetInstance() -> const AnimationTransitionLibrary&;

		[[nodiscard]] static auto AccelerateDecelerateTransition(
			double duration, double finalValue,
			double accelerationRatio, double decelerationRatio) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ConstantTransition(double duration) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			double duration, double finalValue,
			Point<double> p1, Point<double> p2) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			double duration, std::span<const double> finalValues,
			Point<double> p1, Point<double> p2) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			double duration, double finalValue,
			double finalVelocity) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			double duration, std::span<const double> finalValues,
			std::span<const double> finalVelocities) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			double duration, double finalValue,
			double hold) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			double duration, std::span<const double> finalValues,
			double hold) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			double duration,
			double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			double duration,
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

		[[nodiscard]] static auto ReversalTransition(double duration) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromRange(
			double duration,
			double minimumValue, double maximumValue,
			double period, AnimationSlope slope) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromVelocity(
			double duration, double period
		) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SmoothStopTransition(
			double maximumDuration,
			double finalValue) noexcept -> Result<AnimationTransition>;

		private:
		AnimationTransitionLibrary();

		inline static AnimationTransitionLibrary* instance = nullptr;
	};
}

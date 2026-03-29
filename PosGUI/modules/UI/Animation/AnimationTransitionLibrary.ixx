module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTransitionLibrary;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import :AnimationTimeTypes;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	using ValueOrValues = std::variant<double, std::span<const double>>;

	struct AccelerateDecelerateTransitionParameters
	{
		Seconds duration;
		double finalValue;
		double accelerationRatio;
		double decelerationRatio;
	};

	struct ConstantTransitionParameters
	{
		Seconds duration;
	};

	struct CubicBezierLinearTransitionParameters
	{
		Seconds duration;
		ValueOrValues finalValue;
		Point<double> p1;
		Point<double> p2;
	};

	struct CubicTransitionParameters
	{
		Seconds duration;
		ValueOrValues finalValue;
		ValueOrValues finalVelocity;
	};

	struct DiscreteTransitionParameters
	{
		Seconds duration;
		ValueOrValues finalValue;
		double hold;
	};

	struct InstantaneousTransitionParameters
	{
		ValueOrValues finalValue;
	};

	struct LinearTransitionParameters
	{
		Seconds duration;
		ValueOrValues finalValue;
	};

	struct LinearTransitionFromSpeedParameters
	{
		double speed;
		ValueOrValues finalValue;
	};

	struct ParabolicTransitionFromAccelerationParameters
	{
		double finalValue;
		double finalVelocity;
		double acceleration;
	};

	struct ReversalTransitionParameters
	{
		Seconds duration;
	};

	struct SinusoidalTransitionFromRangeParameters
	{
		Seconds duration;
		double minimumValue;
		double maximumValue;
		double period;
		AnimationSlope slope;
	};

	struct SinusoidalTransitionFromVelocityParameters
	{
		Seconds duration;
		double period;
	};

	struct SmoothStopTransitionParameters
	{
		Seconds maximumDuration;
		double finalValue;
	};

	using TransitionParameters = std::variant<
		AccelerateDecelerateTransitionParameters,
		ConstantTransitionParameters,
		CubicBezierLinearTransitionParameters,
		CubicTransitionParameters,
		DiscreteTransitionParameters,
		InstantaneousTransitionParameters,
		LinearTransitionParameters,
		LinearTransitionFromSpeedParameters,
		ParabolicTransitionFromAccelerationParameters,
		ReversalTransitionParameters,
		SinusoidalTransitionFromRangeParameters,
		SinusoidalTransitionFromVelocityParameters,
		SmoothStopTransitionParameters>;

	template <typename T>
	concept SupportsVectorValue = 
		IsInTypeList<T, 
		CubicBezierLinearTransitionParameters,
		CubicTransitionParameters,
		DiscreteTransitionParameters,
		InstantaneousTransitionParameters,
		LinearTransitionParameters,
		LinearTransitionFromSpeedParameters
	>;

	template <typename T>
	concept OnlySupportsSingleValue = 
		IsInTypeList<T, 
		AccelerateDecelerateTransitionParameters,
		ParabolicTransitionFromAccelerationParameters,
		SmoothStopTransitionParameters
	>;

	template <typename T>
	concept SupportsFinalValue = 
		IsInTypeList<T, 
		AccelerateDecelerateTransitionParameters,
		ParabolicTransitionFromAccelerationParameters,
		SmoothStopTransitionParameters,
		CubicBezierLinearTransitionParameters,
		CubicTransitionParameters,
		DiscreteTransitionParameters,
		InstantaneousTransitionParameters,
		LinearTransitionParameters,
		LinearTransitionFromSpeedParameters>;

	template <typename T>
	concept DoesntSupportFinalValue = 
		IsInTypeList<T, 
		ConstantTransitionParameters,
		ReversalTransitionParameters,
		SinusoidalTransitionFromRangeParameters,
		SinusoidalTransitionFromVelocityParameters>;

	class AnimationTransitionLibrary : public ComPtrHolder<IUIAnimationTransitionLibrary2>
	{
		public:
		[[nodiscard]] static auto GetInstance() -> const AnimationTransitionLibrary&;

		[[nodiscard]] static auto TransitionFromParameters(const TransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto AccelerateDecelerateTransition(
			const AccelerateDecelerateTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto AccelerateDecelerateTransition(
			Seconds duration, double finalValue,
			double accelerationRatio, double decelerationRatio) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ConstantTransition(Seconds duration) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ConstantTransition(const ConstantTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			Seconds duration, double finalValue,
			Point<double> p1, Point<double> p2) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			Seconds duration, std::span<const double> finalValues,
			Point<double> p1, Point<double> p2) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicBezierLinearTransition(
			const CubicBezierLinearTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			Seconds duration, double finalValue,
			double finalVelocity) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			Seconds duration, std::span<const double> finalValues,
			std::span<const double> finalVelocities) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto CubicTransition(
			const CubicTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			Seconds duration, double finalValue,
			double hold) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			Seconds duration, std::span<const double> finalValues,
			double hold) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto DiscreteTransition(
			const DiscreteTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto InstantaneousTransition(
			const InstantaneousTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			Seconds duration,
			double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			Seconds duration,
			std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransition(
			const LinearTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransitionFromSpeed(
			double speed,
			double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransitionFromSpeed(
			double speed,
			std::span<const double> finalValues) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto LinearTransitionFromSpeed(
			const LinearTransitionFromSpeedParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ParabolicTransitionFromAcceleration(
			double finalValue, double finalVelocity, double acceleration
		) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ParabolicTransitionFromAcceleration(
			const ParabolicTransitionFromAccelerationParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ReversalTransition(Seconds duration) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto ReversalTransition(const ReversalTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromRange(
			Seconds duration,
			double minimumValue, double maximumValue,
			double period, AnimationSlope slope) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromRange(
			const SinusoidalTransitionFromRangeParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromVelocity(
			Seconds duration, double period
		) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SinusoidalTransitionFromVelocity(
			const SinusoidalTransitionFromVelocityParameters& parameters) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SmoothStopTransition(
			Seconds maximumDuration,
			double finalValue) noexcept -> Result<AnimationTransition>;

		[[nodiscard]] static auto SmoothStopTransition(
			const SmoothStopTransitionParameters& parameters) noexcept -> Result<AnimationTransition>;

		private:
		AnimationTransitionLibrary();
	};

	auto SetFinalValueToTransitionParameters(TransitionParameters& parameters, double finalValue) noexcept -> Error;
	auto SetFinalValueToTransitionParameters(TransitionParameters& parameters, std::span<const double> finalValues) noexcept -> Error;
}

module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.EasingFunction;

import PGUI.ErrorHandling;
import PGUI.Wrapper;
import PGUI.Shape;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionEasingFunction> Projected = WUC::CompositionEasingFunction>
	class EasingFunctionT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;
	};
	using EasingFunction = EasingFunctionT<>;

	class BackEasingFunction : public EasingFunctionT<WUC::BackEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode, const float amplitude) noexcept -> Result<BackEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode, amplitude]
			{
				return BackEasingFunction{ WUC::CompositionEasingFunction::CreateBackEasingFunction(compositor, mode, amplitude) };
			});
		}

		[[nodiscard]] auto GetAmplitude() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Amplitude();
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}
	};

	class BounceEasingFunction : public EasingFunctionT<WUC::BounceEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode, const std::int32_t bounces, const float bounciness) noexcept -> Result<BounceEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode, bounces, bounciness]
			{
				return BounceEasingFunction{ WUC::CompositionEasingFunction::CreateBounceEasingFunction(compositor, mode, bounces, bounciness) };
			});
		}

		[[nodiscard]] auto GetBounces() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().Bounces();
			});
		}

		[[nodiscard]] auto GetBounciness() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Bounciness();
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}
	};

	class CircleEasingFunction : public EasingFunctionT<WUC::CircleEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode) noexcept -> Result<CircleEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode]
			{
				return CircleEasingFunction{ WUC::CompositionEasingFunction::CreateCircleEasingFunction(compositor, mode) };
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}
	};

	class CubicBezierEasingFunction : public EasingFunctionT<WUC::CubicBezierEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const Point2F& controlPoint1, const Point2F& controlPoint2) noexcept -> Result<CubicBezierEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &controlPoint1, &controlPoint2]
			{
				return CubicBezierEasingFunction{ WUC::CompositionEasingFunction::CreateCubicBezierEasingFunction(compositor, controlPoint1, controlPoint2) };
			});
		}

		[[nodiscard]] auto GetControlPoint1() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().ControlPoint1() };
			});
		}

		[[nodiscard]] auto GetControlPoint2() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().ControlPoint2() };
			});
		}
	};

	class ElasticEasingFunction : public EasingFunctionT<WUC::ElasticEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode, const std::int32_t oscillations, const float springiness) noexcept -> Result<ElasticEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode, oscillations, springiness]
			{
				return ElasticEasingFunction{ WUC::CompositionEasingFunction::CreateElasticEasingFunction(compositor, mode, oscillations, springiness) };
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}

		[[nodiscard]] auto GetOscillations() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().Oscillations();
			});
		}

		[[nodiscard]] auto GetSpringiness() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Springiness();
			});
		}
	};

	class ExponentialEasingFunction : public EasingFunctionT<WUC::ExponentialEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode, const float exponent) noexcept -> Result<ExponentialEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode, exponent]
			{
				return ExponentialEasingFunction{ WUC::CompositionEasingFunction::CreateExponentialEasingFunction(compositor, mode, exponent) };
			});
		}

		[[nodiscard]] auto GetExponent() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Exponent();
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}
	};

	class LinearEasingFunction : public EasingFunctionT<WUC::LinearEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create() noexcept -> Result<LinearEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return LinearEasingFunction{ WUC::CompositionEasingFunction::CreateLinearEasingFunction(compositor) };
			});
		}
	};

	class PowerEasingFunction : public EasingFunctionT<WUC::PowerEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode, const float power) noexcept -> Result<PowerEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode, power]
			{
				return PowerEasingFunction{ WUC::CompositionEasingFunction::CreatePowerEasingFunction(compositor, mode, power) };
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}

		[[nodiscard]] auto GetPower() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Power();
			});
		}
	};

	class SineEasingFunction : public EasingFunctionT<WUC::SineEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create(const EasingFunctionMode mode) noexcept -> Result<SineEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, mode]
			{
				return SineEasingFunction{ WUC::CompositionEasingFunction::CreateSineEasingFunction(compositor, mode) };
			});
		}

		[[nodiscard]] auto GetMode() const noexcept -> Result<EasingFunctionMode>
		{
			return CallProjected([this]
			{
				return Get().Mode();
			});
		}
	};

	class StepEasingFunction : public EasingFunctionT<WUC::StepEasingFunction>
	{
		public:
		using EasingFunctionT::EasingFunctionT;

		[[nodiscard]] static auto Create() noexcept -> Result<StepEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return StepEasingFunction{ WUC::CompositionEasingFunction::CreateStepEasingFunction(compositor) };
			});
		}
		[[nodiscard]] static auto Create(const std::int32_t stepCount) noexcept -> Result<StepEasingFunction>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, stepCount]
			{
				return StepEasingFunction{ WUC::CompositionEasingFunction::CreateStepEasingFunction(compositor, stepCount) };
			});
		}

		[[nodiscard]] auto GetFinalStep() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().FinalStep();
			});
		}
		[[nodiscard]] auto SetFinalStep(const std::int32_t finalStep) const noexcept -> Result<void>
		{
			return CallProjected([this, finalStep]
			{
				Get().FinalStep(finalStep);
			});
		}

		[[nodiscard]] auto GetInitialStep() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().InitialStep();
			});
		}
		[[nodiscard]] auto SetInitialStep(const std::int32_t initialStep) const noexcept -> Result<void>
		{
			return CallProjected([this, initialStep]
			{
				Get().InitialStep(initialStep);
			});
		}

		[[nodiscard]] auto EnableFinalStepSingleFrame(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				Get().IsFinalStepSingleFrame(enabled);
			});
		}
		[[nodiscard]] auto DisableFinalStepSingleFrame() const noexcept -> Result<void> { return EnableFinalStepSingleFrame(false); }
		[[nodiscard]] auto IsFinalStepSingleFrame() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return Get().IsFinalStepSingleFrame();
			});
		}

		[[nodiscard]] auto EnableInitialStepSingleFrame(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				Get().IsInitialStepSingleFrame(enabled);
			});
		}
		[[nodiscard]] auto DisableInitialStepSingleFrame() const noexcept -> Result<void> { return EnableInitialStepSingleFrame(false); }
		[[nodiscard]] auto IsInitialStepSingleFrame() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return Get().IsInitialStepSingleFrame();
			});
		}

		[[nodiscard]] auto GetStepCount() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().StepCount();
			});
		}
		[[nodiscard]] auto SetStepCount(const std::int32_t stepCount) const noexcept -> Result<void>
		{
			return CallProjected([this, stepCount]
			{
				Get().StepCount(stepCount);
			});
		}
	};
}

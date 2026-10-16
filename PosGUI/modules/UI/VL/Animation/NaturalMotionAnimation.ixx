module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.h>

export module PGUI.UI.VL.NaturalMotionAnimation;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.UI.VL.CompositionAnimation;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;
namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::NaturalMotionAnimation> Projected = WUC::NaturalMotionAnimation>
	class NaturalMotionAnimationT : public CompositionAnimationT<Projected>
	{
		public:
		using Base = CompositionAnimationT<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetDelayBehavior() const noexcept -> Result<AnimationDelayBehavior>
		{
			return CallProjected([this]
			{
				return this->Get().DelayBehavior();
			});
		}
		[[nodiscard]] auto SetDelayBehavior(const AnimationDelayBehavior delayBehavior) const noexcept -> Result<void>
		{
			return CallProjected([this, delayBehavior]
			{
				this->Get().DelayBehavior(delayBehavior);
			});
		}

		[[nodiscard]] auto GetDelayTime() const noexcept -> Result<std::chrono::milliseconds>
		{
			return CallProjected([this]
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(this->Get().DelayTime());
			});
		}
		[[nodiscard]] auto SetDelayTime(const std::chrono::milliseconds& delayTime) const noexcept -> Result<void>
		{
			return CallProjected([this, &delayTime]
			{
				this->Get().DelayTime(delayTime);
			});
		}

		[[nodiscard]] auto GetStopBehavior() const noexcept -> Result<AnimationStopBehavior>
		{
			return CallProjected([this]
			{
				return this->Get().StopBehavior();
			});
		}
		[[nodiscard]] auto SetStopBehavior(const AnimationStopBehavior stopBehavior) const noexcept -> Result<void>
		{
			return CallProjected([this, stopBehavior]
			{
				this->Get().StopBehavior(stopBehavior);
			});
		}
	};
	using NaturalMotionAnimation = NaturalMotionAnimationT<>;

	template <HandleDerivedFrom<WUC::ScalarNaturalMotionAnimation> Projected = WUC::ScalarNaturalMotionAnimation>
	class ScalarNaturalMotionAnimationT : public NaturalMotionAnimationT<Projected>
	{
		public:
		using Base = NaturalMotionAnimationT<Projected>;
		using Base::Base;

		using AnimationTag = Expression::ExpressionTypes::Scalar;

		[[nodiscard]] auto GetFinalValue() const noexcept -> Result<std::optional<float>>
		{
			return CallProjected([this]
			{
				return std::optional<float>{ this->Get().FinalValue() };
			});
		}
		[[nodiscard]] auto SetFinalValue(const std::optional<float> finalValue) const noexcept -> Result<void>
		{
			return CallProjected([this, finalValue]
			{
				this->Get().FinalValue(finalValue);
			});
		}

		[[nodiscard]] auto GetInitialValue() const noexcept -> Result<std::optional<float>>
		{
			return CallProjected([this]
			{
				return std::optional<float>{ this->Get().InitialValue() };
			});
		}
		[[nodiscard]] auto SetInitialValue(const std::optional<float> initialValue) const noexcept -> Result<void>
		{
			return CallProjected([this, initialValue]
			{
				this->Get().InitialValue(initialValue);
			});
		}

		[[nodiscard]] auto GetInitialVelocity() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().InitialVelocity();
			});
		}
		[[nodiscard]] auto SetInitialVelocity(const float initialVelocity) const noexcept -> Result<void>
		{
			return CallProjected([this, initialVelocity]
			{
				this->Get().InitialVelocity(initialVelocity);
			});
		}
	};
	using ScalarNaturalMotionAnimation = ScalarNaturalMotionAnimationT<>;

	class BounceScalarNaturalMotionAnimation : public ScalarNaturalMotionAnimationT<WUC::BounceScalarNaturalMotionAnimation>
	{
		public:
		using ScalarNaturalMotionAnimationT::ScalarNaturalMotionAnimationT;

		[[nodiscard]] static auto Create() noexcept -> Result<BounceScalarNaturalMotionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return BounceScalarNaturalMotionAnimation{ compositor.CreateBounceScalarAnimation() };
			});
		}

		[[nodiscard]] auto GetAcceleration() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Acceleration();
			});
		}
		[[nodiscard]] auto SetAcceleration(const float acceleration) const noexcept -> Result<void>
		{
			return CallProjected([this, acceleration]
			{
				Get().Acceleration(acceleration);
			});
		}

		[[nodiscard]] auto GetRestitution() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Restitution();
			});
		}
		[[nodiscard]] auto SetRestitution(const float restitution) const noexcept -> Result<void>
		{
			return CallProjected([this, restitution]
			{
				Get().Restitution(restitution);
			});
		}
	};
	class SpringScalarNaturalMotionAnimation : public ScalarNaturalMotionAnimationT<WUC::SpringScalarNaturalMotionAnimation>
	{
		public:
		using ScalarNaturalMotionAnimationT::ScalarNaturalMotionAnimationT;

		[[nodiscard]] static auto Create() noexcept -> Result<SpringScalarNaturalMotionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return SpringScalarNaturalMotionAnimation{ compositor.CreateSpringScalarAnimation() };
			});
		}

		[[nodiscard]] auto GetDampingRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().DampingRatio();
			});
		}
		[[nodiscard]] auto SetDampingRatio(const float dampingRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, dampingRatio]
			{
				Get().DampingRatio(dampingRatio);
			});
		}

		[[nodiscard]] auto GetPeriod() const noexcept -> Result<std::chrono::milliseconds>
		{
			return CallProjected([this]
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(Get().Period());
			});
		}
		[[nodiscard]] auto SetPeriod(const std::chrono::milliseconds& period) const noexcept -> Result<void>
		{
			return CallProjected([this, &period]
			{
				Get().Period(period);
			});
		}
	};

	template <HandleDerivedFrom<WUC::Vector2NaturalMotionAnimation> Projected = WUC::Vector2NaturalMotionAnimation>
	class Vector2NaturalMotionAnimationT : public NaturalMotionAnimationT<Projected>
	{
		public:
		using Base = NaturalMotionAnimationT<Projected>;
		using Base::Base;

		using AnimationTag = Expression::ExpressionTypes::Vector2;

		[[nodiscard]] auto GetFinalValue() const noexcept -> Result<std::optional<Point2F>>
		{
			return CallProjected([this]
			{
				return std::optional<WFN::float2>{ this->Get().FinalValue() }.transform([](const auto& value)
				{
					return Point2F{ value };
				});
			});
		}
		[[nodiscard]] auto SetFinalValue(const std::optional<Point2F>& finalValue) const noexcept -> Result<void>
		{
			return CallProjected([this, &finalValue]
			{
				this->Get().FinalValue(std::optional<WFN::float2>{ finalValue });
			});
		}

		[[nodiscard]] auto GetInitialValue() const noexcept -> Result<std::optional<Point2F>>
		{
			return CallProjected([this]
			{
				return std::optional<WFN::float2>{ this->Get().InitialValue() }.transform([](const auto& value)
				{
					return Point2F{ value };
				});
			});
		}
		[[nodiscard]] auto SetInitialValue(const std::optional<Point2F>& initialValue) const noexcept -> Result<void>
		{
			return CallProjected([this, &initialValue]
			{
				this->Get().InitialValue(std::optional<WFN::float2>{ initialValue });
			});
		}

		[[nodiscard]] auto GetInitialVelocity() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().InitialVelocity() };
			});
		}
		[[nodiscard]] auto SetInitialVelocity(const Point2F& initialVelocity) const noexcept -> Result<void>
		{
			return CallProjected([this, &initialVelocity]
			{
				this->Get().InitialVelocity(initialVelocity);
			});
		}
	};
	using Vector2NaturalMotionAnimation = Vector2NaturalMotionAnimationT<>;

	class BounceVector2NaturalMotionAnimation : public Vector2NaturalMotionAnimationT<WUC::BounceVector2NaturalMotionAnimation>
	{
		public:
		using Vector2NaturalMotionAnimationT::Vector2NaturalMotionAnimationT;

		[[nodiscard]] static auto Create() noexcept -> Result<BounceVector2NaturalMotionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return BounceVector2NaturalMotionAnimation{ compositor.CreateBounceVector2Animation() };
			});
		}

		[[nodiscard]] auto GetAcceleration() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Acceleration();
			});
		}
		[[nodiscard]] auto SetAcceleration(const float acceleration) const noexcept -> Result<void>
		{
			return CallProjected([this, acceleration]
			{
				Get().Acceleration(acceleration);
			});
		}

		[[nodiscard]] auto GetRestitution() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Restitution();
			});
		}
		[[nodiscard]] auto SetRestitution(const float restitution) const noexcept -> Result<void>
		{
			return CallProjected([this, restitution]
			{
				Get().Restitution(restitution);
			});
		}
	};
	class SpringVector2NaturalMotionAnimation : public Vector2NaturalMotionAnimationT<WUC::SpringVector2NaturalMotionAnimation>
	{
		public:
		using Vector2NaturalMotionAnimationT::Vector2NaturalMotionAnimationT;

		[[nodiscard]] static auto Create() noexcept -> Result<SpringVector2NaturalMotionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return SpringVector2NaturalMotionAnimation{ compositor.CreateSpringVector2Animation() };
			});
		}

		[[nodiscard]] auto GetDampingRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().DampingRatio();
			});
		}
		[[nodiscard]] auto SetDampingRatio(const float dampingRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, dampingRatio]
			{
				Get().DampingRatio(dampingRatio);
			});
		}

		[[nodiscard]] auto GetPeriod() const noexcept -> Result<std::chrono::milliseconds>
		{
			return CallProjected([this]
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(Get().Period());
			});
		}
		[[nodiscard]] auto SetPeriod(const std::chrono::milliseconds& period) const noexcept -> Result<void>
		{
			return CallProjected([this, &period]
			{
				Get().Period(period);
			});
		}
	};

	template <HandleDerivedFrom<WUC::Vector3NaturalMotionAnimation> Projected = WUC::Vector3NaturalMotionAnimation>
	class Vector3NaturalMotionAnimationT : public NaturalMotionAnimationT<Projected>
	{
		public:
		using Base = NaturalMotionAnimationT<Projected>;
		using Base::Base;

		using AnimationTag = Expression::ExpressionTypes::Vector3;

		[[nodiscard]] auto GetFinalValue() const noexcept -> Result<std::optional<Point3F>>
		{
			return CallProjected([this]
			{
				return std::optional<WFN::float3>{ this->Get().FinalValue() }.transform([](const auto& value)
				{
					return Point3F{ value };
				});
			});
		}
		[[nodiscard]] auto SetFinalValue(const std::optional<Point3F>& finalValue) const noexcept -> Result<void>
		{
			return CallProjected([this, &finalValue]
			{
				this->Get().FinalValue(std::optional<WFN::float3>{ finalValue });
			});
		}

		[[nodiscard]] auto GetInitialValue() const noexcept -> Result<std::optional<Point3F>>
		{
			return CallProjected([this]
			{
				return std::optional<WFN::float3>{ this->Get().InitialValue() }.transform([](const auto& value)
				{
					return Point3F{ value };
				});
			});
		}
		[[nodiscard]] auto SetInitialValue(const std::optional<Point3F>& initialValue) const noexcept -> Result<void>
		{
			return CallProjected([this, &initialValue]
			{
				this->Get().InitialValue(std::optional<WFN::float3>{ initialValue });
			});
		}

		[[nodiscard]] auto GetInitialVelocity() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ this->Get().InitialVelocity() };
			});
		}
		[[nodiscard]] auto SetInitialVelocity(const Point3F& initialVelocity) const noexcept -> Result<void>
		{
			return CallProjected([this, &initialVelocity]
			{
				this->Get().InitialVelocity(initialVelocity);
			});
		}
	};
	using Vector3NaturalMotionAnimation = Vector3NaturalMotionAnimationT<>;

	class BounceVector3NaturalMotionAnimation : public Vector3NaturalMotionAnimationT<WUC::BounceVector3NaturalMotionAnimation>
	{
		public:
		using Vector3NaturalMotionAnimationT::Vector3NaturalMotionAnimationT;

		[[nodiscard]] static auto Create() noexcept -> Result<BounceVector3NaturalMotionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return BounceVector3NaturalMotionAnimation{ compositor.CreateBounceVector3Animation() };
			});
		}

		[[nodiscard]] auto GetAcceleration() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Acceleration();
			});
		}
		[[nodiscard]] auto SetAcceleration(const float acceleration) const noexcept -> Result<void>
		{
			return CallProjected([this, acceleration]
			{
				Get().Acceleration(acceleration);
			});
		}

		[[nodiscard]] auto GetRestitution() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Restitution();
			});
		}
		[[nodiscard]] auto SetRestitution(const float restitution) const noexcept -> Result<void>
		{
			return CallProjected([this, restitution]
			{
				Get().Restitution(restitution);
			});
		}
	};
	class SpringVector3NaturalMotionAnimation : public Vector3NaturalMotionAnimationT<WUC::SpringVector3NaturalMotionAnimation>
	{
		public:
		using Vector3NaturalMotionAnimationT::Vector3NaturalMotionAnimationT;

		[[nodiscard]] static auto Create() noexcept -> Result<SpringVector3NaturalMotionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return SpringVector3NaturalMotionAnimation{ compositor.CreateSpringVector3Animation() };
			});
		}

		[[nodiscard]] auto GetDampingRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().DampingRatio();
			});
		}
		[[nodiscard]] auto SetDampingRatio(const float dampingRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, dampingRatio]
			{
				Get().DampingRatio(dampingRatio);
			});
		}

		[[nodiscard]] auto GetPeriod() const noexcept -> Result<std::chrono::milliseconds>
		{
			return CallProjected([this]
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(Get().Period());
			});
		}
		[[nodiscard]] auto SetPeriod(const std::chrono::milliseconds& period) const noexcept -> Result<void>
		{
			return CallProjected([this, &period]
			{
				Get().Period(period);
			});
		}
	};
}

module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

export module PGUI.UI.VL.Interaction.InteractionTrackerInertiaModifier;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.ExpressionAnimation;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.NaturalMotionAnimation;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUCI::InteractionTrackerInertiaModifier> Projected = WUCI::InteractionTrackerInertiaModifier>
	class InteractionTrackerInertiaModifierT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;
	};
	using InteractionTrackerInertiaModifier = InteractionTrackerInertiaModifierT<>;

	class InteractionTrackerInertiaMotion : public InteractionTrackerInertiaModifierT<WUCI::InteractionTrackerInertiaMotion>
	{
		public:
		using InteractionTrackerInertiaModifierT::InteractionTrackerInertiaModifierT;

		[[nodiscard]] static auto Create() noexcept -> Result<InteractionTrackerInertiaMotion>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return InteractionTrackerInertiaMotion{ WUCI::InteractionTrackerInertiaMotion::Create(compositor) };
			});
		}

		[[nodiscard]] auto GetCondition() const noexcept -> Result<ExpressionAnimation<Expression::ExpressionTypes::Bool>>
		{
			return CallProjected([this]
			{
				return ExpressionAnimation<Expression::ExpressionTypes::Bool>{ this->Get().Condition() };
			});
		}
		[[nodiscard]] auto SetCondition(const ExpressionAnimation<Expression::ExpressionTypes::Bool>& condition) const noexcept -> Result<void>
		{
			return CallProjected([this, &condition]
			{
				this->Get().Condition(condition.Get());
			});
		}

		[[nodiscard]] auto GetMotion() const noexcept -> Result<ExpressionAnimation<Expression::ExpressionTypes::Scalar>>
		{
			return CallProjected([this]
			{
				return ExpressionAnimation<Expression::ExpressionTypes::Scalar>{ this->Get().Motion() };
			});
		}
		[[nodiscard]] auto SetMotion(const ExpressionAnimation<Expression::ExpressionTypes::Scalar>& motion) const noexcept -> Result<void>
		{
			return CallProjected([this, &motion]
			{
				this->Get().Motion(motion.Get());
			});
		}
	};

	class InteractionTrackerInertiaNaturalMotion : public InteractionTrackerInertiaModifierT<WUCI::InteractionTrackerInertiaNaturalMotion>
	{
		public:
		using InteractionTrackerInertiaModifierT::InteractionTrackerInertiaModifierT;

		[[nodiscard]] static auto Create() noexcept -> Result<InteractionTrackerInertiaNaturalMotion>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return InteractionTrackerInertiaNaturalMotion{ WUCI::InteractionTrackerInertiaNaturalMotion::Create(compositor) };
			});
		}

		[[nodiscard]] auto GetCondition() const noexcept -> Result<ExpressionAnimation<Expression::ExpressionTypes::Bool>>
		{
			return CallProjected([this]
			{
				return ExpressionAnimation<Expression::ExpressionTypes::Bool>{ this->Get().Condition() };
			});
		}
		[[nodiscard]] auto SetCondition(const ExpressionAnimation<Expression::ExpressionTypes::Bool>& condition) const noexcept -> Result<void>
		{
			return CallProjected([this, &condition]
			{
				this->Get().Condition(condition.Get());
			});
		}

		[[nodiscard]] auto GetNaturalMotion() const noexcept -> Result<ScalarNaturalMotionAnimation>
		{
			return CallProjected([this]
			{
				return ScalarNaturalMotionAnimation{ this->Get().NaturalMotion() };
			});
		}
		[[nodiscard]] auto SetNaturalMotion(const ScalarNaturalMotionAnimation& naturalMotion) const noexcept -> Result<void>
		{
			return CallProjected([this, &naturalMotion]
			{
				this->Get().NaturalMotion(naturalMotion.Get());
			});
		}
	};

	class InteractionTrackerInertiaRestingValue : public InteractionTrackerInertiaModifierT<WUCI::InteractionTrackerInertiaRestingValue>
	{
		public:
		using InteractionTrackerInertiaModifierT::InteractionTrackerInertiaModifierT;

		[[nodiscard]] static auto Create() noexcept -> Result<InteractionTrackerInertiaRestingValue>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return InteractionTrackerInertiaRestingValue{ WUCI::InteractionTrackerInertiaRestingValue::Create(compositor) };
			});
		}

		[[nodiscard]] auto GetCondition() const noexcept -> Result<ExpressionAnimation<Expression::ExpressionTypes::Bool>>
		{
			return CallProjected([this]
			{
				return ExpressionAnimation<Expression::ExpressionTypes::Bool>{ this->Get().Condition() };
			});
		}
		[[nodiscard]] auto SetCondition(const ExpressionAnimation<Expression::ExpressionTypes::Bool>& condition) const noexcept -> Result<void>
		{
			return CallProjected([this, &condition]
			{
				this->Get().Condition(condition.Get());
			});
		}

		[[nodiscard]] auto GetRestingValue() const noexcept -> Result<ExpressionAnimation<Expression::ExpressionTypes::Scalar>>
		{
			return CallProjected([this]
			{
				return ExpressionAnimation<Expression::ExpressionTypes::Scalar>{ this->Get().RestingValue() };
			});
		}
		[[nodiscard]] auto SetRestingValue(const ExpressionAnimation<Expression::ExpressionTypes::Scalar>& restingValue) const noexcept -> Result<void>
		{
			return CallProjected([this, &restingValue]
			{
				this->Get().RestingValue(restingValue.Get());
			});
		}
	};
}

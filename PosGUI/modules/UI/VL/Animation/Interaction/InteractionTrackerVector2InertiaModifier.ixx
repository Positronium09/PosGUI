module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

export module PGUI.UI.VL.Interaction.InteractionTrackerVector2InertiaModifier;

import PGUI.ErrorHandling;
import PGUI.Wrapper;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.ExpressionAnimation;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.NaturalMotionAnimation;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUCI::InteractionTrackerVector2InertiaModifier> Projected = WUCI::InteractionTrackerVector2InertiaModifier>
	class InteractionTrackerVector2InertiaModifierT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;
	};
	using InteractionTrackerVector2InertiaModifier = InteractionTrackerVector2InertiaModifierT<>;

	class InteractionTrackerVector2InertiaNaturalMotion : public InteractionTrackerVector2InertiaModifierT<WUCI::InteractionTrackerVector2InertiaNaturalMotion>
	{
		public:
		using InteractionTrackerVector2InertiaModifierT::InteractionTrackerVector2InertiaModifierT;

		[[nodiscard]] static auto Create() noexcept -> Result<InteractionTrackerVector2InertiaNaturalMotion>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return InteractionTrackerVector2InertiaNaturalMotion{ WUCI::InteractionTrackerVector2InertiaNaturalMotion::Create(compositor) };
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

		[[nodiscard]] auto GetNaturalMotion() const noexcept -> Result<Vector2NaturalMotionAnimation>
		{
			return CallProjected([this]
			{
				return Vector2NaturalMotionAnimation{ this->Get().NaturalMotion() };
			});
		}
		[[nodiscard]] auto SetNaturalMotion(const Vector2NaturalMotionAnimation& motion) const noexcept -> Result<void>
		{
			return CallProjected([this, &motion]
			{
				this->Get().NaturalMotion(motion.Get());
			});
		}
	};
}

module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

export module PGUI.UI.VL.Interaction.ConditionalValue;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.ExpressionAnimation;
import PGUI.UI.VL.Expression;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL
{
	template <Expression::ExpressionTypes::ExpressionType T>
	class ConditionalValue : public CompositionObjectT<WUCI::CompositionConditionalValue>
	{
		public:
		using AnimationTag = T;

		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] static auto Create() noexcept -> Result<ConditionalValue>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return ConditionalValue{ WUCI::CompositionConditionalValue::Create(compositor) };
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

		[[nodiscard]] auto GetValue() const noexcept -> Result<ExpressionAnimation<T>>
		{
			return CallProjected([this]
			{
				return ExpressionAnimation<T>{ this->Get().Value() };
			});
		}
		[[nodiscard]] auto SetValue(const ExpressionAnimation<T>& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				this->Get().Value(value.Get());
			});
		}
	};

	using ScalarConditionalValue = ConditionalValue<Expression::ExpressionTypes::Scalar>;
}

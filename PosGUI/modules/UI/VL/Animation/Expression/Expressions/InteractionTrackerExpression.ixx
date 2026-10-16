module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:InteractionTracker;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.Interaction.InteractionTracker;
import PGUI.Wrapper;
import :Bool;
import :Scalar;
import :Vector3;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL::Expression
{
	template <WStringLiteral Name, WrapperDerivedFrom<WUCI::InteractionTracker> InteractionTrackerT>
	struct ExpressionRef<Name, InteractionTrackerT> : Expression<ExpressionTypes::ExpressionTypeOf<InteractionTrackerT>>
	{
		explicit ExpressionRef(InteractionTrackerT object) noexcept :
			Expression<ExpressionTypes::ExpressionTypeOf<InteractionTrackerT>>{ Name.data },
			object{ object }
		{
			this->AddReference(Name.data, Wrapper<WUC::CompositionObject>{ object });
		}
		[[nodiscard]] const auto& GetObject() const noexcept
		{
			return object;
		}

		[[nodiscard]] auto IsInertiaFromImpulse() const noexcept -> BoolExpression
		{
			return StubDetail::Stub<ExpressionTypes::Bool>{ std::format(L"{}.IsInertiaFromImpulse", Name.data), *this };
		}
		[[nodiscard]] auto IsPositionRoundingSuggested() const noexcept -> BoolExpression
		{
			return StubDetail::Stub<ExpressionTypes::Bool>{ std::format(L"{}.IsPositionRoundingSuggested", Name.data), *this };
		}

		[[nodiscard]] auto MaxPosition() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.MaxPosition", Name.data), *this };
		}
		[[nodiscard]] auto MaxScale() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.MaxScale", Name.data), *this };
		}
		[[nodiscard]] auto MinPosition() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.MinPosition", Name.data), *this };
		}
		[[nodiscard]] auto MinScale() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.MinScale", Name.data), *this };
		}

		[[nodiscard]] auto NaturalRestingPosition() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.NaturalRestingPosition", Name.data), *this };
		}
		[[nodiscard]] auto NaturalRestingScale() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.NaturalRestingScale", Name.data), *this };
		}

		[[nodiscard]] auto Position() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.Position", Name.data), *this };
		}
		[[nodiscard]] auto PositionVelocityInPixelsPerSecond() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.PositionVelocityInPixelsPerSecond", Name.data), *this };
		}

		[[nodiscard]] auto Scale() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Scale", Name.data), *this };
		}
		[[nodiscard]] auto ScaleVelocityInPercentPerSecond() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.ScaleVelocityInPercentPerSecond", Name.data), *this };
		}

		private:
		InteractionTrackerT object;
	};
}

module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.ExpressionAnimation;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.Wrapper;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.Properties;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.CompositionAnimation;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <Expression::ExpressionTypes::ExpressionType T>
	class ExpressionAnimation : public CompositionAnimationT<WUC::ExpressionAnimation>
	{
		public:
		using CompositionAnimationT::CompositionAnimationT;
		using AnimationTag = T;
		
		[[nodiscard]] static auto Create(const Expression::IsExpressionOfType<T> auto& e) noexcept -> Result<ExpressionAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			const auto eaResult = CallProjected([&compositor, &e]
			{
				return compositor.CreateExpressionAnimation(e.GetExpression());
			});
			if (!eaResult.has_value())
			{
				return Unexpected{ eaResult.error() };
			}
			const auto& ea = eaResult.value();

			if (const auto bindResult = e.BindReferences(ea);
				!bindResult.has_value())
			{
				return Unexpected{ bindResult.error() };
			}

			return ExpressionAnimation{ ea };
		}

		template <Properties::IsProperty P> requires std::same_as<T, typename P::ValueTag>
		[[nodiscard]] auto SetTarget() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().Target(P::name);
			});
		}

		[[nodiscard]] auto GetExpression() const noexcept -> Result<std::wstring>
		{
			return CallProjected([&]
			{
				return std::wstring{ this->Get().Expression() };
			});
		}

		[[nodiscard]] auto SetExpression(const Expression::IsExpressionOfType<T> auto& e) const noexcept -> Result<void>
		{
			if (const auto result = CallProjected([&]
			{
				this->Get().Expression(e.GetExpression());
			});
				!result.has_value())
			{
				return result;
			}

			return e.BindReferences(this->Get());
		}
	};

	template <Expression::IsExpression E>
	[[nodiscard]] auto CreateExpressionAnimation(const E& e) noexcept -> Result<ExpressionAnimation<typename E::ExpressionTag>>
	{
		return ExpressionAnimation<typename E::ExpressionTag>::Create(e);
	}
}

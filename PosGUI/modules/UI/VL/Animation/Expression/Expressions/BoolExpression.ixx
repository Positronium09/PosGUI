module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Bool;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.Wrapper;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	struct BoolExpression : Expression<ExpressionTypes::Bool>
	{
		template <IsExpression T, IsExpression U> requires
		std::same_as<typename T::ExpressionTag, typename U::ExpressionTag>
		static auto Equals(const T& left, const U& right)
		{
			return BoolExpression{ std::format(L"({} == {})", left.GetExpression(), right.GetExpression()), left, right };
		}
		template <IsExpression T, IsExpression U> requires
			std::same_as<typename T::ExpressionTag, typename U::ExpressionTag>
			static auto NotEquals(const T& left, const U& right)
		{
			return BoolExpression{ std::format(L"({} != {})", left.GetExpression(), right.GetExpression()), left, right };
		}
		static auto LessThan(
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& left, 
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& right)
		{
			return BoolExpression{ std::format(L"({} < {})", left.GetExpression(), right.GetExpression()), left, right };
		}
		static auto LessThanOrEqual(
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& left, 
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& right)
		{
			return BoolExpression{ std::format(L"({} <= {})", left.GetExpression(), right.GetExpression()), left, right };
		}
		static auto GreaterThan(
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& left,
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& right)
		{
			return BoolExpression{ std::format(L"({} > {})", left.GetExpression(), right.GetExpression()), left, right };
		}
		static auto GreaterThanOrEqual(
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& left,
			const IsExpressionOfType<ExpressionTypes::Scalar> auto& right)
		{
			return BoolExpression{ std::format(L"({} >= {})", left.GetExpression(), right.GetExpression()), left, right };
		}

		explicit BoolExpression(const bool value) noexcept :
			Expression{ value ? L"True" : L"False" }
		{ }

		template <IsExpressionOfType<ExpressionTypes::Bool> T>
		explicit(false) BoolExpression(const T& expression) noexcept :
			Expression{ expression.GetExpression() }
		{
			CopyReferences(expression);
		}

		[[nodiscard]] auto operator!() const noexcept -> BoolExpression
		{
			if (GetExpression().starts_with(L"!"))
			{
				return BoolExpression{ GetExpression().substr(1), *this };
			}
			return BoolExpression{ std::format(L"(!{})", GetExpression()), *this };
		}
		[[nodiscard]] auto operator&&(const BoolExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression{ std::format(L"({} && {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator||(const BoolExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression{ std::format(L"({} || {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator==(const BoolExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression{ std::format(L"({} == {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator!=(const BoolExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression{ std::format(L"({} != {})", GetExpression(), other.GetExpression()), *this, other };
		}

		private:
		explicit BoolExpression(const std::wstring_view expression, const auto&... operands) noexcept :
			Expression{ expression }
		{
			CopyReferences(operands...);
		}
	};

	template <ExpressionTypes::ExpressionType T>
	struct IfThenElse : Expression<T>
	{
		IfThenElse(const IsExpressionOfType<ExpressionTypes::Bool> auto& condition, 
			const IsExpressionOfType<T> auto& thenExpr, 
			const IsExpressionOfType<T> auto& elseExpr) noexcept : 
			Expression<T>{ std::format(L"({} ? {} : {})", 
				condition.GetExpression(), 
				thenExpr.GetExpression(), elseExpr.GetExpression()) }
		{
			CopyReferences(condition, thenExpr, elseExpr);
		}
	};

	const auto True = BoolExpression{ true };
	const auto False = BoolExpression{ false };
}

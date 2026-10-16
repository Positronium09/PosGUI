module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Scalar;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.Wrapper;
import :Bool;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

namespace PGUI::UI::VL::Expression::Internal
{
	struct PiLiteral : Expression<ExpressionTypes::Scalar>
	{
		PiLiteral() noexcept : Expression{ L"Pi" }
		{ }
	};
}

export namespace PGUI::UI::VL::Expression
{
	struct ScalarExpression : Expression<ExpressionTypes::Scalar>
	{
		template <typename T> requires std::is_arithmetic_v<T>
		explicit(false) ScalarExpression(T value) noexcept : 
			Expression{ std::to_wstring(value) }
		{ }

		template <IsExpressionOfType<ExpressionTypes::Scalar> T>
		explicit(false) ScalarExpression(const T& expression) noexcept :
			Expression{ expression.GetExpression() }
		{
			CopyReferences(expression);
		}

		[[nodiscard]] auto operator+(const ScalarExpression& other) const noexcept -> ScalarExpression
		{
			return ScalarExpression{ std::format(L"({} + {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator-(const ScalarExpression& other) const noexcept -> ScalarExpression
		{
			return ScalarExpression{ std::format(L"({} - {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator*(const ScalarExpression& other) const noexcept -> ScalarExpression
		{
			return ScalarExpression{ std::format(L"({} * {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator/(const ScalarExpression& other) const noexcept -> ScalarExpression
		{
			return ScalarExpression{ std::format(L"({} / {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator-() const noexcept -> ScalarExpression
		{
			if (GetExpression().starts_with(L"-"))
			{
				return ScalarExpression{ GetExpression().substr(1), *this };
			}
			return ScalarExpression{ std::format(L"(-{})", GetExpression()), *this };
		}
		[[nodiscard]] auto operator==(const ScalarExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::Equals(*this, other);
		}
		[[nodiscard]] auto operator!=(const ScalarExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::NotEquals(*this, other);
		}
		[[nodiscard]] auto operator<(const ScalarExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::LessThan(*this, other);
		}
		[[nodiscard]] auto operator<=(const ScalarExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::LessThanOrEqual(*this, other);
		}
		[[nodiscard]] auto operator>(const ScalarExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::GreaterThan(*this, other);
		}
		[[nodiscard]] auto operator>=(const ScalarExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::GreaterThanOrEqual(*this, other);
		}

		private:
		explicit ScalarExpression(const std::wstring_view expression, const auto&... operands) noexcept :
			Expression{ expression }
		{
			CopyReferences(operands...);
		}
	};

	const auto Pi = ScalarExpression{ Internal::PiLiteral{ } };

	auto Abs(const ScalarExpression& expression) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"abs({})", expression.GetExpression()), expression };
	}
	auto Clamp(const ScalarExpression& expression, const ScalarExpression& min, const ScalarExpression& max) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"clamp({}, {}, {})", 
			expression.GetExpression(), min.GetExpression(), max.GetExpression()), 
			expression, min, max };
	}
	auto Max(const ScalarExpression& value1, const ScalarExpression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"max({}, {})", 
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Min(const ScalarExpression& value1, const ScalarExpression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"min({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Lerp(const ScalarExpression& value1, const ScalarExpression& value2, const ScalarExpression& progress) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"lerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}

	// ReSharper disable once IdentifierTypo

	auto Slerp(const ScalarExpression& value1, const ScalarExpression& value2, const ScalarExpression& progress) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"slerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}
	auto Mod(const ScalarExpression& value1, const ScalarExpression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"mod({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Ceil(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"ceil({})", value.GetExpression()), value };
	}
	auto Floor(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"floor({})", value.GetExpression()), value };
	}
	auto Sqrt(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"sqrt({})", value.GetExpression()), value };
	}
	auto Square(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"square({})", value.GetExpression()), value };
	}
	auto Sin(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"sin({})", value.GetExpression()), value };
	}
	auto Asin(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"asin({})", value.GetExpression()), value };
	}
	auto Cos(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"cos({})", value.GetExpression()), value };
	}
	auto Acos(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"acos({})", value.GetExpression()), value };
	}
	auto Tan(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"tan({})", value.GetExpression()), value };
	}
	auto Atan(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"atan({})", value.GetExpression()), value };
	}
	auto Round(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"round({})", value.GetExpression()), value };
	}
	auto Log10(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"log10({})", value.GetExpression()), value };
	}
	auto Ln(const ScalarExpression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"ln({})", value.GetExpression()), value };
	}
	auto Pow(const ScalarExpression& value, const ScalarExpression& power) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"pow({}, {})",
			value.GetExpression(), power.GetExpression()), value, power };
	}
	auto ToDegrees(const ScalarExpression& radians) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"todegrees({})", radians.GetExpression()), radians };
	}
	auto ToRadians(const ScalarExpression& degrees) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"toradians({})", degrees.GetExpression()), degrees };
	}
}

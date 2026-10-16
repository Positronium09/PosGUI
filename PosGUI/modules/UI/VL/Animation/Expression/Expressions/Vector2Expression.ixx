module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Vector2;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.Wrapper;
import :Bool;
import :Scalar;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	struct Vector2Expression : Expression<ExpressionTypes::Vector2>
	{
		Vector2Expression(const ScalarExpression& x, const ScalarExpression& y) noexcept :
			Expression{ std::format(L"Vector2({}, {})", x.GetExpression(), y.GetExpression()) }
		{
			CopyReferences(x, y);
		}

		explicit(false) Vector2Expression(const Point2F& point) noexcept :
			Expression{ std::format(L"Vector2({}, {})", point.x, point.y) }
		{ }

		template <IsExpressionOfType<ExpressionTypes::Vector2> T>
		explicit(false) Vector2Expression(const T& expression) noexcept :
			Expression{ expression.GetExpression() }
		{
			CopyReferences(expression);
		}

		[[nodiscard]] auto X() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.X", GetExpression()), *this };
		}
		[[nodiscard]] auto Y() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Y", GetExpression()), *this };
		}

		// ReSharper disable CppInconsistentNaming

		[[nodiscard]] auto XX() const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"{}.XX", GetExpression()), *this };
		}
		[[nodiscard]] auto XY() const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"{}.XY", GetExpression()), *this };
		}
		[[nodiscard]] auto YX() const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"{}.YX", GetExpression()), *this };
		}
		[[nodiscard]] auto YY() const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"{}.YY", GetExpression()), *this };
		}

		// ReSharper restore CppInconsistentNaming

		[[nodiscard]] auto operator-() const noexcept -> Vector2Expression
		{
			if (GetExpression().starts_with(L"-"))
			{
				return Vector2Expression{ GetExpression().substr(1), *this };
			}
			return Vector2Expression{ std::format(L"(-{})", GetExpression()), *this };
		}
		[[nodiscard]] auto operator+(const Vector2Expression& other) const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"({} + {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator-(const Vector2Expression& other) const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"({} - {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] friend auto operator*(const ScalarExpression& scalar, const Vector2Expression& vector) noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"({} * {})", scalar.GetExpression(), vector.GetExpression()), scalar, vector };
		}
		[[nodiscard]] auto operator*(const ScalarExpression& scalar) const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"({} * {})", GetExpression(), scalar.GetExpression()), *this, scalar };
		}
		[[nodiscard]] auto operator*(const Vector2Expression& other) const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"({} * {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator/(const ScalarExpression& scalar) const noexcept -> Vector2Expression
		{
			return Vector2Expression{ std::format(L"({} * (1 / {}))", GetExpression(), scalar.GetExpression()), *this, scalar };
		}
		[[nodiscard]] auto operator==(const Vector2Expression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::Equals(*this, other);
		}
		[[nodiscard]] auto operator!=(const Vector2Expression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::NotEquals(*this, other);
		}

		private:
		explicit Vector2Expression(const std::wstring_view expression, const auto&... operands) noexcept :
			Expression{ expression }
		{
			CopyReferences(operands...);
		}
	};

	auto Abs(const Vector2Expression& value) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"abs({})", value.GetExpression()), value };
	}
	auto Clamp(const Vector2Expression& value, const Vector2Expression& min, const Vector2Expression& max) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"clamp({}, {}, {})",
			value.GetExpression(), min.GetExpression(), max.GetExpression()),
			value, min, max };
	}
	auto Max(const Vector2Expression& value1, const Vector2Expression& value2) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"max({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Min(const Vector2Expression& value1, const Vector2Expression& value2) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"min({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Lerp(const Vector2Expression& value1, const Vector2Expression& value2, const ScalarExpression& progress) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"lerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}
	auto Mod(const Vector2Expression& value1, const Vector2Expression& value2) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"mod({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Scale(const Vector2Expression& value, const ScalarExpression& factor) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"scale({}, {})",
			value.GetExpression(), factor.GetExpression()), value, factor };
	}
	auto Length(const Vector2Expression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"length({})", value.GetExpression()), value };
	}
	auto LengthSquared(const Vector2Expression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"lengthsquared({})", value.GetExpression()), value };
	}
	auto Distance(const Vector2Expression& value1, const Vector2Expression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"distance({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto DistanceSquared(const Vector2Expression& value1, const Vector2Expression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"distancesquared({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Normalize(const Vector2Expression& value) noexcept -> Vector2Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"normalize({})", value.GetExpression()), value };
	}
}

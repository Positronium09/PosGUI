module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Vector3;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.Wrapper;
import :Bool;
import :Scalar;
import :Vector2;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	struct Vector3Expression : Expression<ExpressionTypes::Vector3>
	{
		Vector3Expression(const ScalarExpression& x, const ScalarExpression& y, const ScalarExpression& z) noexcept :
			Expression{ std::format(L"Vector3({}, {}, {})", x.GetExpression(), y.GetExpression(), z.GetExpression()) }
		{
			CopyReferences(x, y, z);
		}

		explicit(false) Vector3Expression(const Point3F& point) noexcept :
			Expression{ std::format(L"Vector3({}, {}, {})", point.x, point.y, point.z) }
		{ }

		template <IsExpressionOfType<ExpressionTypes::Vector3> T>
		explicit(false) Vector3Expression(const T& expression) noexcept :
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
		[[nodiscard]] auto Z() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Z", GetExpression()), *this };
		}

		// ReSharper disable CppInconsistentNaming

		[[nodiscard]] auto XX() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.XX", GetExpression()), *this };
		}
		[[nodiscard]] auto XY() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.XY", GetExpression()), *this };
		}
		[[nodiscard]] auto XZ() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.XZ", GetExpression()), *this };
		}
		[[nodiscard]] auto YX() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.YX", GetExpression()), *this };
		}
		[[nodiscard]] auto YY() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.YY", GetExpression()), *this };
		}
		[[nodiscard]] auto YZ() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.YZ", GetExpression()), *this };
		}
		[[nodiscard]] auto ZX() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.ZX", GetExpression()), *this };
		}
		[[nodiscard]] auto ZY() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.ZY", GetExpression()), *this };
		}
		[[nodiscard]] auto ZZ() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.ZZ", GetExpression()), *this };
		}

		[[nodiscard]] auto XXX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XXX", GetExpression()), *this };
		}
		[[nodiscard]] auto XXY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XXY", GetExpression()), *this };
		}
		[[nodiscard]] auto XXZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XXZ", GetExpression()), *this };
		}
		[[nodiscard]] auto XYX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XYX", GetExpression()), *this };
		}
		[[nodiscard]] auto XYY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XYY", GetExpression()), *this };
		}
		[[nodiscard]] auto XYZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XYZ", GetExpression()), *this };
		}
		[[nodiscard]] auto XZX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XZX", GetExpression()), *this };
		}
		[[nodiscard]] auto XZY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XZY", GetExpression()), *this };
		}
		[[nodiscard]] auto XZZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.XZZ", GetExpression()), *this };
		}
		[[nodiscard]] auto YXX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YXX", GetExpression()), *this };
		}
		[[nodiscard]] auto YXY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YXY", GetExpression()), *this };
		}
		[[nodiscard]] auto YXZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YXZ", GetExpression()), *this };
		}
		[[nodiscard]] auto YYX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YYX", GetExpression()), *this };
		}
		[[nodiscard]] auto YYY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YYY", GetExpression()), *this };
		}
		[[nodiscard]] auto YYZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YYZ", GetExpression()), *this };
		}
		[[nodiscard]] auto YZX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YZX", GetExpression()), *this };
		}
		[[nodiscard]] auto YZY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YZY", GetExpression()), *this };
		}
		[[nodiscard]] auto YZZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.YZZ", GetExpression()), *this };
		}
		[[nodiscard]] auto ZXX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZXX", GetExpression()), *this };
		}
		[[nodiscard]] auto ZXY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZXY", GetExpression()), *this };
		}
		[[nodiscard]] auto ZXZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZXZ", GetExpression()), *this };
		}
		[[nodiscard]] auto ZYX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZYX", GetExpression()), *this };
		}
		[[nodiscard]] auto ZYY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZYY", GetExpression()), *this };
		}
		[[nodiscard]] auto ZYZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZYZ", GetExpression()), *this };
		}
		[[nodiscard]] auto ZZX() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZZX", GetExpression()), *this };
		}
		[[nodiscard]] auto ZZY() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZZY", GetExpression()), *this };
		}
		[[nodiscard]] auto ZZZ() const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"{}.ZZZ", GetExpression()), *this };
		}

		// ReSharper restore CppInconsistentNaming

		[[nodiscard]] auto operator-() const noexcept -> Vector3Expression
		{
			if (GetExpression().starts_with(L"-"))
			{
				return Vector3Expression{ GetExpression().substr(1), *this };
			}
			return Vector3Expression{ std::format(L"(-{})", GetExpression()), *this };
		}
		[[nodiscard]] auto operator+(const Vector3Expression& other) const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"({} + {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator-(const Vector3Expression& other) const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"({} - {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] friend auto operator*(const ScalarExpression& scalar, const Vector3Expression& vector) noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"({} * {})", scalar.GetExpression(), vector.GetExpression()), scalar, vector };
		}
		[[nodiscard]] auto operator*(const ScalarExpression& scalar) const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"({} * {})", GetExpression(), scalar.GetExpression()), *this, scalar };
		}
		[[nodiscard]] auto operator*(const Vector3Expression& other) const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"({} * {})", GetExpression(), other.GetExpression()), *this, other };
		}
		[[nodiscard]] auto operator/(const ScalarExpression& scalar) const noexcept -> Vector3Expression
		{
			return Vector3Expression{ std::format(L"({} * (1 / {}))", GetExpression(), scalar.GetExpression()), *this, scalar };
		}
		[[nodiscard]] auto operator==(const Vector3Expression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::Equals(*this, other);
		}
		[[nodiscard]] auto operator!=(const Vector3Expression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::NotEquals(*this, other);
		}

		private:
		explicit Vector3Expression(const std::wstring_view expression, const auto&... operands) noexcept :
			Expression{ expression }
		{
			CopyReferences(operands...);
		}
	};

	auto Abs(const Vector3Expression& value) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"abs({})", value.GetExpression()), value };
	}
	auto Clamp(const Vector3Expression& value, const Vector3Expression& min, const Vector3Expression& max) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"clamp({}, {}, {})",
			value.GetExpression(), min.GetExpression(), max.GetExpression()),
			value, min, max };
	}
	auto Max(const Vector3Expression& value1, const Vector3Expression& value2) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"max({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Min(const Vector3Expression& value1, const Vector3Expression& value2) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"min({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Lerp(const Vector3Expression& value1, const Vector3Expression& value2, const ScalarExpression& progress) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"lerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}
	auto Mod(const Vector3Expression& value1, const Vector3Expression& value2) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"mod({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Scale(const Vector3Expression& value, const ScalarExpression& factor) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"scale({}, {})",
			value.GetExpression(), factor.GetExpression()), value, factor };
	}
	auto Length(const Vector3Expression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"length({})", value.GetExpression()), value };
	}
	auto LengthSquared(const Vector3Expression& value) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"lengthsquared({})", value.GetExpression()), value };
	}
	auto Distance(const Vector3Expression& value1, const Vector3Expression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"distance({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto DistanceSquared(const Vector3Expression& value1, const Vector3Expression& value2) noexcept -> ScalarExpression
	{
		return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"distancesquared({}, {})",
			value1.GetExpression(), value2.GetExpression()), value1, value2 };
	}
	auto Normalize(const Vector3Expression& value) noexcept -> Vector3Expression
	{
		return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"normalize({})", value.GetExpression()), value };
	}
}

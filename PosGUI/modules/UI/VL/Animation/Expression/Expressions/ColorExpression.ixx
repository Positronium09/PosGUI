module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Color;

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
	struct ColorExpression : Expression<ExpressionTypes::Color>
	{
		explicit(false) ColorExpression(const RGBA& color) noexcept :
			Expression{ std::format(L"ColorRgb({}, {}, {}, {})",
				color.a * 255.0F, color.r * 255.0F, color.g * 255.0F, color.b * 255.0F) }
		{ }

		template <IsExpressionOfType<ExpressionTypes::Color> T>
		explicit(false) ColorExpression(const T& expression) noexcept :
			Expression{ expression.GetExpression() }
		{
			CopyReferences(expression);
		}

		[[nodiscard]] auto operator==(const ColorExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::Equals(*this, other);
		}
		[[nodiscard]] auto operator!=(const ColorExpression& other) const noexcept -> BoolExpression
		{
			return BoolExpression::NotEquals(*this, other);
		}

		private:
		explicit ColorExpression(const std::wstring_view expression, const auto&... operands) noexcept :
			Expression{ expression }
		{
			CopyReferences(operands...);
		}
	};

	auto ColorRgb(
		const ScalarExpression& alpha, const ScalarExpression& red,
		const ScalarExpression& green, const ScalarExpression& blue) noexcept -> ColorExpression
	{
		return StubDetail::Stub<ExpressionTypes::Color>{ std::format(L"colorrgb({}, {}, {}, {})",
			alpha.GetExpression(), red.GetExpression(), green.GetExpression(), blue.GetExpression()),
			alpha, red, green, blue };
	}
	auto ColorHsl(const ScalarExpression& hue, const ScalarExpression& saturation, const ScalarExpression& luminosity) noexcept -> ColorExpression
	{
		return StubDetail::Stub<ExpressionTypes::Color>{ std::format(L"colorhsl({}, {}, {})",
			hue.GetExpression(), saturation.GetExpression(), luminosity.GetExpression()),
			hue, saturation, luminosity };
	}
	auto ColorLerp(const ColorExpression& value1, const ColorExpression& value2, const ScalarExpression& progress) noexcept -> ColorExpression
	{
		return StubDetail::Stub<ExpressionTypes::Color>{ std::format(L"colorlerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}
	auto ColorLerpRgb(const ColorExpression& value1, const ColorExpression& value2, const ScalarExpression& progress) noexcept -> ColorExpression
	{
		return StubDetail::Stub<ExpressionTypes::Color>{ std::format(L"colorrgblerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}
	auto ColorLerpHsl(const ColorExpression& value1, const ColorExpression& value2, const ScalarExpression& progress) noexcept -> ColorExpression
	{
		return StubDetail::Stub<ExpressionTypes::Color>{ std::format(L"colorhsllerp({}, {}, {})",
			value1.GetExpression(), value2.GetExpression(), progress.GetExpression()),
			value1, value2, progress };
	}
}

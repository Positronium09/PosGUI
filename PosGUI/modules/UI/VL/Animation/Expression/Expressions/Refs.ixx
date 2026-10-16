module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Refs;

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
import :Vector3;
import :Color;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	using ExpressionClassMap = TypeMap<
		TypePair<ExpressionTypes::Bool, BoolExpression>,
		TypePair<ExpressionTypes::Scalar, ScalarExpression>,
		TypePair<ExpressionTypes::Vector2, Vector2Expression>,
		TypePair<ExpressionTypes::Vector3, Vector3Expression>,
		TypePair<ExpressionTypes::Color, ColorExpression>>;

	template <WStringLiteral Name, typename T> requires
		(!WrapperType<T>) &&
		ExpressionClassMap::Keys::Contains<ExpressionTypes::ExpressionTypeOf<T>>
		struct ExpressionRef<Name, T> : ExpressionClassMap::GetMappedType<ExpressionTypes::ExpressionTypeOf<T>>
	{
		explicit ExpressionRef(const T& value) noexcept :
			ExpressionClassMap::GetMappedType<ExpressionTypes::ExpressionTypeOf<T>>{ Name.data }
		{
			this->AddReference(Name.data, static_cast<ExpressionTypes::NativeTypeOf<ExpressionTypes::ExpressionTypeOf<T>>>(value));
		}
	};
}

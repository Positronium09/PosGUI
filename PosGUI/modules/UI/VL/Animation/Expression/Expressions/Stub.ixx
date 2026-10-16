module;
export module PGUI.UI.VL.Expression.Expressions:Stub;

import std;

import PGUI.UI.VL.Expression;

export namespace PGUI::UI::VL::Expression::StubDetail
{
	template <ExpressionTypes::ExpressionType T>
	struct Stub : Expression<T>
	{
		explicit(false) Stub(const std::wstring_view expression, const auto&... operands) noexcept :
			Expression<T>{ expression }
		{
			this->CopyReferences(operands...);
		}
	};
}

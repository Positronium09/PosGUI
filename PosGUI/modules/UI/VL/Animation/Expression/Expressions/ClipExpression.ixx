module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Clip;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.Clip;
import PGUI.Wrapper;
import :Scalar;
import :Vector2;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	template <WStringLiteral Name, WrapperDerivedFrom<WUC::CompositionClip> ClipT>
	struct ExpressionRef<Name, ClipT> : Expression<ExpressionTypes::ExpressionTypeOf<ClipT>>
	{
		explicit ExpressionRef(ClipT object) noexcept :
			Expression<ExpressionTypes::ExpressionTypeOf<ClipT>>{ Name.data },
			object{ object }
		{
			this->AddReference(Name.data, Wrapper<WUC::CompositionObject>{ object });
		}
		[[nodiscard]] const auto& GetObject() const noexcept
		{
			return object;
		}

		[[nodiscard]] auto AnchorPoint() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.AnchorPoint", Name.data), *this };
		}
		[[nodiscard]] auto CenterPoint() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.CenterPoint", Name.data), *this };
		}
		[[nodiscard]] auto Offset() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.Offset", Name.data), *this };
		}
		[[nodiscard]] auto RotationAngle() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RotationAngle", Name.data), *this };
		}
		[[nodiscard]] auto RotationAngleInDegrees() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RotationAngleInDegrees", Name.data), *this };
		}
		[[nodiscard]] auto Scale() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.Scale", Name.data), *this };
		}

		//TODO TransformMatrix Matrix3x2

		[[nodiscard]] auto BottomInset() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::InsetClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.BottomInset", Name.data), *this };
		}
		[[nodiscard]] auto LeftInset() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::InsetClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.LeftInset", Name.data), *this };
		}
		[[nodiscard]] auto RightInset() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::InsetClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RightInset", Name.data), *this };
		}
		[[nodiscard]] auto TopInset() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::InsetClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.TopInset", Name.data), *this };
		}

		[[nodiscard]] auto Bottom() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Bottom", Name.data), *this };
		}
		[[nodiscard]] auto BottomLeftRadius() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.BottomLeftRadius", Name.data), *this };
		}
		[[nodiscard]] auto BottomRightRadius() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.BottomRightRadius", Name.data), *this };
		}
		[[nodiscard]] auto Left() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Left", Name.data), *this };
		}
		[[nodiscard]] auto Right() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Right", Name.data), *this };
		}
		[[nodiscard]] auto Top() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Top", Name.data), *this };
		}
		[[nodiscard]] auto TopLeftRadius() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.TopLeftRadius", Name.data), *this };
		}
		[[nodiscard]] auto TopRightRadius() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<ClipT, WUC::RectangleClip>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.TopRightRadius", Name.data), *this };
		}

		private:
		ClipT object;
	};
}

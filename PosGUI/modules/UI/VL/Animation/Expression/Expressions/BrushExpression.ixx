module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Brush;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.Brush;
import PGUI.Wrapper;
import :Scalar;
import :Vector2;
import :Color;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	template <WStringLiteral Name, WrapperDerivedFrom<WUC::CompositionBrush> BrushT>
	struct ExpressionRef<Name, BrushT> : Expression<ExpressionTypes::ExpressionTypeOf<BrushT>>
	{
		explicit ExpressionRef(BrushT object) noexcept :
			Expression<ExpressionTypes::ExpressionTypeOf<BrushT>>{ Name.data },
			object{ object }
		{
			this->AddReference(Name.data, Wrapper<WUC::CompositionObject>{ object });
		}
		[[nodiscard]] const auto& GetObject() const noexcept
		{
			return object;
		}

		[[nodiscard]] auto Color() const noexcept -> ColorExpression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionColorBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Color>{ std::format(L"{}.Color", Name.data), *this };
		}

		[[nodiscard]] auto AnchorPoint() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.AnchorPoint", Name.data), *this };
		}
		[[nodiscard]] auto CenterPoint() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.CenterPoint", Name.data), *this };
		}
		[[nodiscard]] auto HorizontalAlignmentRatio() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.HorizontalAlignmentRatio", Name.data), *this };
		}
		[[nodiscard]] auto Offset() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.Offset", Name.data), *this };
		}
		[[nodiscard]] auto RotationAngle() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RotationAngle", Name.data), *this };
		}
		[[nodiscard]] auto RotationAngleInDegrees() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RotationAngleInDegrees", Name.data), *this };
		}
		[[nodiscard]] auto Scale() const noexcept -> Vector2Expression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.Scale", Name.data), *this };
		}

		//TODO TransformMatrix Matrix3x2

		[[nodiscard]] auto VerticalAlignmentRatio() const noexcept -> ScalarExpression
			requires WrapperDerivedFrom<BrushT, WUC::CompositionSurfaceBrush>
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.VerticalAlignmentRatio", Name.data), *this };
		}

		//TODO CompositionNineGridBrush

		//TODO CompositionGradientBrush

		//TODO CompositionLinearGradientBrush

		//TODO CompositionRadialGradientBrush

		//TODO CompositionEffectBrush

		private:
		BrushT object;
	};
}

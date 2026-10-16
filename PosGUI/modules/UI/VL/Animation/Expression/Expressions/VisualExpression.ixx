module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression.Expressions:Visual;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.Visual;
import PGUI.Wrapper;
import :Bool;
import :Scalar;
import :Vector3;
import :Vector2;
import :Stub;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	template <WStringLiteral Name, WrapperDerivedFrom<WUC::Visual> VisualT>
	struct ExpressionRef<Name, VisualT> : Expression<ExpressionTypes::ExpressionTypeOf<VisualT>>
	{
		explicit ExpressionRef(VisualT object) noexcept :
			Expression<ExpressionTypes::ExpressionTypeOf<VisualT>>{ Name.data },
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
		[[nodiscard]] auto CenterPoint() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.CenterPoint", Name.data), *this };
		}
		[[nodiscard]] auto IsVisible() const noexcept -> BoolExpression
		{
			return StubDetail::Stub<ExpressionTypes::Bool>{ std::format(L"{}.IsVisible", Name.data), *this };
		}
		[[nodiscard]] auto Offset() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.Offset", Name.data), *this };
		}
		[[nodiscard]] auto Position() const noexcept
		{
			return Offset();
		}
		[[nodiscard]] auto Opacity() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.Opacity", Name.data), *this };
		}

		//TODO Orientation Quaternion

		[[nodiscard]] auto RelativeOffsetAdjustment() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.RelativeOffset", Name.data), *this };
		}
		[[nodiscard]] auto RelativePositionAdjustment() const noexcept -> Vector3Expression
		{
			return RelativeOffsetAdjustment();
		}
		[[nodiscard]] auto RelativeSizeAdjustment() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.RelativeSize", Name.data), *this };
		}
		[[nodiscard]] auto RotationAngle() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RotationAngle", Name.data), *this };
		}
		[[nodiscard]] auto RotationAngleInDegrees() const noexcept -> ScalarExpression
		{
			return StubDetail::Stub<ExpressionTypes::Scalar>{ std::format(L"{}.RotationAngleInDegrees", Name.data), *this };
		}
		[[nodiscard]] auto RotationAxis() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.RotationAxis", Name.data), *this };
		}
		[[nodiscard]] auto Scale() const noexcept -> Vector3Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector3>{ std::format(L"{}.Scale", Name.data), *this };
		}
		[[nodiscard]] auto Size() const noexcept -> Vector2Expression
		{
			return StubDetail::Stub<ExpressionTypes::Vector2>{ std::format(L"{}.Size", Name.data), *this };
		}

		//TODO TransformMatrix Matrix4x4

		private:
		VisualT object;
	};
}

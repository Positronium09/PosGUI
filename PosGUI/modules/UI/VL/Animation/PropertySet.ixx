module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.h>

#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.UI.VL.PropertySet;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.Expression.Expressions;
import PGUI.UI.VL.VLEnums;

namespace WU = winrt::Windows::UI;
namespace WUC = winrt::Windows::UI::Composition;
namespace WFN = winrt::Windows::Foundation::Numerics;

namespace PGUI::UI::VL::PropertySetDetail
{
	namespace ET = Expression::ExpressionTypes;

	using ProjectedPropertyTypeMap = TypeMap<
		TypePair<ET::Bool, bool>,
		TypePair<ET::Scalar, float>,
		TypePair<ET::Vector2, WFN::float2>,
		TypePair<ET::Vector3, WFN::float3>,
		//TypePair<ET::Vector4, WFN::float4>,
		TypePair<ET::Matrix3X2, WFN::float3x2>,
		TypePair<ET::Matrix4X4, WFN::float4x4>,
		TypePair<ET::Quaternion, WFN::quaternion>,
		TypePair<ET::Color, WU::Color>>;

	template <typename T>
	concept PropertyValueType = ProjectedPropertyTypeMap::Keys::Contains<T>;

	template <PropertyValueType T>
	using ProjectedPropertyType = ProjectedPropertyTypeMap::GetMappedType<T>;

	[[nodiscard]] auto MapStatus(const GetValueStatus status) noexcept -> ErrorCode
	{
		switch (status)
		{
			case GetValueStatus::Succeeded:
			{
				return ErrorCode::Success;
			}
			case GetValueStatus::NotFound:
			{
				return ErrorCode::NotFound;
			}
			case GetValueStatus::TypeMismatch:
			{
				return ErrorCode::InvalidCast;
			}
			default:
			{
				return ErrorCode::Failure;
			}
		}
	}

	template <PropertyValueType T>
	[[nodiscard]] auto InsertProperty(
		const WUC::CompositionPropertySet& propertySet,
		const std::wstring_view propertyName,
		const ET::NativeTypeOf<T>& value) noexcept -> Result<void>
	{
		return CallProjected([&propertySet, propertyName, &value]
		{
			if constexpr (std::same_as<T, ET::Bool>)
			{
				propertySet.InsertBoolean(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Scalar>)
			{
				propertySet.InsertScalar(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Vector2>)
			{
				propertySet.InsertVector2(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Vector3>)
			{
				propertySet.InsertVector3(propertyName, value);
			}
			/*
			else if constexpr (std::same_as<T, ET::Vector4>)
			{
				propertySet.InsertVector4(propertyName, value);
			}
			*/
			else if constexpr (std::same_as<T, ET::Matrix3X2>)
			{
				propertySet.InsertMatrix3x2(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Matrix4X4>)
			{
				propertySet.InsertMatrix4x4(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Quaternion>)
			{
				propertySet.InsertQuaternion(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Color>)
			{
				propertySet.InsertColor(propertyName, value);
			}
			else
			{
				static_assert(false, "Invalid Type for InsertProperty");
			}
		});
	}

	template <PropertyValueType T>
	[[nodiscard]] auto TryGetProperty(
		const WUC::CompositionPropertySet& propertySet,
		const std::wstring_view propertyName) noexcept -> Result<ET::NativeTypeOf<T>>
	{
		ProjectedPropertyType<T> value;

		const auto statusResult = CallProjected([&propertySet, propertyName, &value]
		{
			if constexpr (std::same_as<T, ET::Bool>)
			{
				return propertySet.TryGetBoolean(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Scalar>)
			{
				return propertySet.TryGetScalar(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Vector2>)
			{
				return propertySet.TryGetVector2(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Vector3>)
			{
				return propertySet.TryGetVector3(propertyName, value);
			}
			/*
			else if constexpr (std::same_as<T, ET::Vector4>)
			{
				return propertySet.TryGetVector4(propertyName, value);
			}
			*/
			else if constexpr (std::same_as<T, ET::Matrix3X2>)
			{
				return propertySet.TryGetMatrix3x2(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Matrix4X4>)
			{
				return propertySet.TryGetMatrix4x4(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Quaternion>)
			{
				return propertySet.TryGetQuaternion(propertyName, value);
			}
			else if constexpr (std::same_as<T, ET::Color>)
			{
				return propertySet.TryGetColor(propertyName, value);
			}
			else
			{
				static_assert(false, "Invalid Type for TryGetProperty");
			}
		});
		if (!statusResult.has_value())
		{
			return Unexpected{ statusResult.error() };
		}

		if (const auto status = statusResult.value();
			status != GetValueStatus::Succeeded)
		{
			return Unexpected{ Error{
				MapStatus(status)
			}.AddDetail(L"propertyName", propertyName) };
		}

		return ET::NativeTypeOf<T>{ value };
	}
}

export namespace PGUI::UI::VL
{
	template <WStringLiteral PropertyName, Expression::ExpressionTypes::ExpressionType T>
		requires (!std::same_as<T, Expression::ExpressionTypes::Ref> &&
				 !std::same_as<T, Expression::ExpressionTypes::ObjectRef>)
	struct PropertyItem
	{
		using ValueType = T;
		static constexpr std::wstring_view name = PropertyName.data;
	};

	template <IsSpecialization<TypeList> PropertyList> requires (!PropertyList::IsEmpty)
	class PropertySet : public CompositionObjectT<WUC::CompositionPropertySet>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] static auto Create() noexcept -> Result<PropertySet>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			const auto propertySetResult = CallProjected([&compositor]
			{
				return PropertySet{ compositor.CreatePropertySet() };
			});
			if (!propertySetResult)
			{
				return Unexpected{ propertySetResult.error() };
			}

			const auto& propertySet = propertySetResult.value();
			const auto fill = [&propertySet]<typename... Ps>(TypeList<Ps...>) noexcept -> Result<void>
			{
				auto insertResult = Result{ EmptyResult };
				const auto insert = [&propertySet, &insertResult]<typename P>(std::type_identity<P>)
				{
					insertResult = propertySet.template Insert<P>(
						Expression::ExpressionTypes::NativeTypeOf<typename P::ValueType>{ });
					return insertResult.has_value();
				};
				if (!((insert(std::type_identity<Ps>{ })) && ...))
				{
					return Unexpected{ insertResult.error() };
				}

				return EmptyResult;
			};
			if (const auto fillResult = fill(PropertyList{ });
				!fillResult.has_value())
			{
				return Unexpected{ fillResult.error() };
			}

			return propertySet;
		}

		template <typename P> requires PropertyList::template Contains<P>
		[[nodiscard]] auto Insert(
			const Expression::ExpressionTypes::NativeTypeOf<typename P::ValueType>& val) const noexcept -> Result<void>
		{
			return PropertySetDetail::InsertProperty<typename P::ValueType>(this->Get(), P::name, val);
		}

		template <typename P> requires PropertyList::template Contains<P>
		[[nodiscard]] auto TryGet() const noexcept ->
			Result<Expression::ExpressionTypes::NativeTypeOf<typename P::ValueType>>
		{
			return PropertySetDetail::TryGetProperty<typename P::ValueType>(this->Get(), P::name);
		}

		template <typename P> requires PropertyList::template Contains<P>
		[[nodiscard]] auto GetExpression() const noexcept
		{
			using ValueType = P::ValueType;
			using namespace Expression::ExpressionTypes;

			if constexpr (std::same_as<ValueType, Bool>)
			{
				return Expression::BoolExpression{ Expression::StubDetail::Stub<Bool>{ P::name } };
			}
			else if constexpr (std::same_as<ValueType, Scalar>)
			{
				return Expression::ScalarExpression{ Expression::StubDetail::Stub<Scalar>{ P::name } };
			}
			else if constexpr (std::same_as<ValueType, Vector2>)
			{
				return Expression::Vector2Expression{ Expression::StubDetail::Stub<Vector2>{ P::name } };
			}
			else if constexpr (std::same_as<ValueType, Vector3>)
			{
				return Expression::Vector3Expression{ Expression::StubDetail::Stub<Vector3>{ P::name } };
			}
			/*
			else if constexpr (std::same_as<ValueType, Vector4>)
			{
				return Expression::Vector4Expression{ Expression::StubDetail::Stub<Vector4>{ P::name } };
			}
			else if constexpr (std::same_as<ValueType, Matrix3X2>)
			{
				return Expression::Matrix3X2Expression{ Expression::StubDetail::Stub<Matrix3X2>{ P::name } };
			}
			else if constexpr (std::same_as<ValueType, Matrix4X4>)
			{
				return Expression::Matrix4X4Expression{ Expression::StubDetail::Stub<Matrix4X4>{ P::name } };
			}
			else if constexpr (std::same_as<ValueType, Quaternion>)
			{
				return Expression::QuaternionExpression{ Expression::StubDetail::Stub<Quaternion>{ P::name} };
			}
			*/
			else if constexpr (std::same_as<ValueType, Color>)
			{
				return Expression::ColorExpression{ Expression::StubDetail::Stub<Color>{ P::name } };
			}
			else
			{
				static_assert(false, "Invalid Type for PropertySet::GetExpression");
			}
		}
	};

	class AnyPropertySet : public CompositionObjectT<WUC::CompositionPropertySet>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		template <IsSpecialization<TypeList> PropertyList>
		explicit(false) AnyPropertySet(const PropertySet<PropertyList>& propertySet) noexcept :
			CompositionObjectT{ propertySet }
		{ }

		[[nodiscard]] static auto Create() noexcept -> Result<AnyPropertySet>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return AnyPropertySet{ compositor.CreatePropertySet() };
			});
		}

		template <Expression::ExpressionTypes::ExpressionType T>
		[[nodiscard]] auto Insert(
			const std::wstring_view propertyName,
			const Expression::ExpressionTypes::NativeTypeOf<T>& val) const noexcept -> Result<void>
		{
			return PropertySetDetail::InsertProperty<T>(this->Get(), propertyName, val);
		}

		template <Expression::ExpressionTypes::ExpressionType T>
		[[nodiscard]] auto TryGet(const std::wstring_view propertyName) const noexcept ->
			Result<Expression::ExpressionTypes::NativeTypeOf<T>>
		{
			return PropertySetDetail::TryGetProperty<T>(this->Get(), propertyName);
		}

		template <IsSpecialization<TypeList> PropertyList>
		[[nodiscard]] explicit operator PropertySet<PropertyList>() const noexcept
		{
			return PropertySet<PropertyList>{ this->Get() };
		}
	};
}

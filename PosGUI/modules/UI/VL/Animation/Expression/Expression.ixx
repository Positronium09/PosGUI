module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Expression;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.Wrapper;
import PGUI.ProjectionHolder;
import PGUI.UI.VL.Compositor;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL::Expression
{
	namespace ExpressionTypes
	{
		struct Ref {};
		struct ObjectRef {};
		struct Bool {};
		struct Scalar {};
		struct Vector2 {};
		struct Vector3 {};
		struct Vector4 {};
		struct Matrix3X2 {};
		struct Matrix4X4 {};
		struct Quaternion {};
		struct Color {};

		using ExpressionTypeList = TypeList<
			Ref, ObjectRef, Bool,
			Scalar, Vector2, Vector3, Vector4,
			Matrix3X2, Matrix4X4, Quaternion, Color>;

		template <typename T>
		concept ExpressionType = ExpressionTypeList::Contains<T>;

		using ExpressionToNativeTypeMap = TypeMap<
			TypePair<Ref, Wrapper<WUC::IAnimationObject>>,
			TypePair<ObjectRef, Wrapper<WUC::CompositionObject>>,
			TypePair<Bool, bool>,
			TypePair<Scalar, float>,
			TypePair<Vector2, Point2F>,
			TypePair<Vector3, Point3F>,
			//TypePair<Vector4, WUC::Vector4>,
			TypePair<Matrix3X2, Matrix3x2>,
			TypePair<Matrix4X4, Matrix4x4>,
			TypePair<Quaternion, PGUI::Quaternion>,
			TypePair<Color, RGBA>>;

		using NativeToExpressionTypeMap = TypeMap<
			TypePair<Wrapper<WUC::IAnimationObject>, Ref>,
			TypePair<Wrapper<WUC::CompositionObject>, ObjectRef>,
			TypePair<bool, Bool>,
			TypePair<float, Scalar>,
			TypePair<Point2F, Vector2>,
			TypePair<Point3F, Vector3>,
			//TypePair<WUC::Vector4, Vector4>,
			TypePair<Matrix3x2, Matrix3X2>,
			TypePair<Matrix4x4, Matrix4X4>,
			TypePair<PGUI::Quaternion, Quaternion>,
			TypePair<RGBA, Color>>;
	}
}

namespace PGUI::UI::VL::Expression::ExpressionTypes::ExpressionDetail
{
	template <typename T>
	struct ExpressionTypeOf {};

	template <typename T> requires NativeToExpressionTypeMap::Keys::Contains<T>
	struct ExpressionTypeOf<T>
	{
		using Type = NativeToExpressionTypeMap::GetMappedType<T>;
	};

	template <typename T> requires
		(!NativeToExpressionTypeMap::Keys::Contains<T>) &&
		std::is_arithmetic_v<T> && 
			(!IsInTypeList<T, char, wchar_t, char8_t, char16_t, char32_t>)
	struct ExpressionTypeOf<T>
	{
		using Type = Scalar;
	};

	template <typename T> requires
		(!NativeToExpressionTypeMap::Keys::Contains<T>) &&
		WrapperDerivedFrom<T, WUC::CompositionObject>
		struct ExpressionTypeOf<T>
	{
		using Type = ObjectRef;
	};

	template <typename T> requires
		(!NativeToExpressionTypeMap::Keys::Contains<T>) &&
		(!WrapperDerivedFrom<T, WUC::CompositionObject>) &&
		WrapperDerivedFrom<T, WUC::IAnimationObject>
		struct ExpressionTypeOf<T>
	{
		using Type = Ref;
	};
}

export namespace PGUI::UI::VL::Expression
{
	namespace ExpressionTypes
	{
		template <typename T>
		using ExpressionTypeOf = ExpressionDetail::ExpressionTypeOf<T>::Type;

		template <ExpressionType T>
		using NativeTypeOf = ExpressionToNativeTypeMap::GetMappedType<T>;
	}

	using ExpressionReferenceTypeVariant = ExpressionTypes::NativeToExpressionTypeMap::Keys::Rebind<std::variant>;

	template <ExpressionTypes::ExpressionType T>
	struct Expression
	{
		using NativeType = ExpressionTypes::NativeTypeOf<T>;
		using ExpressionTag = T;

		auto CopyReferences(const auto&... others) noexcept
		{
			(references.insert_range(others.GetReferences()), ...);
		}

		[[nodiscard]] const auto& GetExpression() const noexcept
		{
			return expression;
		}

		template <typename Self>
		[[nodiscard]] auto&& GetReferences(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.references);
		}

		[[nodiscard]] auto BindReferences(const WUC::CompositionAnimation& animation) const noexcept -> Result<void>
		{
			return CallProjected([this, &animation]
			{
				for (const auto& [name, value] : references)
				{
					if (name == L"this.Target")
					{
						continue;
					}

					Match(value,
						[&animation, &name](const Wrapper<WUC::IAnimationObject>& object)
					{
						animation.SetExpressionReferenceParameter(name, object.Get());
					},
						[&animation, &name](const Wrapper<WUC::CompositionObject>& object)
					{
						animation.SetReferenceParameter(name, object.Get());
					},
						[&animation, &name](const bool boolean)
					{
						animation.SetBooleanParameter(name, boolean);
					},
						[&animation, &name](const float scalar)
					{
						animation.SetScalarParameter(name, scalar);
					},
						[&animation, &name](const Point2F& vector)
					{
						animation.SetVector2Parameter(name, vector);
					},
						[&animation, &name](const Point3F& vector)
					{
						animation.SetVector3Parameter(name, vector);
					},
						[&animation, &name](const Matrix3x2& matrix)
					{
						animation.SetMatrix3x2Parameter(name, matrix);
					},
						[&animation, &name](const Matrix4x4& matrix)
					{
						animation.SetMatrix4x4Parameter(name, matrix);
					},
						[&animation, &name](const PGUI::Quaternion& quaternion)
					{
						animation.SetQuaternionParameter(name, quaternion);
					},
						[&animation, &name](const RGBA& color)
					{
						animation.SetColorParameter(name, color);
					});
				}
			});
		}
		[[nodiscard]] auto BindReferences(const WrapperDerivedFrom<WUC::CompositionAnimation> auto& animation) const noexcept
		{
			return BindReferences(animation.Get());
		}

		protected:
		explicit Expression(const std::wstring_view expr) noexcept : 
			expression{ expr }
		{ }

		auto AddReference(const std::wstring_view key, const ExpressionReferenceTypeVariant& value) noexcept -> Result<void>
		{
			if (references.contains(key.data()))
			{
				return Unexpected{ Error{
					ErrorCode::InvalidArgument
				}.AddDetail(L"key", key.data()) };
			}

			return WrapThrowing([&]
			{
				references.emplace(key.data(), value);
			});
		}

		private:
		std::map<std::wstring, ExpressionReferenceTypeVariant, std::less<>> references;
		std::wstring expression;
	};

	template <typename T>
	concept IsExpression = std::derived_from<T, Expression<typename T::ExpressionTag>>;

	template <typename T, typename U>
	concept IsExpressionOfType = IsExpression<T> && std::same_as<typename T::ExpressionTag, U>;

	template <WStringLiteral Name, typename T>
	struct ExpressionRef
	{
		explicit ExpressionRef(const T&)
		{
			static_assert(false);
		}
	};

	template <typename T>
	using ExpressionTarget = ExpressionRef<L"this.Target", T>;

	template <typename T>
	auto CreateExpressionFor(const T& obj) noexcept
	{
		return ExpressionTarget<T>{ obj };
	}
}

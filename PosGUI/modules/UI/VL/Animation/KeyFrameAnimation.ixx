module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.KeyFrameAnimation;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.CompositionAnimation;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.EasingFunction;
import PGUI.UI.VL.Properties;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.Expression;
import PGUI.UI.VL.Expression.Expressions;

namespace WUC = winrt::Windows::UI::Composition;

namespace PGUI::UI::VL::KeyFrameAnimationDetail
{
	template <Expression::ExpressionTypes::ExpressionType T>
	struct Stub : Expression::Expression<T>
	{
		explicit(false) Stub(const std::wstring_view expression) noexcept :
			Expression::Expression<T>{ expression }
		{ }
	};
}

export namespace PGUI::UI::VL
{
	using KeyFrameTypeMap = TypeMap<
		TypePair<WUC::BooleanKeyFrameAnimation, Expression::ExpressionTypes::Bool>,
		TypePair<WUC::ColorKeyFrameAnimation, Expression::ExpressionTypes::Color>,
		TypePair<WUC::QuaternionKeyFrameAnimation, Expression::ExpressionTypes::Quaternion>,
		TypePair<WUC::ScalarKeyFrameAnimation, Expression::ExpressionTypes::Scalar>,
		TypePair<WUC::Vector2KeyFrameAnimation, Expression::ExpressionTypes::Vector2>,
		TypePair<WUC::Vector3KeyFrameAnimation, Expression::ExpressionTypes::Vector3>>;

	template <typename Projected>
	concept KeyFrameExpressionAvailable =
		KeyFrameTypeMap::Keys::Contains<Projected> &&
		Expression::ExpressionClassMap::Keys::Contains<KeyFrameTypeMap::GetMappedType<Projected>>;

	template <KeyFrameExpressionAvailable Projected>
	using KeyFrameExpressionOf =
		Expression::ExpressionClassMap::GetMappedType<KeyFrameTypeMap::GetMappedType<Projected>>;

	template <HandleDerivedFrom<WUC::KeyFrameAnimation> Projected = WUC::KeyFrameAnimation>
	class KeyFrameAnimationT : public CompositionAnimationT<Projected>
	{
		public:
		using Base = CompositionAnimationT<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetDelayBehavior() const noexcept -> Result<AnimationDelayBehavior>
		{
			return CallProjected([this]
			{
				return this->Get().DelayBehavior();
			});
		}
		[[nodiscard]] auto SetDelayBehavior(const AnimationDelayBehavior delayBehavior) const noexcept -> Result<void>
		{
			return CallProjected([this, delayBehavior]
			{
				this->Get().DelayBehavior(delayBehavior);
			});
		}

		[[nodiscard]] auto GetDelayTime() const noexcept -> Result<std::chrono::milliseconds>
		{
			return CallProjected([this]
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(this->Get().DelayTime());
			});
		}
		[[nodiscard]] auto SetDelayTime(const std::chrono::milliseconds& delayTime) const noexcept -> Result<void>
		{
			return CallProjected([this, &delayTime]
			{
				this->Get().DelayTime(delayTime);
			});
		}

		[[nodiscard]] auto GetDirection() const noexcept -> Result<AnimationDirection>
		{
			return CallProjected([this]
			{
				return this->Get().Direction();
			});
		}
		[[nodiscard]] auto SetDirection(const AnimationDirection direction) const noexcept -> Result<void>
		{
			return CallProjected([this, direction]
			{
				this->Get().Direction(direction);
			});
		}

		[[nodiscard]] auto GetDuration() const noexcept -> Result<std::chrono::milliseconds>
		{
			return CallProjected([this]
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(this->Get().Duration());
			});
		}
		[[nodiscard]] auto SetDuration(const std::chrono::milliseconds& duration) const noexcept -> Result<void>
		{
			return CallProjected([this, &duration]
			{
				this->Get().Duration(duration);
			});
		}

		[[nodiscard]] auto GetIterationBehavior() const noexcept -> Result<AnimationIterationBehavior>
		{
			return CallProjected([this]
			{
				return this->Get().IterationBehavior();
			});
		}
		[[nodiscard]] auto SetIterationBehavior(const AnimationIterationBehavior iterationBehavior) const noexcept -> Result<void>
		{
			return CallProjected([this, iterationBehavior]
			{
				this->Get().IterationBehavior(iterationBehavior);
			});
		}

		[[nodiscard]] auto GetIterationCount() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return this->Get().IterationCount();
			});
		}
		[[nodiscard]] auto SetIterationCount(const std::int32_t iterationCount) const noexcept -> Result<void>
		{
			return CallProjected([this, iterationCount]
			{
				this->Get().IterationCount(iterationCount);
			});
		}

		[[nodiscard]] auto GetKeyFrameCount() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return this->Get().KeyFrameCount();
			});
		}

		[[nodiscard]] auto GetStopBehavior() const noexcept -> Result<AnimationStopBehavior>
		{
			return CallProjected([this]
			{
				return this->Get().StopBehavior();
			});
		}
		[[nodiscard]] auto SetStopBehavior(const AnimationStopBehavior stopBehavior) const noexcept -> Result<void>
		{
			return CallProjected([this, stopBehavior]
			{
				this->Get().StopBehavior(stopBehavior);
			});
		}

		template <Properties::IsProperty P> requires
			KeyFrameTypeMap::Keys::Contains<Projected> &&
			std::same_as<KeyFrameTypeMap::GetMappedType<Projected>, typename P::ValueTag>
		[[nodiscard]] auto SetTarget() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().Target(P::name);
			});
		}

		[[nodiscard]] auto StartingValue() const noexcept
			requires KeyFrameExpressionAvailable<Projected>
		{
			return KeyFrameExpressionOf<Projected>{
				KeyFrameAnimationDetail::Stub<KeyFrameTypeMap::GetMappedType<Projected>>{ L"this.StartingValue" } };
		}
		[[nodiscard]] auto CurrentValue() const noexcept
			requires KeyFrameExpressionAvailable<Projected>
		{
			return KeyFrameExpressionOf<Projected>{
				KeyFrameAnimationDetail::Stub<KeyFrameTypeMap::GetMappedType<Projected>>{ L"this.CurrentValue" } };
		}
		[[nodiscard]] auto FinalValue() const noexcept
			requires KeyFrameExpressionAvailable<Projected>
		{
			return KeyFrameExpressionOf<Projected>{
				KeyFrameAnimationDetail::Stub<KeyFrameTypeMap::GetMappedType<Projected>>{ L"this.FinalValue" } };
		}

		template <typename T> requires
			KeyFrameTypeMap::Keys::Contains<Projected> &&
			Expression::IsExpressionOfType<T, KeyFrameTypeMap::GetMappedType<Projected>>
		[[nodiscard]] auto InsertExpressionKeyFrame(
			const float normalizedProgressKey,
			const T& expression) const noexcept -> Result<void>
		{
			if (const auto result = CallProjected([this, normalizedProgressKey, &expression]
			{
				this->Get().InsertExpressionKeyFrame(normalizedProgressKey, expression.GetExpression());
			}); !result.has_value())
			{
				return result;
			}

			return expression.BindReferences(this->Get());
		}

		template <typename T> requires
			KeyFrameTypeMap::Keys::Contains<Projected>&&
			Expression::IsExpressionOfType<T, KeyFrameTypeMap::GetMappedType<Projected>>
			[[nodiscard]] auto InsertExpressionKeyFrame(
				const float normalizedProgressKey,
				const T& expression,
				const EasingFunction& easingFunction) const noexcept -> Result<void>
		{
			if (const auto result = CallProjected([this, normalizedProgressKey, &expression, &easingFunction]
			{
				this->Get().InsertExpressionKeyFrame(normalizedProgressKey, expression.GetExpression(), easingFunction.Get());
			}); !result.has_value())
			{
				return result;
			}

			return expression.BindReferences(this->Get());
		}
	};
	using KeyFrameAnimation = KeyFrameAnimationT<>;

	class BoolKeyFrameAnimation : public KeyFrameAnimationT<WUC::BooleanKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		using AnimationTag = Expression::ExpressionTypes::Bool;

		[[nodiscard]] static auto Create() noexcept -> Result<BoolKeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return BoolKeyFrameAnimation{ compositor.CreateBooleanKeyFrameAnimation() };
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const bool value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, value]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
	};
	class ColorKeyFrameAnimation : public KeyFrameAnimationT<WUC::ColorKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		using AnimationTag = Expression::ExpressionTypes::Color;

		[[nodiscard]] static auto Create() noexcept -> Result<ColorKeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return ColorKeyFrameAnimation{ compositor.CreateColorKeyFrameAnimation() };
			});
		}

		[[nodiscard]] auto GetInterpolationColorSpace() const noexcept -> Result<ColorSpace>
		{
			return CallProjected([this]
			{
				return Get().InterpolationColorSpace();
			});
		}
		[[nodiscard]] auto SetInterpolationColorSpace(const ColorSpace colorSpace) const noexcept -> Result<void>
		{
			return CallProjected([this, colorSpace]
			{
				Get().InterpolationColorSpace(colorSpace);
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const RGBA& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		[[nodiscard]] auto InsertKeyFrame(
			const float normalizedProgressKey, 
			const RGBA& value, const EasingFunction& easingFunction) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value, &easingFunction]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value, easingFunction.Get());
			});
		}
	};
	class [[deprecated("This API contains a known issue in Windows 10, versions 20H2 and below. Do not use this API in your code")]]
		PathKeyFrameAnimation : public KeyFrameAnimationT<WUC::PathKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;

		/*
		[[nodiscard]] static auto Create() noexcept -> Result<PathKeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return PathKeyFrameAnimation{ compositor.CreatePathKeyFrameAnimation() };
			});
		}
		*/

		//TODO InsertKeyFrame(float normalizedProgressKey, CompositionPath value, CompositionEasingFunction easingFunction) -> void
		//TODO InsertKeyFrame(float normalizedProgressKey, CompositionPath value, CompositionEasingFunction easingFunction) -> void
	};
	class QuaternionKeyFrameAnimation : public KeyFrameAnimationT<WUC::QuaternionKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		using AnimationTag = Expression::ExpressionTypes::Quaternion;

		[[nodiscard]] static auto Create() noexcept -> Result<QuaternionKeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return QuaternionKeyFrameAnimation{ compositor.CreateQuaternionKeyFrameAnimation() };
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const Quaternion& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		[[nodiscard]] auto InsertKeyFrame(
			const float normalizedProgressKey, 
			const Quaternion& value, const EasingFunction& easingFunction) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value, &easingFunction]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value, easingFunction.Get());
			});
		}
	};
	class ScalarKeyFrameAnimation : public KeyFrameAnimationT<WUC::ScalarKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		using AnimationTag = Expression::ExpressionTypes::Scalar;

		[[nodiscard]] static auto Create() noexcept -> Result<ScalarKeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return ScalarKeyFrameAnimation{ compositor.CreateScalarKeyFrameAnimation() };
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const float value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, value]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		[[nodiscard]] auto InsertKeyFrame(
			const float normalizedProgressKey, 
			const float value, const EasingFunction& easingFunction) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, value, &easingFunction]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value, easingFunction.Get());
			});
		}
	};
	class Vector2KeyFrameAnimation : public KeyFrameAnimationT<WUC::Vector2KeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		using AnimationTag = Expression::ExpressionTypes::Vector2;

		[[nodiscard]] static auto Create() noexcept -> Result<Vector2KeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return Vector2KeyFrameAnimation{ compositor.CreateVector2KeyFrameAnimation() };
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const Point2F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		[[nodiscard]] auto InsertKeyFrame(
			const float normalizedProgressKey, 
			const Point2F& value, const EasingFunction& easingFunction) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value, &easingFunction]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value, easingFunction.Get());
			});
		}
	};
	class Vector3KeyFrameAnimation : public KeyFrameAnimationT<WUC::Vector3KeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		using AnimationTag = Expression::ExpressionTypes::Vector3;

		[[nodiscard]] static auto Create() noexcept -> Result<Vector3KeyFrameAnimation>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return Vector3KeyFrameAnimation{ compositor.CreateVector3KeyFrameAnimation() };
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const Point3F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		[[nodiscard]] auto InsertKeyFrame(
			const float normalizedProgressKey, 
			const Point3F& value, const EasingFunction& easingFunction) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value, &easingFunction]
			{
				Get().InsertKeyFrame(normalizedProgressKey, value, easingFunction.Get());
			});
		}
	};

	//TODO Vector4KeyFrameAnimation
}

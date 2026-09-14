module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.KeyFrameAnimation;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.Shape;
import PGUI.UI.Color;
import PGUI.UI.VL.CompositionAnimation;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
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
				return std::chrono::milliseconds{ this->Get().DelayTime() };
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
				return std::chrono::milliseconds{ this->Get().Duration() };
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

		[[nodiscard]] auto InsertExpressionKeyFrame(const float normalizedProgressKey, const std::wstring& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				this->Get().InsertExpressionKeyFrame(normalizedProgressKey, value);
			});
		}

		//TODO InsertExpressionKeyFrame(float normalizedProgressKey, string value, CompositionEasingFunction easingFunction) -> void
	};
	using KeyFrameAnimation = KeyFrameAnimationT<>;


	class BoolKeyFrameAnimation : public KeyFrameAnimationT<WUC::BooleanKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const bool value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, value]
			{
				this->Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
	};
	class ColorKeyFrameAnimation : public KeyFrameAnimationT<WUC::ColorKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;

		[[nodiscard]] auto GetInterpolationColorSpace() const noexcept -> Result<ColorSpace>
		{
			return CallProjected([this]
			{
				return this->Get().InterpolationColorSpace();
			});
		}
		[[nodiscard]] auto SetInterpolationColorSpace(const ColorSpace colorSpace) const noexcept -> Result<void>
		{
			return CallProjected([this, colorSpace]
			{
				this->Get().InterpolationColorSpace(colorSpace);
			});
		}

		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const RGBA& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				this->Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		//TODO InsertKeyFrame(float normalizedProgressKey, Color value, CompositionEasingFunction easingFunction) -> void
	};
	class [[deprecated("This API contains a known issue in Windows 10, versions 20H2 and below. Do not use this API in your code")]]
		PathKeyFrameAnimation : public KeyFrameAnimationT<WUC::PathKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;

		//TODO InsertKeyFrame(float normalizedProgressKey, CompositionPath value, CompositionEasingFunction easingFunction) -> void
		//TODO InsertKeyFrame(float normalizedProgressKey, CompositionPath value, CompositionEasingFunction easingFunction) -> void
	};
	class QuaternionKeyFrameAnimation : public KeyFrameAnimationT<WUC::QuaternionKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const Quaternion& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				this->Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		//TODO InsertKeyFrame(float normalizedProgressKey, Quaternion value, CompositionEasingFunction easingFunction) -> void
	};
	class ScalarKeyFrameAnimation : public KeyFrameAnimationT<WUC::ScalarKeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const float value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, value]
			{
				this->Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		//TODO InsertKeyFrame(float normalizedProgressKey, float value, CompositionEasingFunction easingFunction) -> void
	};
	class Vector2KeyFrameAnimation : public KeyFrameAnimationT<WUC::Vector2KeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const Point2F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				this->Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		//TODO InsertKeyFrame(float normalizedProgressKey, Vector2 value, CompositionEasingFunction easingFunction) -> void
	};
	class Vector3KeyFrameAnimation : public KeyFrameAnimationT<WUC::Vector3KeyFrameAnimation>
	{
		public:
		using KeyFrameAnimationT::KeyFrameAnimationT;
		[[nodiscard]] auto InsertKeyFrame(const float normalizedProgressKey, const Point3F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, normalizedProgressKey, &value]
			{
				this->Get().InsertKeyFrame(normalizedProgressKey, value);
			});
		}
		//TODO InsertKeyFrame(float normalizedProgressKey, Vector3 value, CompositionEasingFunction easingFunction) -> void
	};

	//TODO Vector4KeyFrameAnimation
}

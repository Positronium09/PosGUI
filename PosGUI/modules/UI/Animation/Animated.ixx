export module PGUI.UI.Animation:Animated;

import :AnimationEnums;
import :Storyboard;
import :AnimationTransition;
import :AnimationTransitionLibrary;
import :AnimationTimer;
import :AnimationVariable;
import :AnimationManager;

import std;

import PGUI.ErrorHandling;

/*
	class Animated
	{
		public:
		explicit Animated(const float f)
		{
			var = AnimationManager::GetGlobalInstance().CreateAnimationVariable(f).value();
		}

		auto AnimateTo(float f) const -> void
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				throw std::runtime_error("Failed to create storyboard");
			}
			const auto& storyboard = storyboardResult.value();
			auto res = storyboard.AddTransition(var, AnimationTransitionLibrary::LinearTransition(1.0F, f).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			(void)storyboard.Schedule(timer.GetTime().value());
		}

		auto Get() const noexcept
		{
			return static_cast<float>(*var.GetValue());
		}

		private:
		AnimationVariable var;
	};
 */

export namespace PGUI::UI::Animation
{
	template <typename T>
	class Animated
	{
		public:
		explicit Animated(const T& initialValue, 
			const std::optional<AnimationManager>& animationManager = std::nullopt)
		{
			const auto& manager = GetAnimationManager();

			const auto varResult = manager.CreateAnimationVariable(initialValue);
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const T& newValue, const double duration = 1.0) const -> void
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				throw std::runtime_error("Failed to create storyboard");
			}
			const auto& storyboard = storyboardResult.value();
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(duration, newValue).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			(void)storyboard.Schedule(timer.GetTime().value());
		}

		auto Get() const noexcept
		{
			return static_cast<T>(*var.GetValue());
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};
}

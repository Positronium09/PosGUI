export module PGUI.UI.Animation:Animated;

import :AnimationEnums;
import :Storyboard;
import :AnimationTransition;
import :AnimationTransitionLibrary;
import :AnimationTimer;
import :AnimationVariable;
import :AnimationManager;

export namespace PGUI::UI::Animation
{
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
}

module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTimer;

import std;

import :AnimationTimeTypes;
import :AnimationTimerEventHandler;
import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::UI::Animation
{
	class AnimationTimer : public ComPtrHolder<IUIAnimationTimer>
	{
		public:
		explicit(false) AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept;

		[[nodiscard]] static auto Create() noexcept -> Result<AnimationTimer>;

		[[nodiscard]] static auto GetGlobalInstance() noexcept -> Result<std::reference_wrapper<const AnimationTimer>>;

		[[nodiscard]] auto Enable() const noexcept -> Result<void>;

		[[nodiscard]] auto Disable() const noexcept -> Result<void>;

		[[nodiscard]] auto IsEnabled() const noexcept -> Result<bool>;

		[[nodiscard]] auto GetTime() const noexcept -> Result<Seconds>;

		[[nodiscard]] auto SetFrameRateThreshold(UINT32 threshold) const noexcept -> Result<void>;

		[[nodiscard]] auto SetTimerEventHandler(AnimationTimerEventHandler& handler) const noexcept -> Result<void>;
		//TODO SetTimerUpdateHandle
	};
}

module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTimer;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::UI::Animation
{
	class AnimationTimer : public ComPtrHolder<IUIAnimationTimer>
	{
		public:
		AnimationTimer();

		explicit(false) AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept;

		auto Enable() const noexcept -> Error;

		auto Disable() const noexcept -> Error;

		[[nodiscard]] auto IsEnabled() const noexcept -> Result<bool>;

		[[nodiscard]] auto GetTime() const noexcept -> Result<double>;

		auto SetFrameRateThreshold(UINT32 threshold) const noexcept -> Error;

		//TODO SetTimerEventHandler
		//TODO SetTimerUpdateHandler
	};
}

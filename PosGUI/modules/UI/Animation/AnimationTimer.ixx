module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTimer;

import :AnimationTimeTypes;
import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::UI::Animation
{
	class AnimationTimer : public ComPtrHolder<IUIAnimationTimer>
	{
		public:
		[[nodiscard]] static auto GetGlobalInstance() -> const AnimationTimer&;

		AnimationTimer();

		explicit(false) AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept;

		auto Enable() const noexcept -> Error;

		auto Disable() const noexcept -> Error;

		[[nodiscard]] auto IsEnabled() const noexcept -> Result<bool>;

		[[nodiscard]] auto GetTime() const noexcept -> Result<Seconds>;

		auto SetFrameRateThreshold(UINT32 threshold) const noexcept -> Error;

		//TODO SetTimerEventHandler
		//TODO SetTimerUpdateHandler

		private:
		static inline AnimationTimer* instance = nullptr;
	};
}

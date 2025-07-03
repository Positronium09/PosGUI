module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTimer;

import PGUI.ComPtr;

export namespace PGUI::UI::Animation
{
	class AnimationTimer : public ComPtrHolder<IUIAnimationTimer>
	{
		public:
		AnimationTimer();

		explicit(false) AnimationTimer(const ComPtr<IUIAnimationTimer>& ptr) noexcept;

		auto Enable() const -> void;

		auto Disable() const -> void;

		[[nodiscard]] auto IsEnabled() const -> bool;

		[[nodiscard]] auto GetTime() const -> double;

		auto SetFrameRateThreshold(UINT32 threshold) const -> void;

		//TODO SetTimerEventHandler
		//TODO SetTimerUpdateHandler
	};
}

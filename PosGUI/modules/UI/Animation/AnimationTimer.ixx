module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationTimer;

import PGUI.ComPtr;

export namespace  PGUI::UI::Animation
{
	class AnimationTimer : public ComPtrHolder<IUIAnimationTimer>
	{
		public:
		AnimationTimer();
		AnimationTimer(ComPtr<IUIAnimationTimer> ptr) noexcept;

		void Enable() const;
		void Disable() const;

		[[nodiscard]] auto IsEnabled() const -> bool;
		[[nodiscard]] auto GetTime() const -> double;

		void SetFrameRateThreshold(UINT32 threshold) const;

		//TODO SetTimerEventHandler
		//TODO SetTimerUpdateHandler
	};
}

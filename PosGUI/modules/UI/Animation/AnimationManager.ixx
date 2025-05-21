module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationManager;

import PGUI.ComPtr;

export namespace PGUI::UI::Animation
{
	enum class AnimationManagerStatus
	{
		Idle = UI_ANIMATION_MANAGER_IDLE,
		Busy = UI_ANIMATION_MANAGER_BUSY
	};

	enum class AnimationMode
	{
		Disabled = UI_ANIMATION_MODE_DISABLED,
		SystemDefault = UI_ANIMATION_MODE_SYSTEM_DEFAULT,
		Enabled = UI_ANIMATION_MODE_ENABLED
	};

	class AnimationManager : public ComPtrHolder<IUIAnimationManager2>
	{
		public:
		[[nodiscard]] static const auto& GetInstance() noexcept { return instance; }

		static void AbandonAllStoryboards();

		static void Pause();
		static void Resume();
		static void Shutdown();

		[[nodiscard]] static auto GetStatus() -> AnimationManagerStatus;

		static void SetAnimationMode(AnimationMode mode);

		private:
		AnimationManager();

		static AnimationManager instance;
	};

	inline AnimationManager AnimationManager::instance{ };
}

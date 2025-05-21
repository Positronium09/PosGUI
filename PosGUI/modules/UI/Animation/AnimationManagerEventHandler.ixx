module;
#include <UIAnimation.h>
#include <mutex>
#include <functional>

export module PGUI.UI.Animation.AnimationManagerEventHandler;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.UI.Animation.AnimationEnums;

namespace PGUI::UI::Animation
{
	class AnimationManagerEventHandlerRouter final : public IUIAnimationManagerEventHandler2
	{
		using ManagerStatusChangedHandler = std::function<HRESULT(AnimationManagerStatus, AnimationManagerStatus)>;

		public:
		AnimationManagerEventHandlerRouter() noexcept = default;
		AnimationManagerEventHandlerRouter(const ManagerStatusChangedHandler& handler) noexcept;

		auto __stdcall QueryInterface(const IID& iid, void** obj) -> HRESULT override;
		auto __stdcall AddRef() -> ULONG override;
		auto __stdcall Release() -> ULONG override;

		auto __stdcall OnManagerStatusChanged(UI_ANIMATION_MANAGER_STATUS newStatus,
			UI_ANIMATION_MANAGER_STATUS previousStatus) -> HRESULT override;

		void SetHandler(const ManagerStatusChangedHandler& handler) noexcept;
		void ClearHandler() noexcept;

		private:
		std::mutex handlerMutex;
		ManagerStatusChangedHandler handler{ };
		LONG referenceCount = 1;
	};
}

export namespace PGUI::UI::Animation
{
	class AnimationManagerEventHandler
	{
		public:
		AnimationManagerEventHandler();

		virtual void OnManagerStatusChanged(
			AnimationManagerStatus newStatus, 
			AnimationManagerStatus previousStatus) = 0;

		[[nodiscard]] auto& GetRouter() noexcept { return router; }
		[[nodiscard]] const auto& GetRouter() const noexcept { return router; }

		private:
		AnimationManagerEventHandlerRouter router;

		auto CallHandler(AnimationManagerStatus newStatus,
			AnimationManagerStatus previousStatus) noexcept -> HRESULT;
	};

	class AnimationManagerEvent : public AnimationManagerEventHandler, 
		public Event<AnimationManagerStatus, AnimationManagerStatus>
	{
		void OnManagerStatusChanged(AnimationManagerStatus newStatus, 
			AnimationManagerStatus previousStatus) override;
	};
}

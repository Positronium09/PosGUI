module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationManagerEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.Mutex;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	class AnimationManagerEventHandlerRouter final	: 
		public Implements<AnimationManagerEventHandlerRouter, IUIAnimationManagerEventHandler2>
	{
		using ManagerStatusChangedHandler = std::function<HRESULT(AnimationManagerStatus, AnimationManagerStatus)>;

		public:
		AnimationManagerEventHandlerRouter() noexcept = default;

		explicit AnimationManagerEventHandlerRouter(const ManagerStatusChangedHandler& handler) noexcept;

		auto __stdcall OnManagerStatusChanged(
			UI_ANIMATION_MANAGER_STATUS newStatus,
			UI_ANIMATION_MANAGER_STATUS previousStatus) -> HRESULT override;

		auto SetHandler(const ManagerStatusChangedHandler& newHandler) noexcept -> void;

		auto ClearHandler() noexcept -> void;

		private:
		Mutex::CSMutex handlerMutex;
		ManagerStatusChangedHandler handler{ };
	};
}

export namespace PGUI::UI::Animation
{
	class AnimationManagerEventHandler
	{
		public:
		AnimationManagerEventHandler() noexcept;

		virtual ~AnimationManagerEventHandler() noexcept;

		virtual auto OnManagerStatusChanged(
			AnimationManagerStatus newStatus,
			AnimationManagerStatus previousStatus) -> void = 0;

		[[nodiscard]] auto& GetRouter() noexcept { return router; }
		[[nodiscard]] const auto& GetRouter() const noexcept { return router; }

		private:
		ComPtr<AnimationManagerEventHandlerRouter> router;

		auto CallHandler(AnimationManagerStatus newStatus,
		                 AnimationManagerStatus previousStatus) noexcept -> HRESULT;
	};

	class AnimationManagerEvent final : public AnimationManagerEventHandler
	{
		public:
		[[nodiscard]] const auto& ManagerStatusChanged() const noexcept { return managerStatusChangedEvent; }
		[[nodiscard]] auto& ManagerStatusChanged() noexcept { return managerStatusChangedEvent; }

		private:
		Event<AnimationManagerStatus, AnimationManagerStatus> managerStatusChangedEvent{ };

		auto OnManagerStatusChanged(
			AnimationManagerStatus newStatus,
			AnimationManagerStatus previousStatus) -> void override;
	};
}

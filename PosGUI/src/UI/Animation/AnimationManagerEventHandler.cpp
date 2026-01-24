module;
#include <UIAnimation.h>

module PGUI.UI.Animation:AnimationManagerEventHandler;
import :AnimationManagerEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.Utils;
import PGUI.ErrorHandling;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	AnimationManagerEventHandlerRouter::AnimationManagerEventHandlerRouter(
		const ManagerStatusChangedHandler& handler) noexcept :
		handler{ handler }
	{ }

	auto __stdcall AnimationManagerEventHandlerRouter::OnManagerStatusChanged(
		UI_ANIMATION_MANAGER_STATUS newStatus,
		UI_ANIMATION_MANAGER_STATUS previousStatus) -> HRESULT
	{
		std::scoped_lock lock{ handlerMutex };
		if (!handler)
		{
			return E_NOTIMPL;
		}

		return handler(
			static_cast<AnimationManagerStatus>(newStatus),
			static_cast<AnimationManagerStatus>(previousStatus)
		);
	}

	auto AnimationManagerEventHandlerRouter::SetHandler(const ManagerStatusChangedHandler& newHandler) noexcept -> void
	{
		std::scoped_lock lock{ handlerMutex };
		handler = newHandler;
	}

	auto AnimationManagerEventHandlerRouter::ClearHandler() noexcept -> void
	{
		std::scoped_lock lock{ handlerMutex };
		handler = nullptr;
	}

	auto AnimationManagerEventHandler::CallHandler(
		const AnimationManagerStatus newStatus,
		const AnimationManagerStatus previousStatus) noexcept -> HRESULT
	{
		try
		{
			OnManagerStatusChanged(newStatus, previousStatus);
			return S_OK;
		}
		catch (const Exception& exception)
		{
			return exception.HResult();
		}
		catch (const std::exception&)
		{
			return E_FAIL;
		}
	}

	AnimationManagerEventHandler::AnimationManagerEventHandler() noexcept :
		router{
			MakeComPtr<AnimationManagerEventHandlerRouter>(
				std::bind_front(&AnimationManagerEventHandler::CallHandler, this))
		}
	{ }

	auto AnimationManagerEvent::OnManagerStatusChanged(
		const AnimationManagerStatus newStatus,
		const AnimationManagerStatus previousStatus) -> void
	{
		managerStatusChangedEvent.Invoke(newStatus, previousStatus);
	}
}

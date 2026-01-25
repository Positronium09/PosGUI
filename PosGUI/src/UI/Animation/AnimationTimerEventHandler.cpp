module;
#include <UIAnimation.h>

module PGUI.UI.Animation:AnimationTimerEventHandler;
import :AnimationTimerEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.Utils;
import PGUI.ErrorHandling;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	AnimationTimerEventHandlerRouter::AnimationTimerEventHandlerRouter(
		const UpdateHandler& postUpdateHandler,
		const UpdateHandler& preUpdateHandler,
		const SlowRenderingHandler& renderingTooSlowHandler) noexcept :
		postUpdateHandler{ postUpdateHandler },
		preUpdateHandler{ preUpdateHandler },
		renderingTooSlowHandler{ renderingTooSlowHandler }
	{ }

	auto AnimationTimerEventHandlerRouter::OnPostUpdate() -> HRESULT
	{
		std::lock_guard lock{ postUpdateHandlerMutex };
		if (!postUpdateHandler)
		{
			return E_NOTIMPL;
		}

		return postUpdateHandler();
	}

	auto AnimationTimerEventHandlerRouter::OnPreUpdate() -> HRESULT
	{
		std::lock_guard lock{ preUpdateHandlerMutex };
		if (!preUpdateHandler)
		{
			return E_NOTIMPL;
		}

		return preUpdateHandler();
	}

	auto AnimationTimerEventHandlerRouter::OnRenderingTooSlow(const UINT32 fps) -> HRESULT
	{
		std::lock_guard lock{ renderingTooSlowHandlerMutex };
		if (!renderingTooSlowHandler)
		{
			return E_NOTIMPL;
		}

		return renderingTooSlowHandler(fps);
	}

	auto AnimationTimerEventHandlerRouter::SetPostUpdateHandler(const UpdateHandler& handler) noexcept -> void
	{
		std::lock_guard lock{ postUpdateHandlerMutex };
		postUpdateHandler = handler;
	}

	auto AnimationTimerEventHandlerRouter::SetPreUpdateHandler(const UpdateHandler& handler) noexcept -> void
	{
		std::lock_guard lock{ preUpdateHandlerMutex };
		preUpdateHandler = handler;
	}

	auto AnimationTimerEventHandlerRouter::SetRenderingTooSlowHandler(
		const SlowRenderingHandler& handler) noexcept -> void
	{
		std::lock_guard lock{ renderingTooSlowHandlerMutex };
		renderingTooSlowHandler = handler;
	}

	auto AnimationTimerEventHandlerRouter::ClearPostUpdateHandler() noexcept -> void
	{
		std::lock_guard lock{ postUpdateHandlerMutex };
		postUpdateHandler = nullptr;
	}

	auto AnimationTimerEventHandlerRouter::ClearPreUpdateHandler() noexcept -> void
	{
		std::lock_guard lock{ preUpdateHandlerMutex };
		preUpdateHandler = nullptr;
	}

	auto AnimationTimerEventHandlerRouter::ClearRenderingTooSlowHandler() noexcept -> void
	{
		std::lock_guard lock{ renderingTooSlowHandlerMutex };
		renderingTooSlowHandler = nullptr;
	}

	AnimationTimerEventHandler::AnimationTimerEventHandler() noexcept :
		router{
			MakeComPtr<AnimationTimerEventHandlerRouter>(
				std::bind_front(&AnimationTimerEventHandler::CallPostUpdateHandler, this),
				std::bind_front(&AnimationTimerEventHandler::CallPreUpdateHandler, this),
				std::bind_front(&AnimationTimerEventHandler::CallRenderingTooSlowHandler, this)
			)
		}
	{ }

	AnimationTimerEventHandler::~AnimationTimerEventHandler() noexcept
	{
		router->ClearPostUpdateHandler();
		router->ClearPreUpdateHandler();
		router->ClearRenderingTooSlowHandler();
	}

	auto AnimationTimerEventHandler::CallPostUpdateHandler() noexcept -> HRESULT
	{
		try
		{
			OnPostUpdate();
			return S_OK;
		}
		catch (Exception& exception)
		{
			return exception.HResult();
		}
		catch (std::exception&)
		{
			return E_FAIL;
		}
	}

	auto AnimationTimerEventHandler::CallPreUpdateHandler() noexcept -> HRESULT
	{
		try
		{
			OnPreUpdate();
			return S_OK;
		}
		catch (Exception& exception)
		{
			return exception.HResult();
		}
		catch (std::exception&)
		{
			return E_FAIL;
		}
	}

	auto AnimationTimerEventHandler::CallRenderingTooSlowHandler(const UINT32 fps) noexcept -> HRESULT
	{
		try
		{
			OnRenderingTooSlow(fps);
			return S_OK;
		}
		catch (Exception& exception)
		{
			return exception.HResult();
		}
		catch (std::exception&)
		{
			return E_FAIL;
		}
	}

	auto AnimationTimerEvent::OnPostUpdate() -> void
	{
		postUpdateEvent.Invoke();
	}

	auto AnimationTimerEvent::OnPreUpdate() -> void
	{
		preUpdateEvent.Invoke();
	}

	auto AnimationTimerEvent::OnRenderingTooSlow(const UINT32 framesPerSecond) -> void
	{
		renderingTooSlowEvent.Invoke(framesPerSecond);
	}
}

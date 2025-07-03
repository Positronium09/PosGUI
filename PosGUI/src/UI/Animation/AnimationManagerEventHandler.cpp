module;
#include <UIAnimation.h>

module PGUI.UI.Animation:AnimationManagerEventHandler;
import :AnimationManagerEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.Utils;
import PGUI.Exceptions;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	AnimationManagerEventHandlerRouter::AnimationManagerEventHandlerRouter(
		const ManagerStatusChangedHandler& handler) noexcept :
		handler{ handler }
	{ }

	auto __stdcall AnimationManagerEventHandlerRouter::QueryInterface(const IID& iid, void** obj) -> HRESULT
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}
		*obj = nullptr;

		if (iid == IID_IUnknown || iid == IID_IUIAnimationManagerEventHandler2)
		{
			*obj = static_cast<void*>(this);
			AddRef();

			return NOERROR;
		}

		return E_NOINTERFACE;
	}

	auto __stdcall AnimationManagerEventHandlerRouter::AddRef() -> ULONG
	{
		return InterlockedIncrement(&referenceCount);
	}

	auto __stdcall AnimationManagerEventHandlerRouter::Release() -> ULONG
	{
		const auto refCount = InterlockedDecrement(&referenceCount);
		if (0 == refCount)
		{
			delete this;
		}
		return refCount;
	}

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
		catch (const PGUI::HResultException& e)
		{
			return e.HResult();
		}
		catch (const PGUI::Win32Exception& e)
		{
			return PGUI::HresultFromWin32(e.ErrorCode());
		}
		catch (const std::exception&)
		{
			return E_FAIL;
		}
	}

	AnimationManagerEventHandler::AnimationManagerEventHandler() noexcept :
		router{ std::bind_front(&AnimationManagerEventHandler::CallHandler, this) }
	{ }

	auto AnimationManagerEvent::OnManagerStatusChanged(
		const AnimationManagerStatus newStatus,
		const AnimationManagerStatus previousStatus) -> void
	{
		managerStatusChangedEvent.Invoke(newStatus, previousStatus);
	}
}

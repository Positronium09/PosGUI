module;
#include <wrl.h>
#include <UIAnimation.h>

module PGUI.UI.Animation.AnimationStoryboardEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.Animation.AnimationEnums;
import PGUI.UI.Animation.Storyboard;

namespace PGUI::UI::Animation
{
	AnimationStoryboardEventHandlerRouter::AnimationStoryboardEventHandlerRouter(
		const StoryboardStatusChangedHandler& statusChangedHandler, 
		const StoryboardUpdatedHandler& updatedHandler) noexcept : 
		statusChangedHandler{ statusChangedHandler },
		updatedHandler{ updatedHandler }
	{
	}

	auto __stdcall AnimationStoryboardEventHandlerRouter::QueryInterface(const IID& iid, void** obj) -> HRESULT
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}
		*obj = nullptr;

		if (iid == IID_IUnknown || iid == IID_IUIAnimationStoryboardEventHandler2)
		{
			*obj = static_cast<void*>(this);
			AddRef();

			return NOERROR;
		}

		return E_NOINTERFACE;
	}

	auto __stdcall AnimationStoryboardEventHandlerRouter::AddRef() -> ULONG
	{
		return InterlockedIncrement(&referenceCount);
	}
	auto __stdcall AnimationStoryboardEventHandlerRouter::Release() -> ULONG
	{
		auto refCount = InterlockedDecrement(&referenceCount);
		if (0 == refCount)
		{
			delete this;
		}
		return refCount;
	}

	auto __stdcall AnimationStoryboardEventHandlerRouter::OnStoryboardStatusChanged(
		IUIAnimationStoryboard2* storyboard, 
		UI_ANIMATION_STORYBOARD_STATUS newStatus, 
		UI_ANIMATION_STORYBOARD_STATUS previousStatus) -> HRESULT
	{
		std::scoped_lock lock{ statusChangedHandlerMutex };
		if (!statusChangedHandler)
		{
			return E_NOTIMPL;
		}

		return statusChangedHandler(
			Storyboard{ storyboard },
			static_cast<StoryboardStatus>(newStatus),
			static_cast<StoryboardStatus>(previousStatus)
		);
	}

	auto __stdcall AnimationStoryboardEventHandlerRouter::OnStoryboardUpdated(
		IUIAnimationStoryboard2* storyboard) -> HRESULT
	{
		std::scoped_lock lock{ updateHandlerMutex };
		if (!updatedHandler)
		{
			return E_NOTIMPL;
		}

		return updatedHandler(
			Storyboard{ storyboard }
		);
	}

	void AnimationStoryboardEventHandlerRouter::SetStatusChangedHandler(
		const StoryboardStatusChangedHandler& handler) noexcept
	{
		std::scoped_lock lock{ statusChangedHandlerMutex };
		statusChangedHandler = handler;
	}
	void AnimationStoryboardEventHandlerRouter::ClearStatusChangedHandler() noexcept
	{
		std::scoped_lock lock{ statusChangedHandlerMutex };
		statusChangedHandler = nullptr;
	}

	void AnimationStoryboardEventHandlerRouter::SetUpdatedHandler(
		const StoryboardUpdatedHandler& handler) noexcept
	{
		std::scoped_lock lock{ updateHandlerMutex };
		updatedHandler = handler;
	}
	void AnimationStoryboardEventHandlerRouter::ClearUpdatedHandler() noexcept
	{
		std::scoped_lock lock{ updateHandlerMutex };
		updatedHandler = nullptr;
	}

	AnimationStoryboardEventHandler::AnimationStoryboardEventHandler() noexcept :
		router{ 
			std::bind_front(&AnimationStoryboardEventHandler::CallStoryBoardStatusChangedHandler, this),
			std::bind_front(&AnimationStoryboardEventHandler::CallStoryBoardUpdatedHandler, this)
		}
	{
	}

	auto AnimationStoryboardEventHandler::CallStoryBoardStatusChangedHandler(
		Storyboard storyboard, 
		StoryboardStatus newStatus, 
		StoryboardStatus previousStatus) noexcept -> HRESULT
	{
		try
		{
			OnStoryBoardStatusChanged(storyboard, newStatus, previousStatus);
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

	auto AnimationStoryboardEventHandler::CallStoryBoardUpdatedHandler(
		Storyboard storyboard) noexcept -> HRESULT
	{
		try
		{
			OnStoryBoardUpdated(storyboard);
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

	void AnimationStoryboardEvent::OnStoryBoardStatusChanged(Storyboard storyboard,
		StoryboardStatus newStatus, StoryboardStatus previousStatus)
	{
		storyboardStatusChangedEvent.Invoke(storyboard, newStatus, previousStatus);
	}
	void AnimationStoryboardEvent::OnStoryBoardUpdated(Storyboard storyboard)
	{
		storyboardUpdatedEvent.Invoke(storyboard);
	}
}

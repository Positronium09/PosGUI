module;
#include <UIAnimation.h>

module PGUI.UI.Animation:AnimationStoryboardEventHandler;
import :AnimationStoryboardEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Utils;
import :AnimationEnums;
import :Storyboard;

namespace PGUI::UI::Animation
{
	AnimationStoryboardEventHandlerRouter::AnimationStoryboardEventHandlerRouter(
		const StoryboardStatusChangedHandler& statusChangedHandler,
		const StoryboardUpdatedHandler& updatedHandler) noexcept :
		updatedHandler{ updatedHandler },
		statusChangedHandler{ statusChangedHandler }
	{ }

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
		const auto refCount = InterlockedDecrement(&referenceCount);
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

	auto AnimationStoryboardEventHandlerRouter::SetStatusChangedHandler(
		const StoryboardStatusChangedHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ statusChangedHandlerMutex };
		statusChangedHandler = handler;
	}

	auto AnimationStoryboardEventHandlerRouter::ClearStatusChangedHandler() noexcept -> void
	{
		std::scoped_lock lock{ statusChangedHandlerMutex };
		statusChangedHandler = nullptr;
	}

	auto AnimationStoryboardEventHandlerRouter::SetUpdatedHandler(
		const StoryboardUpdatedHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ updateHandlerMutex };
		updatedHandler = handler;
	}

	auto AnimationStoryboardEventHandlerRouter::ClearUpdatedHandler() noexcept -> void
	{
		std::scoped_lock lock{ updateHandlerMutex };
		updatedHandler = nullptr;
	}

	AnimationStoryboardEventHandler::AnimationStoryboardEventHandler() noexcept :
		router{
			std::bind_front(&AnimationStoryboardEventHandler::CallStoryBoardStatusChangedHandler, this),
			std::bind_front(&AnimationStoryboardEventHandler::CallStoryBoardUpdatedHandler, this)
		}
	{ }

	auto AnimationStoryboardEventHandler::CallStoryBoardStatusChangedHandler(
		const Storyboard& storyboard,
		const StoryboardStatus newStatus,
		const StoryboardStatus previousStatus) noexcept -> HRESULT
	{
		try
		{
			OnStoryBoardStatusChanged(storyboard, newStatus, previousStatus);
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

	auto AnimationStoryboardEventHandler::CallStoryBoardUpdatedHandler(
		const Storyboard& storyboard) noexcept -> HRESULT
	{
		try
		{
			OnStoryBoardUpdated(storyboard);
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

	auto AnimationStoryboardEvent::OnStoryBoardStatusChanged(
		const Storyboard storyboard,
		const StoryboardStatus newStatus,
		const StoryboardStatus previousStatus) -> void
	{
		storyboardStatusChangedEvent.Invoke(storyboard, newStatus, previousStatus);
	}

	auto AnimationStoryboardEvent::OnStoryBoardUpdated(const Storyboard storyboard) -> void
	{
		storyboardUpdatedEvent.Invoke(storyboard);
	}
}

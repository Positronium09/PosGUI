module;
#include <wrl.h>
#include <UIAnimation.h>
#include <span>
#include <mutex>
#include <vector>
#include <functional>

module PGUI.UI.Animation.AnimationVariableChangeEventHandler;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.Exceptions;
import PGUI.UI.Animation.Storyboard;
import PGUI.UI.Animation.AnimationVariable;
import PGUI.UI.Animation.AnimationEnums;

namespace PGUI::UI::Animation
{
	AnimationVariableChangeEventHandlerRouter::AnimationVariableChangeEventHandlerRouter(
		const AnimationVariableChangeHandler& handler) noexcept : 
		handler{ handler }
	{
	}
	auto __stdcall AnimationVariableChangeEventHandlerRouter::QueryInterface(const IID& iid, void** obj) -> HRESULT
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}
		*obj = nullptr;

		if (iid == IID_IUnknown || iid == IID_IUIAnimationVariableChangeHandler2)
		{
			*obj = static_cast<void*>(this);
			AddRef();

			return NOERROR;
		}

		return E_NOINTERFACE;
	}

	auto __stdcall AnimationVariableChangeEventHandlerRouter::AddRef() -> ULONG
	{
		return InterlockedIncrement(&referenceCount);
	}
	auto __stdcall AnimationVariableChangeEventHandlerRouter::Release() -> ULONG
	{
		auto refCount = InterlockedDecrement(&referenceCount);
		if (0 == refCount)
		{
			delete this;
		}
		return refCount;
	}

	auto __stdcall AnimationVariableChangeEventHandlerRouter::OnValueChanged(
		IUIAnimationStoryboard2* storyboard, IUIAnimationVariable2* variable, 
		DOUBLE* newValues, DOUBLE* previousValues, UINT dimension) -> HRESULT
	{
		std::scoped_lock lock{ handlerMutex };
		if (!handler)
		{
			return E_NOTIMPL;
		}

		std::span<double> newValueSpan{ newValues, dimension };
		std::span<double> previousValueSpan{ previousValues, dimension };

		return handler(Storyboard{ storyboard }, AnimationVariable{ variable },
			newValueSpan, previousValueSpan);
	}

	void AnimationVariableChangeEventHandlerRouter::SetHandler(
		const AnimationVariableChangeHandler& newHandler) noexcept
	{
		std::scoped_lock lock{ handlerMutex };
		handler = newHandler;
	}
	void AnimationVariableChangeEventHandlerRouter::ClearHandler() noexcept
	{
		std::scoped_lock lock{ handlerMutex };
		handler = nullptr;
	}


	AnimationVariableIntegerChangeEventHandlerRouter::AnimationVariableIntegerChangeEventHandlerRouter(
		const AnimationIntegerVariableChangeHandler& handler) noexcept : 
		handler{ handler }
	{
	}

	auto __stdcall AnimationVariableIntegerChangeEventHandlerRouter::QueryInterface(const IID& iid, void** obj) -> HRESULT
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}
		*obj = nullptr;

		if (iid == IID_IUnknown || iid == IID_IUIAnimationVariableIntegerChangeHandler2)
		{
			*obj = static_cast<void*>(this);
			AddRef();

			return NOERROR;
		}

		return E_NOINTERFACE;
	}

	auto __stdcall AnimationVariableIntegerChangeEventHandlerRouter::AddRef() -> ULONG
	{
		return InterlockedIncrement(&referenceCount);
	}
	auto __stdcall AnimationVariableIntegerChangeEventHandlerRouter::Release() -> ULONG
	{
		auto refCount = InterlockedDecrement(&referenceCount);
		if (0 == refCount)
		{
			delete this;
		}
		return refCount;
	}

	auto __stdcall AnimationVariableIntegerChangeEventHandlerRouter::OnIntegerValueChanged(
		IUIAnimationStoryboard2* storyboard, IUIAnimationVariable2* variable, 
		INT32* newValues, INT32* previousValues, UINT dimension) -> HRESULT
	{
		std::scoped_lock lock{ handlerMutex };
		if (!handler)
		{
			return E_NOTIMPL;
		}
		std::span<INT32> newValueSpan{ newValues, dimension };
		std::span<INT32> previousValueSpan{ previousValues, dimension };

		return handler(Storyboard{ storyboard }, AnimationVariable{ variable },
			newValueSpan, previousValueSpan);
	}

	void AnimationVariableIntegerChangeEventHandlerRouter::SetHandler(
		const AnimationIntegerVariableChangeHandler& newHandler) noexcept
	{
		std::scoped_lock lock{ handlerMutex };
		handler = newHandler;
	}
	void AnimationVariableIntegerChangeEventHandlerRouter::ClearHandler() noexcept
	{
		std::scoped_lock lock{ handlerMutex };
		handler = nullptr;
	}


	AnimationVariableChangeEventHandler::AnimationVariableChangeEventHandler() noexcept : 
		router{ std::bind_front(&AnimationVariableChangeEventHandler::CallVariableChanged, this) },
		integerRouter{ std::bind_front(&AnimationVariableChangeEventHandler::CallVariableIntegerChanged, this) }
	{
	}

	auto AnimationVariableChangeEventHandler::CallVariableChanged(
		Storyboard storyboard, AnimationVariable variable, 
		std::span<double> newValues, std::span<double> previousValues) noexcept -> HRESULT
	{
		try
		{
			OnVariableChanged(storyboard, variable, newValues, previousValues);
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
	auto AnimationVariableChangeEventHandler::CallVariableIntegerChanged(
		Storyboard storyboard, AnimationVariable variable, 
		std::span<INT32> newValues, std::span<INT32> previousValues) noexcept -> HRESULT
	{
		try
		{
			OnVariableIntegerChanged(storyboard, variable, newValues, previousValues);
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

	void AnimationVariableChangeEvent::OnVariableChanged(Storyboard storyboard, AnimationVariable variable, 
		std::span<double> newValues, std::span<double> previousValues)
	{
		variableChangedEvent.Invoke(
			std::move(storyboard), std::move(variable), 
			std::move(newValues), std::move(previousValues));
	}
	void AnimationVariableChangeEvent::OnVariableIntegerChanged(Storyboard storyboard, AnimationVariable variable, 
		std::span<INT32> newValues, std::span<INT32> previousValues)
	{
		variableIntegerChangedEvent.Invoke(
			std::move(storyboard), std::move(variable),
			std::move(newValues), std::move(previousValues));
	}
}

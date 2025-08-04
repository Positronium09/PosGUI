module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationVariableChangeEventHandler;
import :AnimationVariableChangeEventHandler;

import std;

import PGUI.ComPtr;
import PGUI.Event;
import PGUI.ErrorHandling;
import :Storyboard;
import :AnimationVariable;
import :AnimationEnums;

namespace PGUI::UI::Animation
{
	AnimationVariableChangeEventHandlerRouter::AnimationVariableChangeEventHandlerRouter(
		const AnimationVariableChangeHandler& handler) noexcept :
		handler{ handler }
	{ }

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
		const auto refCount = InterlockedDecrement(&referenceCount);
		if (0 == refCount)
		{
			delete this;
		}
		return refCount;
	}

	auto __stdcall AnimationVariableChangeEventHandlerRouter::OnValueChanged(
		IUIAnimationStoryboard2* storyboard, IUIAnimationVariable2* variable,
		DOUBLE* newValues, DOUBLE* previousValues, const UINT dimension) -> HRESULT
	{
		std::scoped_lock lock{ handlerMutex };
		if (!handler)
		{
			return E_NOTIMPL;
		}

		const std::span newValueSpan{ newValues, dimension };
		const std::span previousValueSpan{ previousValues, dimension };

		return handler(Storyboard{ storyboard }, AnimationVariable{ variable },
		               newValueSpan, previousValueSpan);
	}

	auto AnimationVariableChangeEventHandlerRouter::SetHandler(
		const AnimationVariableChangeHandler& newHandler) noexcept -> void
	{
		std::scoped_lock lock{ handlerMutex };
		handler = newHandler;
	}

	auto AnimationVariableChangeEventHandlerRouter::ClearHandler() noexcept -> void
	{
		std::scoped_lock lock{ handlerMutex };
		handler = nullptr;
	}


	AnimationVariableIntegerChangeEventHandlerRouter::AnimationVariableIntegerChangeEventHandlerRouter(
		const AnimationIntegerVariableChangeHandler& handler) noexcept :
		handler{ handler }
	{ }

	auto __stdcall AnimationVariableIntegerChangeEventHandlerRouter::QueryInterface(
		const IID& iid, void** obj) -> HRESULT
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
		const auto refCount = InterlockedDecrement(&referenceCount);
		if (0 == refCount)
		{
			delete this;
		}
		return refCount;
	}

	auto __stdcall AnimationVariableIntegerChangeEventHandlerRouter::OnIntegerValueChanged(
		IUIAnimationStoryboard2* storyboard, IUIAnimationVariable2* variable,
		INT32* newValues, INT32* previousValues, const UINT dimension) -> HRESULT
	{
		std::scoped_lock lock{ handlerMutex };
		if (!handler)
		{
			return E_NOTIMPL;
		}
		const std::span newValueSpan{ newValues, dimension };
		const std::span previousValueSpan{ previousValues, dimension };

		return handler(Storyboard{ storyboard }, AnimationVariable{ variable },
		               newValueSpan, previousValueSpan);
	}

	auto AnimationVariableIntegerChangeEventHandlerRouter::SetHandler(
		const AnimationIntegerVariableChangeHandler& newHandler) noexcept -> void
	{
		std::scoped_lock lock{ handlerMutex };
		handler = newHandler;
	}

	auto AnimationVariableIntegerChangeEventHandlerRouter::ClearHandler() noexcept -> void
	{
		std::scoped_lock lock{ handlerMutex };
		handler = nullptr;
	}


	AnimationVariableChangeEventHandler::AnimationVariableChangeEventHandler() noexcept :
		router{ std::bind_front(&AnimationVariableChangeEventHandler::CallVariableChanged, this) },
		integerRouter{ std::bind_front(&AnimationVariableChangeEventHandler::CallVariableIntegerChanged, this) }
	{ }

	auto AnimationVariableChangeEventHandler::CallVariableChanged(
		const Storyboard& storyboard, const AnimationVariable& variable,
		const std::span<double> newValues, const std::span<double> previousValues) noexcept -> HRESULT
	{
		try
		{
			OnVariableChanged(storyboard, variable, newValues, previousValues);
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

	auto AnimationVariableChangeEventHandler::CallVariableIntegerChanged(
		const Storyboard& storyboard, const AnimationVariable& variable,
		const std::span<INT32> newValues, const std::span<INT32> previousValues) noexcept -> HRESULT
	{
		try
		{
			OnVariableIntegerChanged(storyboard, variable, newValues, previousValues);
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

	auto AnimationVariableChangeEvent::OnVariableChanged(
		const Storyboard storyboard, const AnimationVariable variable,
		const std::span<double> newValues, const std::span<double> previousValues) -> void
	{
		variableChangedEvent.Invoke(
			storyboard, variable,
			newValues, previousValues);
	}

	auto AnimationVariableChangeEvent::OnVariableIntegerChanged(
		const Storyboard storyboard, const AnimationVariable variable,
		const std::span<INT32> newValues, const std::span<INT32> previousValues) -> void
	{
		variableIntegerChangedEvent.Invoke(
			storyboard, variable,
			newValues, previousValues);
	}
}

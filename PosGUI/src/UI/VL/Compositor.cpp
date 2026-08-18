module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.System.h>

#include <DispatcherQueue.h>

module PGUI.UI.VL.Compositor;

import std;

import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace WUC = winrt::Windows::UI::Composition;
namespace WSystem = winrt::Windows::System;
using IDesktopInterop = ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop;

namespace PGUI::UI::VL::Compositor
{
	auto InitDispatcherQueueController() -> void
	{
		constexpr DispatcherQueueOptions options{
			sizeof(DispatcherQueueOptions),
			DQTYPE_THREAD_CURRENT,
			DQTAT_COM_STA
		};

		if (const auto error = Error{
				CreateDispatcherQueueController(
					options,
					ProjectionPut<
						ABI::Windows::System::IDispatcherQueueController>(Internal::dispatcherQueueController))
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Failed to create dispatcher queue controller" };
		}
	}

	auto GetCompositor() noexcept -> Result<WUC::Compositor>
	{
		if (Internal::compositor)
		{
			return Internal::compositor;
		}

		if (Internal::dispatcherQueueController == nullptr)
		{
			return Unexpected{
				Error{ ErrorCode::NotInitialized }
				.SetCustomMessage(L"DispatcherQueueController must be initialized first")
				.SuggestFix(L"Call PGUI::UI::VL::InitDispatcherQueueController before this function")
			};
		}

		try
		{
			Internal::compositor = WUC::Compositor{ };
			return Internal::compositor;
		}
		catch (const winrt::hresult_error& error)
		{
			return Unexpected{ Error{ static_cast<HRESULT>(error.code()) }.SetCustomMessage(error.message().c_str()) };
		}
	}

	auto GetDesktopInterop() noexcept -> Result<ComPtr<IDesktopInterop>>
	{
		if (Internal::desktopInterop)
		{
			return Internal::desktopInterop;
		}

		const auto compositorResult = GetCompositor();
		if (!compositorResult.has_value())
		{
			return Unexpected{ compositorResult.error() };
		}
		const auto& compositor = compositorResult.value();

		const auto ptr = compositor.try_as<IDesktopInterop>();
		if (!ptr)
		{
			return Unexpected{ SystemErrorCode::InterfaceNotSupported };
		}

		Internal::desktopInterop = WinrtToComPtr(ptr);
		return Internal::desktopInterop;
	}
}

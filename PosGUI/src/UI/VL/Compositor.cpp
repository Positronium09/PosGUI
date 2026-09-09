module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.System.h>

#include <windows.graphics.effects.interop.h>

#include <windows.ui.composition.interop.h>


#include <DispatcherQueue.h>

module PGUI.UI.VL.Compositor;

import std;

import PGUI.Utils;
import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace WUC = winrt::Windows::UI::Composition;
namespace WF = winrt::Windows::Foundation;
namespace WSystem = winrt::Windows::System;
namespace AWUC = ABI::Windows::UI::Composition;

using IDesktopInterop = ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop;

namespace PGUI::UI::VL::Compositor::Internal
{
	struct CompositorThreadState
	{
		WUC::Compositor compositor{ nullptr };
		WSystem::DispatcherQueueController dispatcherQueueController{ nullptr };

		ComPtr<IDesktopInterop> desktopInterop{ nullptr };
		ComPtr<AWUC::ICompositorInterop> interop{ nullptr };
	};

	[[nodiscard]] auto GetCompositorState() noexcept -> CompositorThreadState*
	{
		thread_local auto* state = new(std::nothrow) CompositorThreadState{ };
		return state;
	}
}

namespace PGUI::UI::VL::Compositor
{
	auto InitDispatcherQueueController() -> void
	{
		constexpr DispatcherQueueOptions options{
			sizeof(DispatcherQueueOptions),
			DQTYPE_THREAD_CURRENT,
			DQTAT_COM_NONE
		};

		if (const auto error = Error{
				CreateDispatcherQueueController(
					options,
					ProjectionPut<
						ABI::Windows::System::IDispatcherQueueController>(
						Internal::GetCompositorState()->dispatcherQueueController))
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Failed to create dispatcher queue controller" };
		}
	}

	auto Shutdown() -> void
	{
		const auto& dispatcherQueueController = Internal::GetCompositorState()->dispatcherQueueController;
		const auto action = dispatcherQueueController.ShutdownQueueAsync();
		const auto dispatcherQueue = WSystem::DispatcherQueue::GetForCurrentThread();

		auto done = false;
		dispatcherQueue.ShutdownCompleted([&done](auto&&, auto&&)
		{
			done = true;
		});

		auto msg = MSG{ };
		while (!done && GetMessageW(&msg, nullptr, 0, 0))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage(static_cast<int>(msg.wParam));
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	auto GetCompositor() noexcept -> Result<WUC::Compositor>
	{
		auto& state = *Internal::GetCompositorState();
		auto& compositor = state.compositor;
		const auto& dispatcherQueueController = state.dispatcherQueueController;

		if (compositor)
		{
			return compositor;
		}

		if (dispatcherQueueController == nullptr)
		{
			return Unexpected{
				Error{ ErrorCode::NotInitialized }
				.SetCustomMessage(L"DispatcherQueueController must be initialized first")
				.SuggestFix(L"Call PGUI::UI::VL::InitDispatcherQueueController before this function")
			};
		}

		return WrapThrowing([&]
		{
			compositor = WUC::Compositor{ };
			return compositor;
		});
	}

	auto GetDesktopInterop() noexcept -> Result<ComPtr<IDesktopInterop>>
	{
		auto& desktopInterop = Internal::GetCompositorState()->desktopInterop;

		if (desktopInterop)
		{
			return desktopInterop;
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

		desktopInterop = WinrtToComPtr(ptr);
		return desktopInterop;
	}

	auto GetCompositorInterop() noexcept -> Result<ComPtr<AWUC::ICompositorInterop>>
	{
		auto& interop = Internal::GetCompositorState()->interop;
		if (interop)
		{
			return interop;
		}

		const auto compositorResult = GetCompositor();
		if (!compositorResult.has_value())
		{
			return Unexpected{ compositorResult.error() };
		}
		const auto& compositor = compositorResult.value();

		const auto ptr = compositor.try_as<AWUC::ICompositorInterop>();
		if (!ptr)
		{
			return Unexpected{ SystemErrorCode::InterfaceNotSupported };
		}

		interop = WinrtToComPtr(ptr);
		return interop;
	}
}

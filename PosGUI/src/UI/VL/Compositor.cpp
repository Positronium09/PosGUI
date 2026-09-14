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
import PGUI.ProjectionHolder;
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

	[[nodiscard]] auto GetCompositorState() noexcept -> CompositorThreadState*&
	{
		thread_local auto* state = new(std::nothrow) CompositorThreadState{ };
		return state;
	}
}

namespace PGUI::UI::VL::Compositor
{
	auto InitDispatcherQueueController() -> void
	{
		if (Internal::GetCompositorState() == nullptr)
		{
			throw Exception{ Error{ ErrorCode::NotInitialized } };
		}

		constexpr DispatcherQueueOptions options{
			.dwSize = sizeof(DispatcherQueueOptions),
			.threadType = DQTYPE_THREAD_CURRENT,
			.apartmentType = DQTAT_COM_NONE
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
		if (Internal::GetCompositorState() == nullptr)
		{
			return;
		}

		ScopedDefer close{
			[]
			{
				if (Internal::GetCompositorState() == nullptr)
				{
					return;
				}
				if (const auto compositor = Internal::GetCompositorState()->compositor;
					compositor)
				{
					Unused(CallProjected([&compositor]
					{
						compositor.Close();
					}));
				}
				delete Internal::GetCompositorState();
				Internal::GetCompositorState() = nullptr;
			}
		};

		const auto& dispatcherQueueController = Internal::GetCompositorState()->dispatcherQueueController;
		if (!dispatcherQueueController)
		{
			return;
		}
		const auto dispatcherQueueResult = CallProjected([]
		{
			return WSystem::DispatcherQueue::GetForCurrentThread();
		});
		if (!dispatcherQueueResult.has_value())
		{
			throw Exception{ dispatcherQueueResult.error(), L"Failed to get dispatcher queue for current thread" };
		}
		const auto& dispatcherQueue = dispatcherQueueResult.value();
		auto done = false;

		if (const auto result = CallProjected([&dispatcherQueue, &done]
			{
				return dispatcherQueue.ShutdownCompleted([&done](auto&&, auto&&)
				{
					done = true;
				});
			});
			!result.has_value())
		{
			throw Exception{ result.error(), L"Failed to shutdown dispatcher queue" };
		}

		const auto actionResult = CallProjected([&dispatcherQueueController]
		{
			return dispatcherQueueController.ShutdownQueueAsync();
		});
		if (!actionResult.has_value())
		{
			throw Exception{ actionResult.error(), L"Failed to shutdown dispatcher queue" };
		}

		auto msg = MSG{ };
		while (!done && GetMessageW(&msg, nullptr, 0, 0) > 0)
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
		auto* statePtr = Internal::GetCompositorState();
		if (statePtr == nullptr)
		{
			return Unexpected{ Error{ ErrorCode::NotInitialized } };
		}
		auto& state = *statePtr;

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
		auto* statePtr = Internal::GetCompositorState();
		if (statePtr == nullptr)
		{
			return Unexpected{ Error{ ErrorCode::NotInitialized } };
		}
		auto& desktopInterop = statePtr->desktopInterop;

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
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		desktopInterop = WinrtToComPtr(ptr);
		return desktopInterop;
	}

	auto GetCompositorInterop() noexcept -> Result<ComPtr<AWUC::ICompositorInterop>>
	{
		auto* statePtr = Internal::GetCompositorState();
		if (statePtr == nullptr)
		{
			return Unexpected{ Error{ ErrorCode::NotInitialized } };
		}
		auto& interop = statePtr->interop;

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
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		interop = WinrtToComPtr(ptr);
		return interop;
	}
}

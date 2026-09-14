module;
#include <d2d1_3.h>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.Composition.h>

#include <windows.ui.composition.interop.h>

module PGUI.UI.VL.Surface.CompositionGraphicsDevice;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.UI.DXDevices;
import PGUI.UI.VL.Compositor;

namespace WUC = winrt::Windows::UI::Composition;
namespace AWUC = ABI::Windows::UI::Composition;

using IGraphicsDeviceInterop = ABI::Windows::UI::Composition::ICompositionGraphicsDeviceInterop;

namespace PGUI::UI::VL::CompositionGraphicsDevice::Internal
{
	struct GraphicsDeviceThreadState
	{
		WUC::CompositionGraphicsDevice graphicsDevice{ nullptr };
		ComPtr<IGraphicsDeviceInterop> interop{ nullptr };
		std::uint64_t deviceCreationID = 0;
	};

	[[nodiscard]] auto GetGraphicsDeviceState() noexcept -> GraphicsDeviceThreadState*&
	{
		thread_local auto* state = new(std::nothrow) GraphicsDeviceThreadState{ };
		return state;
	}

	[[nodiscard]] auto GetInteropFor(
		GraphicsDeviceThreadState& state) noexcept -> Result<ComPtr<IGraphicsDeviceInterop>>
	{
		if (state.interop)
		{
			return state.interop;
		}

		const auto ptr = state.graphicsDevice.try_as<IGraphicsDeviceInterop>();
		if (!ptr)
		{
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		state.interop = WinrtToComPtr(ptr);
		return state.interop;
	}
}

namespace PGUI::UI::VL::CompositionGraphicsDevice
{
	auto Shutdown() -> void
	{
		if (Internal::GetGraphicsDeviceState() == nullptr)
		{
			return;
		}

		if (const auto& graphicsDevice = Internal::GetGraphicsDeviceState()->graphicsDevice)
		{
			Unused(CallProjected([&graphicsDevice]
			{
				graphicsDevice.Close();
			}));
		}

		delete Internal::GetGraphicsDeviceState();
		Internal::GetGraphicsDeviceState() = nullptr;
	}

	auto GetGraphicsDevice() noexcept -> Result<WUC::CompositionGraphicsDevice>
	{
		auto* statePtr = Internal::GetGraphicsDeviceState();
		if (statePtr == nullptr)
		{
			return Unexpected{ Error{ ErrorCode::NotInitialized } };
		}
		auto& state = *statePtr;

		const auto& renderingDevice = DXDevices::D2D1Device();
		if (!renderingDevice)
		{
			return Unexpected{
				Error{ ErrorCode::NotInitialized }
				.SetCustomMessage(L"D2D1 device must be initialized first")
				.SuggestFix(L"Call PGUI::UI::DXDevices::InitDevices before this function")
			};
		}

		if (state.graphicsDevice)
		{
			if (const auto creationID = DXDevices::GetDeviceCreationID();
				creationID != state.deviceCreationID)
			{
				const auto interopResult = Internal::GetInteropFor(state);
				if (!interopResult.has_value())
				{
					return Unexpected{ interopResult.error() };
				}

				if (const auto error = Error{
						interopResult.value()->SetRenderingDevice(renderingDevice.get())
					};
					error.IsFailure())
				{
					return Unexpected{ error };
				}

				state.deviceCreationID = creationID;
			}

			return state.graphicsDevice;
		}

		const auto compositorInteropResult = Compositor::GetCompositorInterop();
		if (!compositorInteropResult.has_value())
		{
			return Unexpected{ compositorInteropResult.error() };
		}
		const auto& compositorInterop = compositorInteropResult.value();

		ComPtr<AWUC::ICompositionGraphicsDevice> graphicsDeviceInterop{ nullptr };
		if (const auto error = Error{
				compositorInterop->CreateGraphicsDevice(
					renderingDevice.get(),
					graphicsDeviceInterop.put())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		state.graphicsDevice = ComPtrToProjection<WUC::CompositionGraphicsDevice>(graphicsDeviceInterop);
		state.deviceCreationID = DXDevices::GetDeviceCreationID();

		return state.graphicsDevice;
	}

	auto GetGraphicsDeviceInterop() noexcept -> Result<ComPtr<IGraphicsDeviceInterop>>
	{
		auto* statePtr = Internal::GetGraphicsDeviceState();
		if (statePtr == nullptr)
		{
			return Unexpected{ Error{ ErrorCode::NotInitialized } };
		}
		auto& state = *statePtr;

		if (state.interop)
		{
			return state.interop;
		}

		if (const auto deviceResult = GetGraphicsDevice();
			!deviceResult.has_value())
		{
			return Unexpected{ deviceResult.error() };
		}

		return Internal::GetInteropFor(state);
	}

	auto Trim() noexcept -> Result<void>
	{
		const auto deviceResult = GetGraphicsDevice();
		if (!deviceResult.has_value())
		{
			return Unexpected{ deviceResult.error() };
		}

		return WrapThrowing([&deviceResult]
		{
			deviceResult.value().Trim();
		});
	}
}

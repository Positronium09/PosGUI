module;
#include <d2d1.h>
#include <d2d1_3.h>
#include <d3d11.h>
#include <dcomp.h>
#include <dwrite_3.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_6.h>
#include <Windows.h>

module PGUI.UI.DCompWindow;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.DpiScaled;
import PGUI.UI.D2D.RenderTarget;
import PGUI.Factories;

namespace PGUI::UI
{
	auto DCompWindow::InitDCompDevice() -> void
	{
		if (dCompositionDevice.AreAllInitialized())
		{
			Logger::Info(
				L"DCompWindow::InitDCompDevice called, but DirectComposition device already initialized");
			return;
		}

		ComPtr<IDCompositionDevice4> dCompDevice;
		if (const auto error = Error{
				DCompositionCreateDevice3(
					DXDevices::DXGIDevice().get(),
					GetIID(dCompDevice),
					dCompDevice.put_void())
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot create DirectComposition device" };
		}

		dCompositionDevice = DComp::Device{ dCompDevice.try_query<IDCompositionDevice5>() };
		if (!dCompositionDevice.AreAllInitialized())
		{
			throw Exception{ Error{ SystemErrorCode::InterfaceNotSupported }, L"Cannot query IDCompositionDevice5 interface" };
		}

		auto surfaceFactoryResult = dCompositionDevice.CreateSurfaceFactory(DXDevices::DXGIDevice());
		if (!surfaceFactoryResult.has_value())
		{
			throw Exception{ surfaceFactoryResult.error(), L"Cannot create DirectComposition surface factory" };
		}

		dCompositionSurfaceFactory = MoveChecked(surfaceFactoryResult.value());
	}

	auto DCompWindow::BeginDraw() -> void
	{
		CreateDeviceResources();

		if (const auto hdc = BeginPaint(Hwnd(), &paintStruct);
			hdc == nullptr)
		{
			throw Exception{ Error{ GetLastError() }, L"BeginPaint failed" };
		}

		GetD2D1DeviceContext()->BeginDraw();
	}

	auto DCompWindow::EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG>
	{
		D2D1_TAG tag1 = 0;
		D2D1_TAG tag2 = 0;
		auto hr = GetD2D1DeviceContext()->EndDraw(&tag1, &tag2);

		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceResources();
			HandleDeviceLoss();
			InitDeviceDependent();

			EndPaint(Hwnd(), &paintStruct);

			Invalidate();

			return std::make_pair(tag1, tag2);
		}
		if (FAILED(hr))
		{
			throw Exception{ Error{ hr }, L"D2D1DeviceContext::EndDraw failed" };
		}

		hr = GetSwapChain()->Present(1, NULL);
		if (hr == DXGI_ERROR_DEVICE_RESET || hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			DiscardDeviceResources();
			HandleDeviceLoss();
			InitDeviceDependent();

			Invalidate();
		}
		else if (FAILED(hr))
		{
			throw Exception{ Error{ hr }, L"SwapChain::Present failed" };
		}

		EndPaint(Hwnd(), &paintStruct);

		return std::make_pair(tag1, tag2);
	}

	DCompWindow::DCompWindow(const WindowClassPtr& wndClass) noexcept :
		Window{ wndClass }
	{
		RegisterHandler(WM_NCCREATE, &DCompWindow::OnNCCreate);
		RegisterHandler(WM_WINDOWPOSCHANGED, &DCompWindow::OnWindowPosChanged);
		RegisterHandler(WM_PAINT, &DCompWindow::OnPaint);
	}

	auto DCompWindow::OnSizeChanged(SizeL) -> void
	{
		const auto& d2d1 = GetD2D1DeviceContext();
		d2d1->SetTarget(nullptr);

		SizeL size = LogicalToPhysical(GetClientSize());

		if (size.cy == 0)
		{
			size.cy = 1;
		}
		if (size.cx == 0)
		{
			size.cx = 1;
		}

		if (const auto error = Error{
				GetSwapChain()->ResizeBuffers(
					0, size.cx, size.cy,
					DXGI_FORMAT_UNKNOWN, NULL)
			};
			error.IsFailure())
		{
			throw Exception{
				Error{ error }
				.AddDetail(L"Window Size", std::format(L"{}", size))
			};
		}

		DiscardDeviceResources();
		InitD2D1DeviceContext();
	}

	auto DCompWindow::InitSwapChain() -> void
	{
		const auto dxgiFactory = Factories::DXGIFactory::GetFactory();
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ };
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
		swapChainDesc.Width = 1;
		swapChainDesc.Height = 1;

		if (const auto error = Error{
				dxgiFactory->CreateSwapChainForComposition(
					DXDevices::DXGIDevice().get(),
					&swapChainDesc, nullptr,
					Put<IDXGISwapChain1>())
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot create swap chain" };
		}
	}

	auto DCompWindow::InitD2D1DeviceContext() -> void
	{
		auto& d2d1Dc = GetD2D1DeviceContext();
		const auto& swapChain = GetSwapChain();

		if (d2d1Dc.get() == nullptr)
		{
			if (const auto error = Error{
					DXDevices::D2D1Device()->CreateDeviceContext(
						D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
						d2d1Dc.put())
				};
				error.IsFailure())
			{
				throw Exception{ error, L"Cannot create D2D1DeviceContext" };
			}
		}

		ComPtr<IDXGISurface2> surface;
		if (const auto error = Error{
				swapChain->GetBuffer(0, GetIID(surface), surface.put_void())
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot get swap chain buffer" };
		}

		D2D1_BITMAP_PROPERTIES1 properties = { };
		properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

		ComPtr<ID2D1Bitmap1> bitmap;
		if (const auto error = Error{
				d2d1Dc->CreateBitmapFromDxgiSurface(
					surface.get(),
					properties,
					bitmap.put())
			};
			error.IsFailure())
		{
			throw Exception{ error, L"Cannot create bitmap from DXGI surface" };
		}

		d2d1Dc->SetTarget(bitmap.get());
	}

	auto DCompWindow::InitDirectComposition() -> void
	{
		const auto& swapChain = GetSwapChain();

		auto targetResult = dCompositionDevice.CreateTargetForHwnd(Hwnd(), false);
		if (!targetResult.has_value())
		{
			throw Exception{ targetResult.error(), L"Cannot create DComposition target for window" };
		}
		target = MoveChecked(targetResult.value());

		auto visualResult = dCompositionDevice.CreateVisual();
		if (!visualResult.has_value())
		{
			throw Exception{ visualResult.error(), L"Cannot create DComposition visual" };
		}
		visual = MoveChecked(visualResult.value());

		if (const auto result = visual.SetContent(swapChain);
			!result.has_value())
		{
			throw Exception{ result.error(), L"Cannot set content for DComposition visual" };
		}

		if (const auto result = target.SetRoot(visual);
			!result.has_value())
		{
			throw Exception{ result.error(), L"Cannot set root for DComposition target" };
		}

		if (const auto result = dCompositionDevice.Commit();
			!result.has_value())
		{
			throw Exception{ result.error(), L"Cannot commit DComposition device" };
		}
	}

	auto DCompWindow::OnNCCreate(
		UINT, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		InitDeviceDependent();

		currentMonitor = MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTONULL);

		return { 1, MessageHandlerFlags::PassToDefProc };
	}

	auto DCompWindow::OnWindowPosChanged(
		const MessageID msg, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		if (const auto monitor = MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTONULL);
			monitor != currentMonitor)
		{
			currentMonitor = monitor;
			const auto& factory = Factories::DWriteFactory::GetFactory();

			ComPtr<IDWriteRenderingParams> renderingParams;
			const auto error = Error{
				factory->CreateMonitorRenderingParams(
					currentMonitor,
					renderingParams.put())
			};

			LogIfFailed(
				Error{ error }
				.AddDetail(L"Window Message", WindowMsgToText(msg)),
				L"Cannot create IDWriteRenderingParams with CreateMonitorRenderingParams"
			);

			if (error.IsSuccess())
			{
				GetD2D1DeviceContext()->SetTextRenderingParams(renderingParams.get());
			}
		}

		return { 0, MessageHandlerFlags::PassToDefProc };
	}

	auto DCompWindow::OnPaint(
		UINT, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		BeginDraw();

		Draw(GetGraphics());

		EndDraw();

		return 0;
	}
}

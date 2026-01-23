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

module PGUI.UI.DirectXCompositionWindow;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.UI.D2D.RenderTarget;
import PGUI.Factories;

namespace PGUI::UI
{
	DirectXCompositionWindow::DirectXCompositionWindow(const WindowClassPtr& wndClass) noexcept :
		Window{ wndClass }
	{
		RegisterHandler(WM_NCCREATE, &DirectXCompositionWindow::OnNCCreate);
		RegisterHandler(WM_WINDOWPOSCHANGED, &DirectXCompositionWindow::OnWindowPosChanged);
		RegisterHandler(WM_PAINT, &DirectXCompositionWindow::OnPaint);
	}

	auto DirectXCompositionWindow::OnSizeChanged(SizeL) -> void
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

		if (const auto hr = GetSwapChain()->ResizeBuffers(
			0, size.cx, size.cy,
			DXGI_FORMAT_UNKNOWN, NULL);
			FAILED(hr))
		{
			throw Exception{
				Error{ hr }
				.AddDetail(L"Window Size", std::format(L"{}", size))
			};
		}

		DiscardDeviceResources();
		InitD2D1DeviceContext();
	}

	auto DirectXCompositionWindow::BeginDraw() -> void
	{
		CreateDeviceResources();

		BeginPaint(Hwnd(), &paintStruct);

		GetD2D1DeviceContext()->BeginDraw();
	}

	auto DirectXCompositionWindow::EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG>
	{
		D2D1_TAG tag1;
		D2D1_TAG tag2;
		auto hr = GetD2D1DeviceContext()->EndDraw(&tag1, &tag2);

		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceResources();
		}
		else if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"D2D1DeviceContext::EndDraw failed"
			};
		}

		if (hr = GetSwapChain()->Present(1, NULL);
			FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"SwapChain::Present failed"
			};
		}

		EndPaint(Hwnd(), &paintStruct);

		return std::make_pair(tag1, tag2);
	}

	auto DirectXCompositionWindow::InitSwapChain() -> void
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

		if (const auto hr = dxgiFactory->CreateSwapChainForComposition(
				dxgiDevice.get(),
			&swapChainDesc, nullptr,
			Put<IDXGISwapChain1>());
			FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create swap chain"
			};
		}
	}

	auto DirectXCompositionWindow::InitD2D1DeviceContext() -> void
	{
		auto& d2d1Dc = GetD2D1DeviceContext();
		const auto& swapChain = GetSwapChain();

		auto hr = d2d1Device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&d2d1Dc);
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create D2D1DeviceContext"
			};
		}

		ComPtr<IDXGISurface2> surface;
		hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface2), surface.put_void());
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot get swap chain buffer"
			};
		}

		D2D1_BITMAP_PROPERTIES1 properties = { };
		properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

		ComPtr<ID2D1Bitmap1> bitmap;
		hr = d2d1Dc->CreateBitmapFromDxgiSurface(
			surface.get(),
			properties,
			&bitmap);
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create bitmap from DXGI surface"
			};
		}

		d2d1Dc->SetTarget(bitmap.get());
	}

	auto DirectXCompositionWindow::InitDirectComposition() -> void
	{
		const auto& swapChain = GetSwapChain();
		auto& dcompTarget = GetDCompositionTarget();
		auto& visual = GetDCompositionVisual();

		auto hr = dcompDevice->CreateTargetForHwnd(
			Hwnd(), false,
			&dcompTarget);
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create DComposition target for window"
			};
		}

		hr = dcompDevice->CreateVisual(&visual);
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create DComposition visual"
			};
		}

		hr = visual->SetContent(swapChain.get());
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot set content for DComposition visual"
			};
		}

		hr = dcompTarget->SetRoot(visual.get());
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot set root for DComposition target"
			};
		}

		hr = dcompDevice->Commit();
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot commit DComposition device"
			};
		}
	}

	auto DirectXCompositionWindow::InitD3D11Device() -> void
	{
		if (d3d11Device)
		{
			Logger::Info(L"DirectXCompositionWindow::InitD3D11Device called, but D3D11 device already initialized");
			return;
		}

		const auto dxgiFactory = Factories::DXGIFactory::GetFactory();
		SYSTEM_POWER_STATUS powerStatus{ };
		if (const auto ret = GetSystemPowerStatus(&powerStatus);
			ret == 0)
		{
			Logger::Error(
				L"Cannot get system power status {}",
				Error{ GetLastError() }
			);
		}

		auto gpuPreference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
		const auto powerSaverOn = powerStatus.SystemStatusFlag == 1;
		const bool lowBattery = powerStatus.BatteryFlag & (2 | 4);
		const auto charging = powerStatus.ACLineStatus != 0 || powerStatus.BatteryFlag & 8;

		if (const bool hasBattery = powerStatus.BatteryFlag & 128;
			(powerSaverOn || lowBattery) && (!charging) && hasBattery)
		{
			gpuPreference = DXGI_GPU_PREFERENCE_MINIMUM_POWER;
		}

		ComPtr<IDXGIAdapter1> adapter;
		auto hr = dxgiFactory->EnumAdapterByGpuPreference(
			0, gpuPreference,
			__uuidof(IDXGIAdapter1),
			adapter.put_void());
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot enumerate DXGI adapters"
			};
		}

		constexpr auto createDeviceFlags =
#ifdef _DEBUG
			static_cast<D3D11_CREATE_DEVICE_FLAG>(D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG);
#else
			D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

		constexpr std::array featureLevels =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		ComPtr<ID3D11Device> device;

		hr = D3D11CreateDevice(
			adapter.get(),
			D3D_DRIVER_TYPE_UNKNOWN, nullptr,
			createDeviceFlags,
			featureLevels.data(),
			// ReSharper disable once CppRedundantCastExpression
			static_cast<UINT>(featureLevels.size()),
			D3D11_SDK_VERSION,
			&device, nullptr, nullptr);
		if (FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create D3D11 device"
			};
		}

		d3d11Device = device.try_query<ID3D11Device2>();
		if (d3d11Device.get() == nullptr)
		{
			throw Exception{
				Error{ E_NOINTERFACE },
				L"Cannot query D3D11Device2 interface"
			};
		}

		dxgiDevice = d3d11Device.try_query<IDXGIDevice4>();
		if (dxgiDevice.get() == nullptr)
		{
			throw Exception{
				Error{ E_NOINTERFACE },
				L"Cannot query IDXGIDevice4 interface"
			};
		}
	}

	auto DirectXCompositionWindow::InitDCompDevice() -> void
	{
		if (dcompDevice)
		{
			Logger::Info(
				L"DirectXCompositionWindow::InitDCompDevice called, but DirectComposition device already initialized");
			return;
		}

		if (const auto hr = DCompositionCreateDevice(
				dxgiDevice.get(),
			__uuidof(dcompDevice),
			std::bit_cast<void**>(&dcompDevice));
			FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create DirectComposition device"
			};
		}
	}

	auto DirectXCompositionWindow::InitD2D1Device() -> void
	{
		if (d2d1Device)
		{
			Logger::Info(L"DirectXCompositionWindow::InitD2D1Device called, but D2D1 device already initialized");
			return;
		}

		const auto d2Factory = Factories::D2DFactory::GetFactory();

		if (const auto hr = d2Factory->CreateDevice(
				dxgiDevice.get(),
			&d2d1Device);
			FAILED(hr))
		{
			throw Exception{
				Error{ hr },
				L"Cannot create D2D1 device"
			};
		}
	}

	auto DirectXCompositionWindow::OnNCCreate(
		UINT, WPARAM, LPARAM) -> MessageHandlerResult
	{
		InitSwapChain();
		InitD2D1DeviceContext();
		InitDirectComposition();

		currentMonitor = MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTONULL);

		return { 1, MessageHandlerReturnFlags::PassToDefProc };
	}

	auto DirectXCompositionWindow::OnWindowPosChanged(
		const UINT msg, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		if (const auto monitor = MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTONULL);
			monitor != currentMonitor)
		{
			currentMonitor = monitor;
			const auto& factory = Factories::DWriteFactory::GetFactory();

			ComPtr<IDWriteRenderingParams> renderingParams;
			const auto hr = factory->CreateMonitorRenderingParams(
				currentMonitor,
				renderingParams.put());

			LogIfFailed(
				Error{
					hr
				}
				.AddDetail(L"Window Message", WindowMsgToText(msg)),
				L"Cannot create IDWriteRenderingParams with CreateMonitorRenderingParams"
			);

			if (!FAILED(hr))
			{
				GetAs<ID2D1DeviceContext7, ID2D1RenderTarget>()->SetTextRenderingParams(renderingParams.get());
			}
		}

		return { 0, MessageHandlerReturnFlags::PassToDefProc };
	}

	auto DirectXCompositionWindow::OnPaint(
		UINT, WPARAM, LPARAM) -> MessageHandlerResult
	{
		Draw(GetGraphics());

		return 0;
	}
}

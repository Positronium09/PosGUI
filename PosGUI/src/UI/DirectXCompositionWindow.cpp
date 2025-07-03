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
#include <wrl.h>

module PGUI.UI.DirectXCompositionWindow;

import std;

import PGUI.Logging;
import PGUI.Exceptions;
import PGUI.Utils;
import PGUI.UI.D2D.RenderTarget;
import PGUI.Factories;

namespace PGUI::UI
{
	DirectXCompositionWindow::DirectXCompositionWindow(const WindowClassPtr& wndClass) noexcept :
		Window{ wndClass }
	{
		RegisterHandler(WM_NCCREATE, &DirectXCompositionWindow::OnNCCreate);
		RegisterHandler(WM_SIZE, &DirectXCompositionWindow::OnSize);
		RegisterHandler(WM_WINDOWPOSCHANGED, &DirectXCompositionWindow::OnWindowPosChanged);
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
			hr = S_OK;
		}
		ThrowFailed(hr);

		hr = GetSwapChain()->Present(1, NULL);
		ThrowFailed(hr);

		EndPaint(Hwnd(), &paintStruct);

		return { tag1, tag2 };
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

		const auto hr = dxgiFactory->CreateSwapChainForComposition(
			dxgiDevice.Get(),
			&swapChainDesc, nullptr,
			GetAddress<IDXGISwapChain1>());
		ThrowFailed(hr);
	}

	auto DirectXCompositionWindow::InitD2D1DeviceContext() -> void
	{
		auto& d2d1Dc = GetD2D1DeviceContext();
		const auto& swapChain = GetSwapChain();

		auto hr = d2d1Device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&d2d1Dc);
		ThrowFailed(hr);

		ComPtr<IDXGISurface2> surface;
		hr = swapChain->GetBuffer(0, IID_PPV_ARGS(surface.GetAddressOf()));
		ThrowFailed(hr);

		D2D1_BITMAP_PROPERTIES1 properties = { };
		properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

		ComPtr<ID2D1Bitmap1> bitmap;
		hr = d2d1Dc->CreateBitmapFromDxgiSurface(
			surface.Get(),
			properties,
			&bitmap);
		ThrowFailed(hr);

		d2d1Dc->SetTarget(bitmap.Get());
	}

	auto DirectXCompositionWindow::InitDirectComposition() -> void
	{
		const auto& swapChain = GetSwapChain();
		auto& dcompTarget = GetDCompositionTarget();
		auto& visual = GetDCompositionVisual();

		auto hr = dcompDevice->CreateTargetForHwnd(
			Hwnd(), false,
			&dcompTarget);
		ThrowFailed(hr);

		hr = dcompDevice->CreateVisual(&visual);
		ThrowFailed(hr);

		hr = visual->SetContent(swapChain.Get());
		ThrowFailed(hr);
		hr = dcompTarget->SetRoot(visual.Get());
		ThrowFailed(hr);

		hr = dcompDevice->Commit();
		ThrowFailed(hr);
	}

	auto DirectXCompositionWindow::InitD3D11Device() -> void
	{
		if (d3d11Device)
		{
			return;
		}

		const auto dxgiFactory = Factories::DXGIFactory::GetFactory();
		SYSTEM_POWER_STATUS powerStatus{ };
		GetSystemPowerStatus(&powerStatus);

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
			IID_PPV_ARGS(adapter.GetAddressOf()));
		ThrowFailed(hr);

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
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN, nullptr,
			createDeviceFlags,
			featureLevels.data(),
			// ReSharper disable once CppRedundantCastExpression
			static_cast<UINT>(featureLevels.size()),
			D3D11_SDK_VERSION,
			&device, nullptr, nullptr);
		ThrowFailed(hr);
		hr = device.As(&d3d11Device);
		ThrowFailed(hr);
		hr = d3d11Device.As(&dxgiDevice);
		ThrowFailed(hr);
	}

	auto DirectXCompositionWindow::InitDCompDevice() -> void
	{
		if (dcompDevice)
		{
			return;
		}

		const auto hr = DCompositionCreateDevice(
			dxgiDevice.Get(),
			__uuidof(dcompDevice),
			std::bit_cast<void**>(&dcompDevice));
		ThrowFailed(hr);
	}

	auto DirectXCompositionWindow::InitD2D1Device() -> void
	{
		if (d2d1Device)
		{
			return;
		}

		const auto d2Factory = Factories::D2DFactory::GetFactory();

		const auto hr = d2Factory->CreateDevice(
			dxgiDevice.Get(),
			&d2d1Device);
		ThrowFailed(hr);
	}

	auto DirectXCompositionWindow::OnNCCreate(
		UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) -> MessageHandlerResult
	{
		InitSwapChain();
		InitD2D1DeviceContext();
		InitDirectComposition();

		currentMonitor = MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTONULL);

		return { 1, ReturnFlags::PassToDefProc };
	}

	auto DirectXCompositionWindow::OnWindowPosChanged(
		UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) -> MessageHandlerResult
	{
		if (const auto monitor = MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTONULL);
			monitor != currentMonitor)
		{
			currentMonitor = monitor;
			const auto& factory = Factories::DWriteFactory::GetFactory();

			ComPtr<IDWriteRenderingParams> renderingParams;
			const auto hr = factory->CreateMonitorRenderingParams(
				currentMonitor,
				renderingParams.GetAddressOf());

			LogFailed(LogLevel::Error, hr);

			if (!FAILED(hr))
			{
				GetAs<ID2D1DeviceContext7, ID2D1RenderTarget>()->SetTextRenderingParams(renderingParams.Get());
			}
		}

		return { 0, ReturnFlags::PassToDefProc };
	}

	auto DirectXCompositionWindow::OnSize(
		UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) -> MessageHandlerResult
	{
		const auto& d2d1 = GetD2D1DeviceContext();
		d2d1->SetTarget(nullptr);

		auto size = GetWindowSize();

		if (size.cy == 0)
		{
			size.cy = 1;
		}

		const auto hr = GetSwapChain()->ResizeBuffers(
			0, size.cx, size.cy,
			DXGI_FORMAT_UNKNOWN, NULL);
		ThrowFailed(hr);

		DiscardDeviceResources();
		InitD2D1DeviceContext();

		return 0;
	}
}

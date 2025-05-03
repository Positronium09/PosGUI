module;
#include <winrt/base.h>
#include <winrt/windows.ui.viewmanagement.h>
#include <winrt/windows.foundation.h>
#include <Windows.h>
#include <dwmapi.h>
#include <wrl.h>

module PGUI;

import PGUI.Factories;
import PGUI.UI.SystemTheme;
import PGUI.UI.Styling.Theme;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI
{
	void Init()
	{
		winrt::init_apartment();

		(void)Factories::D2DFactory::GetFactory();
		(void)Factories::DXGIFactory::GetFactory();
		(void)Factories::DWriteFactory::GetFactory();
		(void)Factories::WICFactory::GetFactory();

		UI::SystemTheme::uiSettings.ColorValuesChanged([](
			const winrt::Windows::UI::ViewManagement::UISettings&,
			const winrt::Windows::Foundation::IInspectable&)
		{
			UI::SystemTheme::ColorValuesChanged().InvokeAsync();
		});
		
		UI::Styling::ThemeContext::InitializeThemes();

		UI::SystemTheme::ColorValuesChanged().AddCallback(
			&UI::Styling::ThemeContext::OnSystemThemeChanged
		);

		auto* awarenessContext = GetDpiAwarenessContextForProcess(nullptr);
		auto awareness = GetAwarenessFromDpiAwarenessContext(awarenessContext);
		if (awareness != DPI_AWARENESS_PER_MONITOR_AWARE)
		{
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		}

		UI::DirectXCompositionWindow::InitD3D11Device();
		UI::DirectXCompositionWindow::InitDCompDevice();
		UI::DirectXCompositionWindow::InitD2D1Device();
	}
}

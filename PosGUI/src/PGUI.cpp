module;
#include <winrt/base.h>
#include <winrt/windows.ui.viewmanagement.h>
#include <winrt/windows.foundation.h>
#include <Windows.h>
#include <dwmapi.h>
#include <wrl.h>

module PGUI;

import PGUI.Factories;
import PGUI.UI.Theming.SystemTheme;
import PGUI.UI.Theming.Theme;
import PGUI.UI.DirectXCompositionWindow;
import PGUI.UI.Animation.AnimationManager;
import PGUI.UI.Animation.AnimationTransitionLibrary;

namespace PGUI
{
	void Init()
	{
		winrt::init_apartment();

		BOOL val = FALSE;
		SetUserObjectInformationW(GetCurrentProcess(),
			UOI_TIMERPROC_EXCEPTION_SUPPRESSION,
			&val, sizeof(val));

		(void)Factories::D2DFactory::GetFactory();
		(void)Factories::DXGIFactory::GetFactory();
		(void)Factories::DWriteFactory::GetFactory();
		(void)Factories::WICFactory::GetFactory();
		(void)UI::Animation::AnimationManager::GetInstance();
		(void)UI::Animation::AnimationTransitionLibrary::GetInstance();

		UI::Theming::SystemTheme::uiSettings.ColorValuesChanged([](
			const winrt::Windows::UI::ViewManagement::UISettings&,
			const winrt::Windows::Foundation::IInspectable&)
		{
			UI::Theming::SystemTheme::ColorValuesChanged().InvokeAsync();
		});
		
		UI::Theming::ThemeContext::InitializeThemes();

		UI::Theming::SystemTheme::ColorValuesChanged().AddCallback(
			&UI::Theming::ThemeContext::OnSystemThemeChanged
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

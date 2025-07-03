module;
#include <dwmapi.h>
#include <Windows.h>
#include <wrl.h>
#include <winrt/base.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <winrt/windows.foundation.h>

module PGUI;

import PGUI.Factories;
import PGUI.UI.Theming.SystemTheme;
import PGUI.UI.Theming.Theme;
import PGUI.UI.DirectXCompositionWindow;
import PGUI.UI.Animation;

namespace PGUI
{
	auto Init() -> void
	{
		winrt::init_apartment();

		auto val = FALSE;
		SetUserObjectInformationW(GetCurrentProcess(),
			UOI_TIMERPROC_EXCEPTION_SUPPRESSION,
			&val, sizeof(val));

		(void)Factories::D2DFactory::GetFactory();
		(void)Factories::DXGIFactory::GetFactory();
		(void)Factories::DWriteFactory::GetFactory();
		(void)Factories::WICFactory::GetFactory();
		(void)UI::Animation::AnimationManager::GetInstance();
		(void)UI::Animation::AnimationTransitionLibrary::GetInstance();

		UI::Theming::SystemTheme::RegisterSystemEventChanges();
		
		UI::Theming::ThemeContext::InitializeThemes();
		if (UI::Theming::SystemTheme::IsDarkMode())
		{
			UI::Theming::ThemeContext::ChangeCurrentTheme(UI::Theming::ThemeContext::DarkTheme);
		}
		else
		{
			UI::Theming::ThemeContext::ChangeCurrentTheme(UI::Theming::ThemeContext::LightTheme);
		}

		UI::Theming::SystemTheme::ColorValuesChanged().AddCallback(
			&UI::Theming::ThemeContext::OnSystemThemeChanged
		);

		auto* awarenessContext = GetDpiAwarenessContextForProcess(nullptr);
		if (const auto awareness = GetAwarenessFromDpiAwarenessContext(awarenessContext);
			awareness != DPI_AWARENESS_PER_MONITOR_AWARE)
		{
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		}

		UI::DirectXCompositionWindow::InitD3D11Device();
		UI::DirectXCompositionWindow::InitDCompDevice();
		UI::DirectXCompositionWindow::InitD2D1Device();
	}
}

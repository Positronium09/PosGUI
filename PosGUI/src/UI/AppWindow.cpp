module;
#include <bit>
#include <string>
#include <string_view>
#include <dwmapi.h>
#include <Windows.h>
#include <strsafe.h>

module PGUI.UI.AppWindow;

import PGUI.Window;
import PGUI.UI.Colors;
import PGUI.Logging;

namespace PGUI::UI
{
	AppWindow::AppWindow() noexcept :
		AppWindow{ WindowClass::Create(L"AppWindow") }
	{
	}

	AppWindow::AppWindow(const WindowClassPtr& wndClass) noexcept : 
		DirectXCompositionWindow{ wndClass }
	{
		Hook(autoStop);
		RegisterHandler(WM_NCCREATE, &AppWindow::OnNCCreate);
		RegisterHandler(WM_SETTEXT, &AppWindow::OnSetText);
		RegisterHandler(WM_GETTEXT, &AppWindow::OnGetText);
		RegisterHandler(WM_GETTEXTLENGTH, &AppWindow::OnGetTextLength);
		RegisterHandler(WM_GETMINMAXINFO, &AppWindow::OnGetMinMaxInfo);
		RegisterHandler(WM_LBUTTONDOWN, &AppWindow::OnLButtonDown);
	}

	void AppWindow::EnterFullScreenMode() noexcept
	{
		if (isFullScreen)
		{
			return;
		}
		
		prevPlacement = GetPlacement();

		auto windowStyle = GetStyle();

		MONITORINFO monitorInfo{ };
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfoW(MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

		SetWindowLongPtrW(Hwnd(), GWL_STYLE, windowStyle & ~WS_OVERLAPPEDWINDOW);
		
		SetPosition(monitorInfo.rcMonitor, PositionFlags::NoOwnerZOrder | PositionFlags::FrameChanged, InsertAfter::Top);

		isFullScreen = true;
	}
	void AppWindow::ExitFullScreenMode() noexcept
	{
		if (!isFullScreen)
		{
			return;
		}

		DWORD windowStyle = GetStyle();

		SetWindowLongPtrW(Hwnd(), GWL_STYLE, windowStyle | WS_OVERLAPPEDWINDOW);
		WINDOWPLACEMENT placement = prevPlacement;
		SetWindowPlacement(Hwnd(), &placement);

		SetPosition(RectL{ }, 
			PositionFlags::NoMove | PositionFlags::NoSize | 
			PositionFlags::NoZOrder | PositionFlags::NoOwnerZOrder | 
			PositionFlags::FrameChanged);

		isFullScreen = false;
	}

	void AppWindow::SetTitle(std::wstring_view title) const noexcept
	{
		SendMsg(WM_SETTEXT, 0, std::bit_cast<LPARAM>(title.data()));
	}

	auto AppWindow::IsMaximizable() const noexcept -> bool
	{
		return GetStyle() & WS_MAXIMIZEBOX;
	}
	void AppWindow::SetMaximizable(bool isMaximizable) const noexcept
	{
		if (isMaximizable)
		{
			AddStyle(WS_MAXIMIZEBOX);
			return;
		}
		RemoveStyle(WS_MAXIMIZEBOX);
	}
	auto AppWindow::IsMinimizable() const noexcept -> bool
	{
		return GetStyle() & WS_MINIMIZEBOX;
	}
	void AppWindow::SetMinimizable(bool isMinimizable) const noexcept
	{
		if (isMinimizable)
		{
			AddStyle(WS_MINIMIZEBOX);
			return;
		}
		RemoveStyle(WS_MINIMIZEBOX);
	}
	auto AppWindow::IsCloseable() const noexcept -> bool
	{
		auto menu = GetSystemMenu(Hwnd(), FALSE);
		auto state = GetMenuState(menu, SC_CLOSE, MF_BYCOMMAND);

		return (state & MF_GRAYED) || (state & MF_DISABLED);
	}
	void AppWindow::SetCloseable(bool isCloseable) const noexcept
	{
		auto menu = GetSystemMenu(Hwnd(), FALSE);
		if (isCloseable)
		{
			EnableMenuItem(menu, SC_CLOSE, MF_ENABLED);
			return;
		}
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_DISABLED);
	}
	auto AppWindow::IsAlwaysOnTop() const noexcept -> bool
	{
		return GetExStyle() & WS_EX_TOPMOST;
	}
	void AppWindow::SetAlwaysOnTop(bool isAlwaysOnTop) const noexcept
	{
		if (isAlwaysOnTop)
		{
			AddExStyle(WS_EX_TOPMOST);
			return;
		}
		RemoveExStyle(WS_EX_TOPMOST);
	}
	auto AppWindow::IsResizable() const noexcept -> bool
	{
		return GetStyle() & WS_SIZEBOX;
	}
	void AppWindow::SetResizable(bool isResizable) const noexcept
	{
		if (isResizable)
		{
			AddStyle(WS_SIZEBOX);
			return;
		}
		RemoveStyle(WS_SIZEBOX);
	}

	void AppWindow::SetAutoStopAnimations(bool enable) noexcept
	{
		if (autoStopAnimations == enable)
		{
			return;
		}
		autoStopAnimations = enable;

		if (autoStopAnimations)
		{
			Hook(autoStop);
			return;
		}
		UnHook(autoStop);
	}

	void AppWindow::SetBorderColor(RGBA color) const noexcept
	{
		COLORREF colorRef = color != Colors::Transparent ? static_cast<COLORREF>(color) : DWMWA_COLOR_DEFAULT;

		auto hr = DwmSetWindowAttribute(Hwnd(), 
			DWMWA_BORDER_COLOR, 
			&colorRef, 
			sizeof(colorRef)); LogFailed(LogLevel::Error, hr);
	}

	void AppWindow::SetCaptionColor(RGBA color) const noexcept
	{
		COLORREF colorRef = color != Colors::Transparent ? static_cast<COLORREF>(color) : DWMWA_COLOR_DEFAULT;
		auto hr = DwmSetWindowAttribute(Hwnd(),
			DWMWA_CAPTION_COLOR,
			&colorRef,
			sizeof(colorRef)); LogFailed(LogLevel::Error, hr);
	}

	void AppWindow::SetCaptionTextColor(RGBA color) const noexcept
	{
		COLORREF colorRef = color != Colors::Transparent ? static_cast<COLORREF>(color) : DWMWA_COLOR_DEFAULT;
		auto hr = DwmSetWindowAttribute(Hwnd(),
			DWMWA_TEXT_COLOR,
			&colorRef,
			sizeof(colorRef)); LogFailed(LogLevel::Error, hr);
	}

	void AppWindow::SetCornerPreference(Theming::CornerPreference cornerPreference) const noexcept
	{
		auto preference = static_cast<DWM_WINDOW_CORNER_PREFERENCE>(cornerPreference);
		auto hr = DwmSetWindowAttribute(Hwnd(),
			DWMWA_WINDOW_CORNER_PREFERENCE,
			&preference,
			sizeof(preference)); LogFailed(LogLevel::Error, hr);
	}

	void AppWindow::ApplyStyle(const Theming::AppWindowStyle& style) noexcept
	{
		SetBorderColor(style.borderColor);
		SetCaptionColor(style.captionColor);
		SetCaptionTextColor(style.captionTextColor);
		SetCornerPreference(style.cornerPreference);

		EnableDarkTitleBar(Hwnd(), style.darkMode);
	}

	auto AppWindow::OnNCCreate(UINT /*unused*/, WPARAM /*unused*/, LPARAM lParam) noexcept -> MessageHandlerResult
	{
		auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);

		titleText = createStruct->lpszName;
		createStruct->dwExStyle |= WS_EX_NOREDIRECTIONBITMAP;

		ApplyCurrentTheme();

		return { 1, ReturnFlags::PassToDefProc };
	}
	auto AppWindow::OnSetText(UINT /*unused*/, WPARAM /*unused*/, LPARAM lParam) noexcept -> MessageHandlerResult
	{
		titleText = std::bit_cast<wchar_t*>(lParam);

		return { 1, ReturnFlags::PassToDefProc };
	}
	auto AppWindow::OnGetText(UINT /*unused*/, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		auto size = std::min(titleText.size() + 1, wParam);
		StringCchCopyW(std::bit_cast<wchar_t*>(lParam), size, titleText.data());

		return size;
	}
	auto AppWindow::OnGetTextLength(UINT /*unused*/, WPARAM /*unused*/, LPARAM /*unused*/) const noexcept -> MessageHandlerResult
	{
		return static_cast<LRESULT>(titleText.length());
	}
	auto AppWindow::OnGetMinMaxInfo(UINT /*unused*/, WPARAM /*unused*/, LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		auto* minMaxInfo = std::bit_cast<LPMINMAXINFO>(lParam);

		int frameX = GetSystemMetrics(SM_CXFRAME);
		int frameY = GetSystemMetrics(SM_CYFRAME);
		int padding = GetSystemMetrics(SM_CXPADDEDBORDER);
		minMaxInfo->ptMinTrackSize.y = minSize.cy + frameY + padding;

		minMaxInfo->ptMinTrackSize.x = minSize.cx + 2 * (frameX + padding);

		return 0;
	}
	auto AppWindow::OnLButtonDown(UINT /*unused*/, WPARAM /*unused*/, LPARAM /*unused*/) const noexcept -> MessageHandlerResult
	{
		SetFocus();

		return 0;
	}
}
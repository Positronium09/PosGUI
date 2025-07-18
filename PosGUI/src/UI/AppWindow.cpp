module;
#include <dwmapi.h>
#include <strsafe.h>
#include <Windows.h>

module PGUI.UI.AppWindow;

import std;

import PGUI.Window;
import PGUI.UI.Colors;
import PGUI.Logging;

namespace PGUI::UI
{
	AppWindow::AppWindow() noexcept :
		AppWindow{ WindowClass::Create(L"PGUI_AppWindow") }
	{ }

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

	auto AppWindow::EnterFullScreenMode() noexcept -> void
	{
		if (isFullScreen)
		{
			return;
		}

		prevPlacement = GetPlacement();

		const auto windowStyle = GetStyle();

		MONITORINFO monitorInfo{ };
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfoW(MonitorFromWindow(Hwnd(), MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

		SetWindowLongPtrW(Hwnd(), GWL_STYLE, windowStyle & ~WS_OVERLAPPEDWINDOW);

		SetPosition(monitorInfo.rcMonitor,
		            PositionFlags::NoOwnerZOrder | PositionFlags::FrameChanged,
		            InsertAfter::Top);

		isFullScreen = true;
	}

	auto AppWindow::ExitFullScreenMode() noexcept -> void
	{
		if (!isFullScreen)
		{
			return;
		}

		const auto windowStyle = GetStyle();

		SetWindowLongPtrW(Hwnd(), GWL_STYLE, windowStyle | WS_OVERLAPPEDWINDOW);
		const WINDOWPLACEMENT placement = prevPlacement;
		SetWindowPlacement(Hwnd(), &placement);

		SetPosition(RectL{ },
		            PositionFlags::NoMove | PositionFlags::NoSize |
		            PositionFlags::NoZOrder | PositionFlags::NoOwnerZOrder |
		            PositionFlags::FrameChanged);

		isFullScreen = false;
	}

	auto AppWindow::SetTitle(const std::wstring_view title) const noexcept -> void
	{
		SendMsg(WM_SETTEXT, 0, std::bit_cast<LPARAM>(title.data()));
	}

	auto AppWindow::IsMaximizable() const noexcept -> bool
	{
		return GetStyle() & WS_MAXIMIZEBOX;
	}

	auto AppWindow::SetMaximizable(const bool isMaximizable) const noexcept -> void
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

	auto AppWindow::SetMinimizable(const bool isMinimizable) const noexcept -> void
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
		const auto menu = GetSystemMenu(Hwnd(), FALSE);
		const auto state = GetMenuState(menu, SC_CLOSE, MF_BYCOMMAND);

		return (state & MF_GRAYED) || (state & MF_DISABLED);
	}

	auto AppWindow::SetCloseable(const bool isCloseable) const noexcept -> void
	{
		const auto menu = GetSystemMenu(Hwnd(), FALSE);
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

	auto AppWindow::SetAlwaysOnTop(const bool isAlwaysOnTop) const noexcept -> void
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

	auto AppWindow::SetResizable(const bool isResizable) const noexcept -> void
	{
		if (isResizable)
		{
			AddStyle(WS_SIZEBOX);
			return;
		}
		RemoveStyle(WS_SIZEBOX);
	}

	auto AppWindow::SetAutoStopAnimations(const bool enable) noexcept -> void
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

	auto AppWindow::SetBorderColor(const RGBA color) const noexcept -> void
	{
		const auto colorRef = color != Colors::Transparent ? static_cast<COLORREF>(color) : DWMWA_COLOR_DEFAULT;

		const auto hr = DwmSetWindowAttribute(
			Hwnd(),
			DWMWA_BORDER_COLOR,
			&colorRef,
			sizeof(colorRef));
		LogFailed(LogLevel::Error, hr);
	}

	auto AppWindow::SetCaptionColor(const RGBA color) const noexcept -> void
	{
		const auto colorRef = color != Colors::Transparent ? static_cast<COLORREF>(color) : DWMWA_COLOR_DEFAULT;
		const auto hr = DwmSetWindowAttribute(
			Hwnd(),
			DWMWA_CAPTION_COLOR,
			&colorRef,
			sizeof(colorRef));
		LogFailed(LogLevel::Error, hr);
	}

	auto AppWindow::SetCaptionTextColor(const RGBA color) const noexcept -> void
	{
		const auto colorRef = color != Colors::Transparent ? static_cast<COLORREF>(color) : DWMWA_COLOR_DEFAULT;
		const auto hr = DwmSetWindowAttribute(
			Hwnd(),
			DWMWA_TEXT_COLOR,
			&colorRef,
			sizeof(colorRef));
		LogFailed(LogLevel::Error, hr);
	}

	auto AppWindow::SetCornerPreference(Theming::CornerPreference cornerPreference) const noexcept -> void
	{
		const auto preference = static_cast<DWM_WINDOW_CORNER_PREFERENCE>(cornerPreference);
		const auto hr = DwmSetWindowAttribute(
			Hwnd(),
			DWMWA_WINDOW_CORNER_PREFERENCE,
			&preference,
			sizeof(preference));
		LogFailed(LogLevel::Error, hr);
	}

	auto AppWindow::ApplyStyle(const Theming::AppWindowStyle& style) noexcept -> void
	{
		SetBorderColor(style.borderColor);
		SetCaptionColor(style.captionColor);
		SetCaptionTextColor(style.captionTextColor);
		SetCornerPreference(style.cornerPreference);

		EnableDarkTitleBar(Hwnd(), style.darkMode);
	}

	auto AppWindow::OnNCCreate(UINT /*unused*/, WPARAM /*unused*/, const LPARAM lParam) noexcept -> MessageHandlerResult
	{
		auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);

		titleText = createStruct->lpszName;
		createStruct->dwExStyle |= WS_EX_NOREDIRECTIONBITMAP;

		ApplyCurrentTheme();

		return { 1, HandlerReturnFlags::PassToDefProc };
	}

	auto AppWindow::OnSetText(UINT /*unused*/, WPARAM /*unused*/, const LPARAM lParam) noexcept -> MessageHandlerResult
	{
		titleText = std::bit_cast<wchar_t*>(lParam);

		return { 1, HandlerReturnFlags::PassToDefProc };
	}

	auto AppWindow::OnGetText(UINT /*unused*/, const WPARAM wParam,
	                          const LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		const auto size = std::min(titleText.size() + 1, wParam);
		const auto hr = StringCchCopyW(std::bit_cast<wchar_t*>(lParam), size, titleText.data());
		LogFailed(LogLevel::Error, hr);

		return size;
	}

	auto AppWindow::OnGetTextLength(
		UINT /*unused*/, WPARAM /*unused*/,
		LPARAM /*unused*/) const noexcept -> MessageHandlerResult
	{
		return static_cast<LRESULT>(titleText.length());
	}

	auto AppWindow::OnGetMinMaxInfo(
		UINT /*unused*/, WPARAM /*unused*/,
		const LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		auto* minMaxInfo = std::bit_cast<LPMINMAXINFO>(lParam);

		const auto frameX = GetSystemMetrics(SM_CXFRAME);
		const auto frameY = GetSystemMetrics(SM_CYFRAME);
		const auto padding = GetSystemMetrics(SM_CXPADDEDBORDER);
		minMaxInfo->ptMinTrackSize.y = minSize.cy + frameY + padding;

		minMaxInfo->ptMinTrackSize.x = minSize.cx + 2 * (frameX + padding);

		return 0;
	}

	auto AppWindow::OnLButtonDown(
		UINT /*unused*/, WPARAM /*unused*/,
		LPARAM /*unused*/) const noexcept -> MessageHandlerResult
	{
		SetFocus();

		return 0;
	}
}

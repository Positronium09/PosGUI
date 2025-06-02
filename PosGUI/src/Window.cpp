module;
#include <Windows.h>
#include <dwmapi.h>
#include <bit>
#include <ranges>
#include <variant>
#include <algorithm>
#include <vector>
#include <chrono>

module PGUI.Window;

import PGUI.Utils;
import PGUI.Logging;
import PGUI.Exceptions;


namespace PGUI
{
	Window::Window(const WindowClassPtr& windowClass) noexcept :
		windowClass{ windowClass }
	{
		RegisterHandler(WM_DPICHANGED, &Window::_OnDpiChanged);
	}

	auto Window::OnDpiChanged(UINT /* unused */, RectL suggestedRect) -> MessageHandlerResult
	{
		MoveAndResize(suggestedRect);
		return 0;
	}

	Window::~Window()
	{
		std::ranges::for_each(timerMap | std::views::keys,
			[this](const auto& id)
		{
			KillTimer(hWnd, id);
		});
		DestroyWindow(hWnd);
	}

	void Window::_RegisterHandler(UINT msg, const HandlerHWND& handler)
	{
		messageHandlerMap[msg].push_back(handler);
	}

	void Window::_RegisterHandler(UINT msg, const Handler& handler)
	{
		messageHandlerMap[msg].push_back(handler);
	}

	auto Window::_OnDpiChanged(UINT /* unused */, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult
	{
		return OnDpiChanged(LOWORD(wParam), *std::bit_cast<LPRECT>(lParam));
	}

	void Window::RemoveChildWindow(HWND childHwnd)
	{
		auto found = std::ranges::find_if(childWindows,
			[childHwnd](const auto& wnd) 
		{
			return wnd->hWnd == childHwnd;
		});
		if (found == childWindows.end())
		{
			return;
		}
		auto& childWindow = *found;
		childWindows.erase(found);

		SetParent(childHwnd, nullptr);
		childWindow->parentHwnd = nullptr;
		childWindow->ModifyStyle(WS_POPUP, WS_CHILD);

		OnChildRemoved(childHwnd);
	}

	auto Window::GetParentWindow() const noexcept -> Window*
	{
		if (parentHwnd == nullptr)
		{
			return nullptr;
		}
		return GetWindowPtrFromHWND(parentHwnd);
	}

	void Window::Hook(MessageHooker& hooker) noexcept
	{
		hooker.hookedWindow = this;
		hookers.push_back(hooker);
	}
	void Window::UnHook(MessageHooker& hooker) noexcept
	{
		auto [begin, end] = std::ranges::remove_if(hookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		hooker.hookedWindow = nullptr;
		hookers.erase(begin, end);
	}

	auto Window::AddTimer(TimerId id, std::chrono::milliseconds delay,
		std::optional<TimerCallback> callback) noexcept -> TimerId
	{
		if (TimerId setTimerId =
			SetTimer(hWnd, id, static_cast<UINT>(delay.count()), nullptr);
			setTimerId == 0)
		{
			auto error = GetLastError();
			LogFailed(LogLevel::Error, HresultFromWin32(error));
			return setTimerId;
		}

		if (callback.has_value())
		{
			timerMap[id] = *callback;
		}

		return id;
	}

	void Window::RemoveTimer(TimerId id) noexcept
	{
		if (!HasTimer(id))
		{
			return;
		}

		if (auto succeeded = KillTimer(hWnd, id);
			succeeded == 0)
		{
			if (DWORD error = GetLastError();
				error != ERROR_SUCCESS)
			{
				LogFailed(LogLevel::Error, HresultFromWin32(error));
				return;
			}
		}

		if (timerMap.contains(id))
		{
			timerMap.erase(id);
		}
	}

	auto Window::GetChildWindow(HWND hwnd) const noexcept -> WindowPtr<Window>
	{
		auto result = std::ranges::find_if(childWindows, [hwnd](const auto& wnd) { return wnd.get()->hWnd == hwnd; });

		if (result == childWindows.end())
		{
			return nullptr;
		}

		return *result;
	}

	auto Window::ChildWindowFromPoint(PointL point, UINT flags) const noexcept -> WindowPtr<Window>
	{
		WindowPtr<Window> wnd = nullptr;
		if (HWND hwnd = ChildWindowFromPointEx(hWnd, point, flags);
			hwnd != nullptr)
		{
			return GetChildWindow(hwnd);
		}
		return wnd;
	}

	void Window::AdjustForClientSize(SizeI size) const noexcept
	{
		RECT rc;
		SetRect(&rc, 0, 0, size.cx, size.cy);

		AdjustWindowRectExForDpi(&rc,
			static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE)),
			FALSE,
			static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_EXSTYLE)),
			GetDPI());

		RectL r = rc;
		Resize(r.Size());
	}

	void Window::AdjustForRect(RectI rect) const noexcept
	{
		RECT rc = rect;

		AdjustWindowRectExForDpi(&rc,
			static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE)),
			FALSE,
			static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_EXSTYLE)),
			GetDPI());

		MoveAndResize(rc);
	}

	auto Window::GetWindowRect() const noexcept -> RectL
	{
		RECT windowRect{ };
		::GetWindowRect(hWnd, &windowRect);
		return windowRect;
	}

	auto Window::GetClientRect() const noexcept -> RectL
	{
		RECT clientRect{ };
		::GetClientRect(hWnd, &clientRect);
		return clientRect;
	}

	auto Window::GetWindowSize() const noexcept -> SizeL
	{
		return GetWindowRect().Size();
	}

	auto Window::GetClientSize() const noexcept -> SizeL
	{
		return GetClientRect().Size();
	}

	auto Window::GetPlacement() const noexcept -> WindowPlacement
	{
		WINDOWPLACEMENT placement{ };
		GetWindowPlacement(hWnd, &placement);

		return placement;
	}
	
	auto Window::ScreenToClient(PointL point) const noexcept -> PointL
	{
		POINT p = point;
		::ScreenToClient(hWnd, &p);
		return p;
	}

	auto Window::ScreenToClient(RectL rect) const noexcept -> RectL
	{
		RECT rc = rect;
		::ScreenToClient(hWnd, std::bit_cast<LPPOINT>(&rc));
		::ScreenToClient(hWnd, std::bit_cast<LPPOINT>(&rc.right));
		return rc;
	}

	auto Window::ClientToScreen(PointL point) const noexcept -> PointL
	{
		POINT p = point;
		::ClientToScreen(hWnd, &p);
		return p;
	}

	auto Window::ClientToScreen(RectL rect) const noexcept -> RectL
	{
		RECT rc = rect;
		::ClientToScreen(hWnd, std::bit_cast<LPPOINT>(&rc));
		::ClientToScreen(hWnd, std::bit_cast<LPPOINT>(&rc.right));
		return rc;
	}

	void Window::CenterAroundWindow(WindowPtr<> wnd) const noexcept
	{
		if (wnd == nullptr)
		{
			return;
		}

		const auto rect = wnd->GetWindowRect();
		const auto center = rect.Center();
		const auto centeredWindowRect = GetWindowRect().CenteredAround(center);

		if (parentHwnd != nullptr)
		{
			const auto* parentWindow = GetParentWindow();
			const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);
			SetPosition(centeredClientRect, PositionFlags::NoZOrder);
			return;
		}

		SetPosition(centeredWindowRect, PositionFlags::NoZOrder);
	}
	void Window::CenterAroundWindow(HWND hwnd) const noexcept
	{
		if (hwnd == nullptr)
		{
			return;
		}

		RECT rc;
		::GetWindowRect(hwnd, &rc);

		RectL rect = rc;
		const auto center = rect.Center();
		const auto centeredWindowRect = GetWindowRect().CenteredAround(center);

		if (parentHwnd != nullptr)
		{
			const auto* parentWindow = GetParentWindow();
			const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);
			SetPosition(centeredClientRect, PositionFlags::NoZOrder);
			return;
		}

		SetPosition(centeredWindowRect, PositionFlags::NoZOrder);
	}

	void Window::CenterAroundPoint(PointL point) const noexcept
	{
		const auto centeredWindowRect = GetWindowRect().CenteredAround(point);

		if (parentHwnd != nullptr)
		{
			const auto* parentWindow = GetParentWindow();
			const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);
			SetPosition(centeredClientRect, PositionFlags::NoZOrder);
			return;
		}

		SetPosition(centeredWindowRect, PositionFlags::NoZOrder);
	}
	void Window::CenterAroundRect(RectL rect) const noexcept
	{
		const auto center = rect.Center();
		const auto centeredWindowRect = GetWindowRect().CenteredAround(center);

		if (parentHwnd != nullptr)
		{
			const auto* parentWindow = GetParentWindow();
			const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);
			SetPosition(centeredClientRect, PositionFlags::NoZOrder);
			return;
		}

		SetPosition(centeredWindowRect, PositionFlags::NoZOrder);
	}

	void Window::CenterAroundParent() const noexcept
	{
		if (parentHwnd == nullptr)
		{
			return;
		}

		const auto* parentWindow = GetParentWindow();
		const auto parentRect = parentWindow->GetWindowRect();
		const auto center = parentRect.Center();
		const auto centeredWindowRect = GetWindowRect().CenteredAround(center);
		const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);

		SetPosition(centeredClientRect, PositionFlags::NoZOrder);
	}
	void Window::VerticallyCenterAroundParent() const noexcept
	{
		if (parentHwnd == nullptr)
		{
			return;
		}

		const auto* parentWindow = GetParentWindow();
		const auto parentRect = parentWindow->GetWindowRect();
		const auto center = parentRect.Center();
		const auto windowRect = GetWindowRect();
		auto centeredWindowRect = windowRect.CenteredAround(center);
		centeredWindowRect.left = windowRect.left;
		centeredWindowRect.right = windowRect.right;
		const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);

		SetPosition(centeredClientRect, PositionFlags::NoZOrder);
	}
	void Window::HorizontallyCenterAroundParent() const noexcept
	{
		if (parentHwnd == nullptr)
		{
			return;
		}

		const auto* parentWindow = GetParentWindow();
		const auto parentRect = parentWindow->GetWindowRect();
		const auto center = parentRect.Center();
		const auto windowRect = GetWindowRect();
		auto centeredWindowRect = windowRect.CenteredAround(center);
		centeredWindowRect.top = windowRect.top;
		centeredWindowRect.bottom = windowRect.bottom;
		const auto centeredClientRect = parentWindow->ScreenToClient(centeredWindowRect);

		SetPosition(centeredClientRect, PositionFlags::NoZOrder);
	}

	void Window::ModifyStyle(DWORD add, DWORD remove) const noexcept
	{
		auto style = GetStyle();
		style |= add;
		style &= ~remove;
		SetWindowLongPtrW(hWnd, GWL_STYLE, style);
	}

	void Window::ModifyExStyle(DWORD add, DWORD remove) const noexcept
	{
		auto style = GetExStyle();
		style |= add;
		style &= ~remove;
		SetWindowLongPtrW(hWnd, GWL_EXSTYLE, style);
	}

	void Window::Flash(WindowFlashFlags flags, UINT count, std::chrono::milliseconds timeout) const noexcept
	{
		FLASHWINFO flashInfo{ 
			.cbSize = sizeof(FLASHWINFO), 
			.hwnd = hWnd, 
			.dwFlags = ToUnderlying(flags), 
			.uCount = count,
			.dwTimeout = static_cast<DWORD>(timeout.count()) 
		};
		FlashWindowEx(&flashInfo);
	}

	void Window::StopFlash() const noexcept
	{
		FLASHWINFO flashInfo{ 
			.cbSize = sizeof(FLASHWINFO), 
			.hwnd = hWnd, 
			.dwFlags = FLASHW_STOP,
			.uCount = 0,
			.dwTimeout = 0
		};
		FlashWindowEx(&flashInfo);
	}

	void Window::SetPosition(PointL position, SizeL size, PositionFlags flags, HWND insertAfter) const noexcept
	{
		SetWindowPos(hWnd, insertAfter, position.x, position.y,
			size.cx, size.cy, static_cast<UINT>(flags));
	}

	void Window::SetPosition(RectL rect, PositionFlags flags, HWND insertAfter) const noexcept
	{
		SetPosition(rect.TopLeft(), rect.Size(), flags, insertAfter);
	}

	void Window::Move(PointL newPos) const noexcept
	{
		SetPosition(newPos, SizeL{ }, 
			PositionFlags::NoSize | PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	void Window::Resize(SizeL newSize) const noexcept
	{
		SetPosition(PointL{ }, newSize,
			PositionFlags::NoMove | PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	void Window::MoveAndResize(RectL newRect) const noexcept
	{
		const auto size = newRect.Size();

		SetPosition(newRect.TopLeft(), size,
			PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	void Window::MoveAndResize(PointL newPos, SizeL newSize) const noexcept
	{
		SetPosition(newPos, newSize,
			PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::MapPoints(HWND hWndTo, std::span<PointL> points) const noexcept -> std::span<PointL>
	{
		return PGUI::MapPoints(hWnd, hWndTo, points);
	}

	auto Window::MapPoint(HWND hWndTo, PointL point) const noexcept -> PointL
	{
		return PGUI::MapPoint(hWnd, hWndTo, point);
	}

	auto Window::MapRects(HWND hWndTo, std::span<RectL> rects) const noexcept -> std::span<RectL>
	{
		return PGUI::MapRects(hWnd, hWndTo, rects);
	}

	auto Window::MapRect(HWND hWndTo, RectL rect) const noexcept -> RectL
	{
		return PGUI::MapRect(hWnd, hWndTo, rect);
	}

	auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		if (msg == WM_NCCREATE) [[unlikely]]
		{
			auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);
			auto* window = std::bit_cast<Window*>(createStruct->lpCreateParams);

			window->hWnd = hWnd;
			window->parentHwnd = createStruct->hwndParent;

			SetWindowLongPtrW(hWnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(window));
		}
		
		if (LRESULT result = 0;
			DwmDefWindowProc(hWnd, msg, wParam, lParam, &result)) [[unlikely]]
		{
			return result;
		}

		RawWindowPtr<Window> window = GetWindowPtrFromHWND(hWnd);

		if (msg == WM_TIMER)
		{
			auto timerId = wParam;
			if (window->timerMap.contains(timerId))
			{
				const auto& callback = window->timerMap.at(timerId);
				callback(timerId);
				return 0;
			}
		}

		if (!window) [[unlikely]]
		{
			return DefWindowProcW(hWnd, msg, wParam, lParam);
		}

		MessageHandlerResult result{ 0 };
		bool hookerHandled = false;

		for (const auto& hooker : window->hookers)
		{
			const auto& handlers = hooker.get().GetHandlers();
			if (!handlers.contains(msg))
			{
				continue;
			}

			for (const auto& messageHandlers : handlers.at(msg))
			{
				std::visit([&](const auto& handler)
				{
					using T = std::decay_t<decltype(handler)>;
					if constexpr (std::is_same_v<T, HandlerHWND>)
					{
						result = handler(hWnd, msg, wParam, lParam);
					}
					else if constexpr (std::is_same_v<T, Handler>)
					{
						result = handler(msg, wParam, lParam);
					}
				}, messageHandlers);

				hookerHandled = true;

				if (IsFlagSet(result.flags, ReturnFlags::ForceThisResult)) [[unlikely]]
				{
					return result.result;
				}
			}
		}

		if (!window->messageHandlerMap.contains(msg))
		{
			if (hookerHandled)
			{
				return result;
			}
			return DefWindowProcW(hWnd, msg, wParam, lParam);
		}
		
		for (const auto& handlerVariant : window->messageHandlerMap.at(msg))
		{
			std::visit([&](const auto& handler)
			{
				using T = std::decay_t<decltype(handler)>;
				if constexpr (std::is_same_v<T, HandlerHWND>)
				{
					result = handler(hWnd, msg, wParam, lParam);
				}
				else if constexpr (std::is_same_v<T, Handler>)
				{
					result = handler(msg, wParam, lParam);
				}
			}, handlerVariant);
		}
		if (IsFlagSet(result.flags, ReturnFlags::PassToDefProc)) [[unlikely]]
		{
			DefWindowProcW(hWnd, msg, wParam, lParam);
		}

		if (msg == WM_NCCREATE) [[unlikely]]
		{
			auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);

			if (createStruct->style & WS_CHILD)
			{
				window->parentHwnd = createStruct->hwndParent;
				createStruct->x = AdjustForDPI(createStruct->x,
					static_cast<float>(GetDpiForWindow(hWnd)));
				createStruct->y = AdjustForDPI(createStruct->y,
					static_cast<float>(GetDpiForWindow(hWnd)));
			}

			createStruct->cx = AdjustForDPI(createStruct->cx,
				static_cast<float>(GetDpiForWindow(hWnd)));
			createStruct->cy = AdjustForDPI(createStruct->cy,
				static_cast<float>(GetDpiForWindow(hWnd)));

			RectI rect{
				PointI{ createStruct->x, createStruct->y },
				SizeI{ createStruct->cx, createStruct->cy } };

			window->AdjustForRect(rect);
		}

		return result;
	}
}

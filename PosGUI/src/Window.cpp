module;
#include <dwmapi.h>
#include <ranges>
#include <Windows.h>

module PGUI.Window;

import std;

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

	auto Window::OnDpiChanged(UINT /* unused */, const RectL suggestedRect) -> MessageHandlerResult
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
		timerMap.clear();

		std::ranges::for_each(childWindows,
			[](const auto& childWindow)
		{
			childWindow->parentHwnd = nullptr;
			childWindow->hWnd = nullptr;
		});
		childWindows.clear();

		
		std::ranges::for_each(beforeHookers,
			[](auto& hook)
		{
			hook.get().hookedWindow = nullptr;
		});
		std::ranges::for_each(afterHookers,
			[](auto& hook)
		{
			hook.get().hookedWindow = nullptr;
		});
		beforeHookers.clear();
		afterHookers.clear();

		hWnd = nullptr;
		parentHwnd = nullptr;

		DestroyWindow(hWnd);
	}

	// ReSharper disable CppInconsistentNaming
	auto Window::_RegisterHandler(const UINT msg, const HandlerHWND& handler) -> void
	{
		messageHandlerMap[msg].push_back(handler);
	}

	auto Window::_RegisterHandler(const UINT msg, const Handler& handler) -> void
	{
		messageHandlerMap[msg].push_back(handler);
	}

	auto Window::_OnDpiChanged(UINT /* unused */, const WPARAM wParam, const LPARAM lParam) -> MessageHandlerResult
	{
		return OnDpiChanged(LOWORD(wParam), *std::bit_cast<LPRECT>(lParam));
	}

	// ReSharper restore CppInconsistentNaming

	auto Window::RemoveChildWindow(HWND childHwnd) -> void
	{
		const auto found = std::ranges::find_if(childWindows,
		                                        [childHwnd](const auto& wnd) 
		                                        {
			                                        return wnd->hWnd == childHwnd;
		                                        });
		if (found == childWindows.end())
		{
			return;
		}
		const auto& childWindow = *found;
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

	auto Window::Hook(MessageHooker& hooker) noexcept -> void
	{
		if (hooker.hookedWindow != nullptr)
		{
			hooker.hookedWindow->UnHook(hooker);
		}
		hooker.hookedWindow = this;
		beforeHookers.push_back(hooker);
		afterHookers.push_back(hooker);
	}

	auto Window::HookBefore(MessageHooker& hooker) noexcept -> void
	{
		if (hooker.hookedWindow != nullptr)
		{
			hooker.hookedWindow->UnHookBefore(hooker);
		}
		hooker.hookedWindow = this;
		beforeHookers.push_back(hooker);
	}

	auto Window::HookAfter(MessageHooker& hooker) noexcept -> void
	{
		if (hooker.hookedWindow != nullptr)
		{
			hooker.hookedWindow->UnHookAfter(hooker);
		}
		hooker.hookedWindow = this;
		afterHookers.push_back(hooker);
	}

	auto Window::UnHook(MessageHooker& hooker) noexcept -> void
	{
		hooker.hookedWindow = nullptr;

		auto [beforeBegin, beforeEnd] = std::ranges::remove_if(beforeHookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		beforeHookers.erase(beforeBegin, beforeEnd);

		auto [afterBegin, afterEnd] = std::ranges::remove_if(afterHookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		afterHookers.erase(afterBegin, afterEnd);
	}

	auto Window::UnHookBefore(MessageHooker& hooker) noexcept -> void
	{
		hooker.hookedWindow = nullptr;

		auto [begin, end] = std::ranges::remove_if(beforeHookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		beforeHookers.erase(begin, end);
	}

	auto Window::UnHookAfter(MessageHooker& hooker) noexcept -> void
	{
		hooker.hookedWindow = nullptr;

		auto [begin, end] = std::ranges::remove_if(afterHookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		afterHookers.erase(begin, end);
	}

	auto Window::AddTimer(const TimerId id, const std::chrono::milliseconds delay,
	                      std::optional<TimerCallback> callback) noexcept -> TimerId
	{
		if (const auto setTimerId =
			SetTimer(hWnd, id, static_cast<UINT>(delay.count()), nullptr);
			setTimerId == 0)
		{
			const auto error = GetLastError();
			LogFailed(LogLevel::Error, HresultFromWin32(error));
			return setTimerId;
		}

		if (callback.has_value())
		{
			timerMap[id] = *callback;
		}

		return id;
	}

	auto Window::RemoveTimer(const TimerId id) noexcept -> void
	{
		if (!HasTimer(id))
		{
			return;
		}

		if (const auto succeeded = KillTimer(hWnd, id);
			succeeded == 0)
		{
			if (const auto error = GetLastError();
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
		const auto result = std::ranges::find_if(childWindows, [hwnd](const auto& wnd)
		{
			return wnd.get()->hWnd == hwnd;
		});

		if (result == childWindows.end())
		{
			return nullptr;
		}

		return *result;
	}

	auto Window::ChildWindowFromPoint(const PointL point, const UINT flags) const noexcept -> WindowPtr<Window>
	{
		WindowPtr<Window> wnd = nullptr;
		if (const HWND hwnd = ChildWindowFromPointEx(hWnd, point, flags);
			hwnd != nullptr)
		{
			return GetChildWindow(hwnd);
		}
		return wnd;
	}

	auto Window::AdjustForClientSize(const SizeI size) const noexcept -> void
	{
		RECT rc;
		SetRect(&rc, 0, 0, size.cx, size.cy);

		AdjustWindowRectExForDpi(&rc,
			static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE)),
			FALSE,
			static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_EXSTYLE)),
			GetDPI());

		const RectL r = rc;
		Resize(r.Size());
	}

	auto Window::AdjustForRect(const RectI rect) const noexcept -> void
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

	auto Window::ScreenToClient(const PointL point) const noexcept -> PointL
	{
		POINT p = point;
		::ScreenToClient(hWnd, &p);
		return p;
	}

	auto Window::ScreenToClient(const RectL rect) const noexcept -> RectL
	{
		RECT rc = rect;
		::ScreenToClient(hWnd, std::bit_cast<LPPOINT>(&rc));
		::ScreenToClient(hWnd, std::bit_cast<LPPOINT>(&rc.right));
		return rc;
	}

	auto Window::ClientToScreen(const PointL point) const noexcept -> PointL
	{
		POINT p = point;
		::ClientToScreen(hWnd, &p);
		return p;
	}

	auto Window::ClientToScreen(const RectL rect) const noexcept -> RectL
	{
		RECT rc = rect;
		::ClientToScreen(hWnd, std::bit_cast<LPPOINT>(&rc));
		::ClientToScreen(hWnd, std::bit_cast<LPPOINT>(&rc.right));
		return rc;
	}

	auto Window::CenterAroundWindow(WindowPtr<> wnd) const noexcept -> void
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

	auto Window::CenterAroundWindow(const HWND hwnd) const noexcept -> void
	{
		if (hwnd == nullptr)
		{
			return;
		}

		RECT rc;
		::GetWindowRect(hwnd, &rc);

		const RectL rect = rc;
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

	auto Window::CenterAroundPoint(const PointL point) const noexcept -> void
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

	auto Window::CenterAroundRect(const RectL rect) const noexcept -> void
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

	auto Window::CenterAroundParent() const noexcept -> void
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

	auto Window::VerticallyCenterAroundParent() const noexcept -> void
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

	auto Window::HorizontallyCenterAroundParent() const noexcept -> void
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

	auto Window::ModifyStyle(const DWORD add, const DWORD remove) const noexcept -> void
	{
		auto style = GetStyle();
		style |= add;
		style &= ~remove;
		SetWindowLongPtrW(hWnd, GWL_STYLE, style);
	}

	auto Window::ModifyExStyle(const DWORD add, const DWORD remove) const noexcept -> void
	{
		auto style = GetExStyle();
		style |= add;
		style &= ~remove;
		SetWindowLongPtrW(hWnd, GWL_EXSTYLE, style);
	}

	auto Window::Flash(const WindowFlashFlags flags, const UINT count,
	                   const std::chrono::milliseconds timeout) const noexcept -> void
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

	auto Window::StopFlash() const noexcept -> void
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

	auto Window::SetPosition(const PointL position, const SizeL size, PositionFlags flags,
	                         const HWND insertAfter) const noexcept -> void
	{
		SetWindowPos(hWnd, insertAfter, position.x, position.y,
			size.cx, size.cy, static_cast<UINT>(flags));
	}

	auto Window::SetPosition(const RectL rect, const PositionFlags flags, const HWND insertAfter) const noexcept -> void
	{
		SetPosition(rect.TopLeft(), rect.Size(), flags, insertAfter);
	}

	auto Window::Move(const PointL newPos) const noexcept -> void
	{
		SetPosition(newPos, SizeL{ }, 
			PositionFlags::NoSize | PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::Resize(const SizeL newSize) const noexcept -> void
	{
		SetPosition(PointL{ }, newSize,
			PositionFlags::NoMove | PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::MoveAndResize(const RectL newRect) const noexcept -> void
	{
		const auto size = newRect.Size();

		SetPosition(newRect.TopLeft(), size,
			PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::MoveAndResize(const PointL newPos, const SizeL newSize) const noexcept -> void
	{
		SetPosition(newPos, newSize,
			PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::MapPoints(const HWND hWndTo, const std::span<PointL> points) const noexcept -> std::span<PointL>
	{
		return PGUI::MapPoints(hWnd, hWndTo, points);
	}

	auto Window::MapPoint(const HWND hWndTo, const PointL point) const noexcept -> PointL
	{
		return PGUI::MapPoint(hWnd, hWndTo, point);
	}

	auto Window::MapRects(const HWND hWndTo, const std::span<RectL> rects) const noexcept -> std::span<RectL>
	{
		return PGUI::MapRects(hWnd, hWndTo, rects);
	}

	auto Window::MapRect(const HWND hWndTo, const RectL rect) const noexcept -> RectL
	{
		return PGUI::MapRect(hWnd, hWndTo, rect);
	}

	// ReSharper disable once CppInconsistentNaming
	auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		if (msg == WM_NCCREATE) [[unlikely]]
		{
			const auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);
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

		const RawWindowPtr<Window> window = GetWindowPtrFromHWND(hWnd);

		if (msg == WM_TIMER)
		{
			const auto timerId = wParam;
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
		auto hookerHandled = false;

		for (const auto& hooker : window->beforeHookers)
		{
			const auto& handlers = hooker.get().GetHandlers();
			if (!handlers.contains(msg))
			{
				continue;
			}

			for (const auto& messageHandlers : handlers.at(msg))
			{
				std::visit([&]<typename Func>(const Func& handler)
				{
					using T = std::decay_t<Func>;
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
			result = DefWindowProcW(hWnd, msg, wParam, lParam);
		}
		else
		{
			for (const auto& handlerVariant : window->messageHandlerMap.at(msg))
			{
				std::visit([&]<typename Func>(const Func& handler)
				{
					using T = std::decay_t<Func>;
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
		}

		for (const auto& hooker : window->afterHookers)
		{
			const auto& handlers = hooker.get().GetHandlers();
			if (!handlers.contains(msg))
			{
				continue;
			}

			for (const auto& messageHandlers : handlers.at(msg))
			{
				std::visit([&]<typename Func>(const Func& handler)
				{
					using T = std::decay_t<Func>;
					if constexpr (std::is_same_v<T, HandlerHWND>)
					{
						result = handler(hWnd, msg, wParam, lParam);
					}
					else if constexpr (std::is_same_v<T, Handler>)
					{
						result = handler(msg, wParam, lParam);
					}
				}, messageHandlers);

				if (IsFlagSet(result.flags, ReturnFlags::ForceThisResult)) [[unlikely]]
				{
					return result.result;
				}
			}
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

			const RectI rect{
				PointI{ createStruct->x, createStruct->y },
				SizeI{ createStruct->cx, createStruct->cy } };

			window->AdjustForRect(rect);
		}

		return result;
	}
}

module;
#include <dwmapi.h>
#include <ranges>
#include <Windows.h>

module PGUI.Window;

import std;

import PGUI.Utils;
import PGUI.ScopedTimer;
import PGUI.DpiScaled;
import PGUI.ErrorHandling;

namespace PGUI
{
	static auto SetWindowPtrToHWND(const HWND hWnd, const RawWindowPtr<const Window> ptr) noexcept -> void
	{
		SetWindowLongPtrW(hWnd, WindowPointerOffset, std::bit_cast<LONG_PTR>(ptr));
	}

	Window::Window(const WindowClassPtr& windowClass) noexcept :
		windowClass{ windowClass }
	{
		RegisterHandler(WM_DPICHANGED, &Window::_OnDpiChanged);
		RegisterHandler(WM_DPICHANGED_AFTERPARENT, &Window::_OnDpiChanged);
		RegisterHandler(WM_DPICHANGED_BEFOREPARENT, &Window::_OnDpiChanged);
		RegisterHandler(WM_WINDOWPOSCHANGED, &Window::_OnWindowPosChanged);
		RegisterHandler(WM_SIZE, &Window::_OnSize);
		RegisterHandler(WM_MOVE, &Window::_OnMove);
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


		std::ranges::for_each(
			beforeHookers,
			[](auto& hook)
			{
				hook.get().UnhookFromWindow();
			});

		std::ranges::for_each(
			afterHookers,
			[](auto& hook)
			{
				hook.get().UnhookFromWindow();
			});

		beforeHookers.clear();
		afterHookers.clear();

		parentHwnd = nullptr;
		if (const auto copyHwnd = hWnd;
			copyHwnd != nullptr)
		{
			hWnd = nullptr;

			SetWindowPtrToHWND(copyHwnd, nullptr);
			DestroyWindow(copyHwnd);
		}
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

	auto Window::_OnDpiChanged(const UINT msg, const WPARAM, const LPARAM lParam) -> MessageHandlerResult
	{
		LRESULT result = 0;
		if (const auto error = logicalRect.SetDpi(GetDpi());
			error.IsFailure())
		{
			Logger::Error(error, L"SetDpi failed in _OnDpiChanged");
		}

		if (msg == WM_DPICHANGED)
		{
			logicalRect.SetPhysicalValue(*std::bit_cast<LPRECT>(lParam));
			result = OnDpiChanged(GetDpi());
		}
		else if (msg == WM_DPICHANGED_AFTERPARENT)
		{
			result = OnDpiChangedAfterParent(GetDpi());
		}
		else if (msg == WM_DPICHANGED_BEFOREPARENT)
		{
			result = OnDpiChangedBeforeParent(GetDpi());
		}

		Redraw();

		return result;
	}

	auto Window::_OnWindowPosChanged(UINT, WPARAM, const LPARAM lParam) -> MessageHandlerResult
	{
		const auto windowPos = *std::bit_cast<LPWINDOWPOS>(lParam);
		const RectF windowRect{
			static_cast<float>(windowPos.x),
			static_cast<float>(windowPos.y),
			static_cast<float>(windowPos.x + windowPos.cx),
			static_cast<float>(windowPos.y + windowPos.cy)
		};

		const auto windowSize = windowRect.Size();
		auto rect = logicalRect.GetPhysicalValue();
		const auto flags = static_cast<PositionFlags>(windowPos.flags);

		if (!IsFlagSet(flags, PositionFlags::NoClientMove))
		{
			rect.left = windowRect.left;
			rect.top = windowRect.top;
			rect.right = rect.left + windowSize.cx;
			rect.bottom = rect.top + windowSize.cy;
		}
		if (!IsFlagSet(flags, PositionFlags::NoClientSize))
		{
			rect.right = rect.left + windowSize.cx;
			rect.bottom = rect.top + windowSize.cy;
		}
		logicalRect.SetPhysicalValue(rect);

		if (!IsFlagSet(flags, PositionFlags::NoClientMove))
		{
			[[maybe_unused]]
				const PointL point{
					static_cast<long>(windowRect.left),
					static_cast<long>(windowRect.top)
			};
			OnMoved(logicalRect->TopLeft());
			//SendMsg(WM_MOVE, NULL, MAKELONG(point.x, point.y));
		}
		if (!IsFlagSet(flags, PositionFlags::NoClientSize) || IsFlagSet(flags, PositionFlags::StateChanged))
		{
			[[maybe_unused]]
			auto wparam = SIZE_RESTORED;
			if (IsMinimized())
			{
				wparam = SIZE_MINIMIZED;
			}
			else if (IsMaximized())
			{
				wparam = SIZE_MAXIMIZED;
			}
			OnSizeChanged(logicalRect->Size());
			//SendMsg(WM_SIZE, wparam, MAKELONG(windowSize.cx, windowSize.cy));
		}

		return 0;
	}

	auto Window::_OnSize(UINT, WPARAM, const LPARAM) -> MessageHandlerResult
	{
		OnSizeChanged(logicalRect->Size());

		return 0;
	}

	auto Window::_OnMove(UINT, WPARAM, const LPARAM) -> MessageHandlerResult
	{
		OnMoved(logicalRect->TopLeft());

		return 0;
	}

	// ReSharper restore CppInconsistentNaming

	auto Window::RemoveChildWindow(const HWND childHwnd) -> void
	{
		const auto found = std::ranges::find_if(
			childWindows,
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
		if (hooker.HookedWindow() != nullptr)
		{
			hooker.HookedWindow()->UnHook(hooker);
		}
		hooker.HookToWindow(this);
		beforeHookers.push_back(hooker);
		afterHookers.push_back(hooker);
	}

	auto Window::HookBefore(MessageHooker& hooker) noexcept -> void
	{
		if (hooker.HookedWindow() != nullptr)
		{
			hooker.HookedWindow()->UnHookBefore(hooker);
		}
		hooker.HookToWindow(this);
		beforeHookers.push_back(hooker);
	}

	auto Window::HookAfter(MessageHooker& hooker) noexcept -> void
	{
		if (hooker.HookedWindow() != nullptr)
		{
			hooker.HookedWindow()->UnHookAfter(hooker);
		}
		hooker.HookToWindow(this);
		afterHookers.push_back(hooker);
	}

	auto Window::UnHook(MessageHooker& hooker) noexcept -> void
	{
		hooker.UnhookFromWindow();

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
		hooker.UnhookFromWindow();

		auto [begin, end] = std::ranges::remove_if(beforeHookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		beforeHookers.erase(begin, end);
	}

	auto Window::UnHookAfter(MessageHooker& hooker) noexcept -> void
	{
		hooker.UnhookFromWindow();

		auto [begin, end] = std::ranges::remove_if(afterHookers, [&hooker](const auto& hook)
		{
			return &hooker == &(hook.get());
		});

		afterHookers.erase(begin, end);
	}

	auto Window::AddTimer(const TimerId id, const std::chrono::milliseconds delay,
	                      const std::optional<TimerCallback>& callback) noexcept -> TimerId
	{
		if (const auto setTimerId =
			SetTimer(hWnd, id, static_cast<UINT>(delay.count()), nullptr);
			setTimerId == 0)
		{
			const auto error = GetLastError();

			Logger::Error(
				Error{ error }
			);

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
				Logger::Error(
					Error{ error }
				);

				return;
			}
		}

		if (timerMap.contains(id))
		{
			timerMap.erase(id);
		}
	}

	auto Window::GetChildWindow(const HWND hwnd) const noexcept -> WindowPtr<Window>
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

	auto Window::ChildWindowFromPoint(const PointF point) const noexcept -> WindowPtr<Window>
	{
		auto hwnd = ::ChildWindowFromPoint(Hwnd(), LogicalToPhysical(point));

		if (const auto result = std::ranges::find_if(
				childWindows,
				[hwnd](const auto& wnd)
				{
					return wnd->Hwnd() == hwnd;
				});
			result != childWindows.end())
		{
			return *result;
		}

		return nullptr;
	}

	auto Window::GetWindowRect() const noexcept -> RectF
	{
		RECT rc;
		::GetWindowRect(Hwnd(), &rc);
		return PhysicalToLogical(RectF{ rc });
	}

	auto Window::GetClientRect() const noexcept -> RectF
	{
		RECT rc;
		::GetClientRect(Hwnd(), &rc);
		return PhysicalToLogical(RectF{ rc });
	}

	auto Window::GetWindowSize() const noexcept -> SizeF
	{
		return GetWindowRect().Size();
	}

	auto Window::GetClientSize() const noexcept -> SizeF
	{
		return GetClientRect().Size();
	}

	auto Window::GetParentRelativeRect() const noexcept -> RectF
	{
		return PhysicalToLogical(MapRectToParent(LogicalToPhysical(GetClientRect())));
	}

	auto Window::GetPlacement() const noexcept -> WindowPlacement
	{
		WINDOWPLACEMENT placement{ };
		GetWindowPlacement(hWnd, &placement);

		return placement;
	}

	auto Window::ScreenToClient(const PointF point) const noexcept -> PointF
	{
		PointL physical = LogicalToPhysical(point);
		::ScreenToClient(Hwnd(), std::bit_cast<LPPOINT>(&physical));
		return PhysicalToLogical<PointF>(physical);
	}

	auto Window::ScreenToClient(const RectF rect) const noexcept -> RectF
	{
		RectL physical = LogicalToPhysical(rect);
		::ScreenToClient(Hwnd(), std::bit_cast<LPPOINT>(&physical));
		::ScreenToClient(Hwnd(), std::next(std::bit_cast<LPPOINT>(&physical)));
		return PhysicalToLogical<RectF>(physical);
	}

	auto Window::ClientToScreen(const PointF point) const noexcept -> PointF
	{
		PointL physical = LogicalToPhysical(point);
		::ClientToScreen(Hwnd(), std::bit_cast<LPPOINT>(&physical));
		return PhysicalToLogical<PointF>(physical);
	}

	auto Window::ClientToScreen(const RectF rect) const noexcept -> RectF
	{
		RectL physical = LogicalToPhysical(rect);
		::ClientToScreen(Hwnd(), std::bit_cast<LPPOINT>(&physical));
		::ClientToScreen(Hwnd(), std::next(std::bit_cast<LPPOINT>(&physical)));
		return PhysicalToLogical<RectF>(physical);
	}

	auto Window::CenterAroundWindow(const WindowPtr<>& wnd) noexcept -> void
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

	auto Window::CenterAroundWindow(const HWND hwnd) noexcept -> void
	{
		if (hwnd == nullptr)
		{
			return;
		}

		RECT rc;
		::GetWindowRect(hwnd, &rc);

		const RectL rect = PhysicalToLogical(RectF{ rc });
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

	auto Window::CenterAroundPoint(const PointF point) noexcept -> void
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

	auto Window::CenterAroundRect(const RectF rect) noexcept -> void
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


	auto Window::CenterAroundParent() noexcept -> void
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

	auto Window::VerticallyCenterAroundParent() noexcept -> void
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

	auto Window::HorizontallyCenterAroundParent() noexcept -> void
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

	auto Window::SetPosition(const PointF position, const SizeF size,
	                         const PositionFlags flags, const HWND insertAfter) noexcept -> void
	{
		RectF rect = *logicalRect;
		if (!IsFlagSet(flags, PositionFlags::NoMove))
		{
			const auto rectSize = rect.Size();
			rect.left = position.x;
			rect.top = position.y;
			rect.right = rect.left + rectSize.cx;
			rect.bottom = rect.top + rectSize.cy;
		}
		if (!IsFlagSet(flags, PositionFlags::NoSize))
		{
			rect.right = rect.left + size.cx;
			rect.bottom = rect.top + size.cy;
		}
		logicalRect.SetLogicalValue(rect);
		const RectL physicalRect = logicalRect.GetPhysicalValue();

		SetWindowPos(hWnd, insertAfter,
		             physicalRect.left, physicalRect.top,
		             physicalRect.right - physicalRect.left,
		             physicalRect.bottom - physicalRect.top,
		             ToUnderlying(flags));
	}

	auto Window::SetPosition(const RectF rect, const PositionFlags flags, const HWND insertAfter) noexcept -> void
	{
		SetPosition(rect.TopLeft(), rect.Size(), flags, insertAfter);
	}

	auto Window::Move(const PointF newPos) noexcept -> void
	{
		SetPosition(newPos, SizeF{ },
		            PositionFlags::NoSize | PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::Resize(const SizeF newSize) noexcept -> void
	{
		SetPosition(PointF{ }, newSize,
		            PositionFlags::NoMove | PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::MoveAndResize(const RectF newRect) noexcept -> void
	{
		SetPosition(newRect, PositionFlags::NoZOrder | PositionFlags::NoActivate);
	}

	auto Window::MoveAndResize(const PointF newPos, const SizeF newSize) noexcept -> void
	{
		SetPosition(newPos, newSize, PositionFlags::NoZOrder | PositionFlags::NoActivate);
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

	auto Window::MapPointsToParent(const std::span<PointL> points) const noexcept -> std::span<PointL>
	{
		return MapPoints(parentHwnd, points);
	}

	auto Window::MapPointToParent(const PointL point) const noexcept -> PointL
	{
		return MapPoint(parentHwnd, point);
	}

	auto Window::MapRectsToParent(const std::span<RectL> rects) const noexcept -> std::span<RectL>
	{
		return MapRects(parentHwnd, rects);
	}

	auto Window::MapRectToParent(const RectF rect) const noexcept -> RectF
	{
		return MapRect(parentHwnd, rect);
	}

	// ReSharper disable once CppInconsistentNaming
	auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		DebugTimer timer{
			std::format(L"MSG {}", WindowMsgToText(msg))
		};

		if (msg == WM_NCCREATE) [[unlikely]]
		{
			const auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);
			auto* window = std::bit_cast<Window*>(createStruct->lpCreateParams);

			window->hWnd = hWnd;
			window->parentHwnd = createStruct->hwndParent;
			if (const auto error = window->logicalRect.SetDpi(window->GetDpi());
				error.IsFailure())
			{
				Logger::Error(error, L"SetDpi failed in WM_NCCREATE");
			}

			SetWindowPtrToHWND(hWnd, window);
		}

		if (LRESULT result = 0;
			DwmDefWindowProc(hWnd, msg, wParam, lParam, &result)) [[unlikely]]
		{
			return result;
		}

		const RawWindowPtr<Window> window = GetWindowPtrFromHWND(hWnd);
		if (window == nullptr) [[unlikely]]
		{
			return DefWindowProcW(hWnd, msg, wParam, lParam);
		}

		if (msg == WM_TIMER)
		{
			if (const auto timerId = wParam;
				window->timerMap.contains(timerId))
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

				if (IsFlagSet(result.flags, MessageHandlerReturnFlags::ForceThisResult)) [[unlikely]]
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

				if (IsFlagSet(result.flags, MessageHandlerReturnFlags::NoFurtherHandling)) [[unlikely]]
				{
					break;
				}
			}
			if (IsFlagSet(result.flags, MessageHandlerReturnFlags::PassToDefProc)) [[unlikely]]
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

				if (IsFlagSet(result.flags, MessageHandlerReturnFlags::ForceThisResult)) [[unlikely]]
				{
					return result.result;
				}
			}
		}

		if (msg == WM_NCCREATE) [[unlikely]]
		{
			const auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(lParam);
			RectL rc = RectF{
				           0,
				           0,
				           static_cast<float>(createStruct->cx),
				           static_cast<float>(createStruct->cy)
			           } * window->GetDpiScaleFactor();

			AdjustWindowRectExForDpi(
				std::bit_cast<LPRECT>(&rc),
				window->GetStyle(),
				FALSE,
				window->GetExStyle(),
				static_cast<UINT>(window->GetDpi()));

			const PointF p{ static_cast<float>(createStruct->x), static_cast<float>(createStruct->y) };
			const RectF rect{ p * window->GetDpiScaleFactor(), rc.Size() };

			window->logicalRect.SetPhysicalValue(rect);

			window->MoveAndResize(*window->logicalRect);
		}

		if (msg == WM_NCDESTROY) [[unlikely]]
		{
			SetWindowPtrToHWND(hWnd, nullptr);
			window->hWnd = nullptr;
			window->parentHwnd = nullptr;
		}

		return result;
	}
}

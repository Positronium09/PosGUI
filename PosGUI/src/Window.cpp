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

	auto MessageHandlerHost::Attach(MessageInterceptor& interceptor, HandlerOrder order) noexcept -> Result<void>
	{
		auto& interceptorList = std::invoke([this, order] -> auto&
		{
			if (order == HandlerOrder::Before)
			{
				return beforeInterceptors;
			}
			return afterInterceptors;
		});

		if (const auto it = std::ranges::find_if(interceptorList, [&interceptor](const auto& ref)
			{
				return &ref.get() == &interceptor;
			});
			it != interceptorList.end())
		{
			return Unexpected{ Error{ ErrorCode::DuplicateEntry } };
		}

		interceptorList.emplace_back(interceptor);
		return EmptyResult;
	}

	auto MessageHandlerHost::Detach(MessageInterceptor& interceptor) noexcept -> Result<void>
	{
		auto* addr = &interceptor;

		if (const auto it = std::ranges::find_if(beforeInterceptors, [addr](const auto& ref)
			{
				return &ref.get() == addr;
			});
			it != beforeInterceptors.end())
		{
			beforeInterceptors.erase(it);
			return EmptyResult;
		}

		if (const auto it = std::ranges::find_if(afterInterceptors, [addr](const auto& ref)
			{
				return &ref.get() == addr;
			});
			it != afterInterceptors.end())
		{
			afterInterceptors.erase(it);
			return EmptyResult;
		}

		return Unexpected{ Error{ ErrorCode::NotFound } };
	}

	auto MessageHandlerHost::HandleMessage(
		const HWND hWnd, const MessageID msg,
		const Argument1 arg1, const Argument2 arg2) const noexcept -> std::optional<MessageHandlerResult>
	{
		auto handled = false;
		auto forced = false;
		auto defProcHandled = false;
		MessageHandlerResult result{ 0 };

		for (const auto& interceptor : beforeInterceptors)
		{
			if (Run(interceptor.get(),
			        hWnd, msg, arg1, arg2,
			        handled, forced, defProcHandled, result))
			{
				return result;
			}
		}

		if (Run(*this,
		        hWnd, msg, arg1, arg2,
		        handled, forced, defProcHandled, result))
		{
			return result;
		}

		for (const auto& interceptor : afterInterceptors)
		{
			if (Run(interceptor.get(),
			        hWnd, msg, arg1, arg2,
			        handled, forced, defProcHandled, result))
			{
				return result;
			}
		}

		if (!handled)
		{
			return std::nullopt;
		}

		return result;
	}

	auto MessageHandlerHost::Run(
		const MessageInterceptor& interceptor,
		const HWND hWnd, const MessageID msg,
		const Argument1 arg1, const Argument2 arg2,
		bool& handled, bool& forced, bool& defProcHandled,
		MessageHandlerResult& result) noexcept -> bool
	{
		const auto& handlers = interceptor.HandlersFor(msg);
		if (!handlers.has_value())
		{
			return false;
		}

		handled = true;

		for (const auto& handler : handlers.value())
		{
			const auto handledResult = Match(
				handler,
				[hWnd, msg, arg1, arg2](const HandlerHWNDType& h)
				{
					return h(hWnd, msg, arg1, arg2);
				}, 
				[msg, arg1, arg2](const HandlerType& h)
				{
					return h(msg, arg1, arg2);
				});

			const auto flags = handledResult.flags;

			if (!forced)
			{
				forced = IsFlagSet(flags, MessageHandlerFlags::ForceThisResult);
				result.result = handledResult.result;
			}

			if (!forced && !defProcHandled && IsFlagSet(flags, MessageHandlerFlags::ForceDefProcResult))
			{
				const auto defProcResult = DefWindowProcW(hWnd, msg, arg1, arg2);
				forced = true;
				defProcHandled = true;
				result.result = defProcResult;
			}

			if (!defProcHandled && IsFlagSet(flags, MessageHandlerFlags::PassToDefProc))
			{
				const auto defProcResult = DefWindowProcW(hWnd, msg, arg1, arg2);
				if (!forced)
				{
					result.result = defProcResult;
				}
				defProcHandled = true;
			}

			if (IsFlagSet(flags, MessageHandlerFlags::NoFurtherHandling))
			{
				return true;
			}
		}

		return false;
	}

	Window::~Window() noexcept
	{
		for (const auto& timerId : timerMap | std::views::keys)
		{
			KillTimer(hWnd, timerId);
		}
		timerMap.clear();


		parentHwnd = nullptr;
		if (const auto copyHwnd = hWnd;
			copyHwnd != nullptr)
		{
			hWnd = nullptr;

			SetWindowPtrToHWND(copyHwnd, nullptr);
			DestroyWindow(copyHwnd);
		}
	}

	Window::Window(const WindowClassPtr& windowClass) noexcept :
		windowClass{ windowClass }
	{
		RegisterHandler(WM_DPICHANGED, &Window::_OnDpiChanged);
		RegisterHandler(WM_DPICHANGED, [](MessageID, Argument1, Argument2) noexcept -> MessageHandlerResult
		{
			return 0;
		});
		RegisterHandler(WM_DPICHANGED_AFTERPARENT, &Window::_OnDpiChanged);
		RegisterHandler(WM_DPICHANGED_BEFOREPARENT, &Window::_OnDpiChanged);
		RegisterHandler(WM_WINDOWPOSCHANGED, &Window::_OnWindowPosChanged);
		// RegisterHandler(WM_SIZE, &Window::_OnSize);
		// RegisterHandler(WM_MOVE, &Window::_OnMove);
	}

	// ReSharper disable CppInconsistentNaming

	auto Window::_OnDpiChanged(const MessageID msg, const Argument1,
	                           const Argument2 arg2) noexcept -> MessageHandlerResult
	{
		LRESULT messageResult = 0;
		if (const auto result = logicalRect.SetDpi(GetDpi());
			!result.has_value())
		{
			Logger::Error(result.error(), L"SetDpi failed in _OnDpiChanged");
		}

		if (msg == WM_DPICHANGED)
		{
			logicalRect.SetPhysicalValue(*std::bit_cast<LPRECT>(arg2));
			messageResult = OnDpiChanged(GetDpi());
		}
		else if (msg == WM_DPICHANGED_AFTERPARENT)
		{
			messageResult = OnDpiChangedAfterParent(GetDpi());
		}
		else if (msg == WM_DPICHANGED_BEFOREPARENT)
		{
			messageResult = OnDpiChangedBeforeParent(GetDpi());
		}

		Redraw();

		return messageResult;
	}

	auto Window::_OnWindowPosChanged(UINT, Argument1, const Argument2 arg2) noexcept -> MessageHandlerResult
	{
		const auto windowPos = *std::bit_cast<LPWINDOWPOS>(arg2);
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

	auto Window::_OnSize(UINT, Argument1, const Argument2) noexcept -> MessageHandlerResult
	{
		OnSizeChanged(logicalRect->Size());

		return 0;
	}

	auto Window::_OnMove(UINT, Argument1, const Argument2) noexcept -> MessageHandlerResult
	{
		OnMoved(logicalRect->TopLeft());

		return 0;
	}

	// ReSharper restore CppInconsistentNaming

	// ReSharper disable once CppInconsistentNaming
	auto _WindowProc(const HWND hWnd, const MessageID msg, const Argument1 arg1, const Argument2 arg2) -> LRESULT
	{
		DebugTimer timer{
			#ifdef _DEBUG
			std::format(L"MSG {}", WindowMsgToText(msg))
			#endif
		};

		if (msg == WM_NCCREATE) [[unlikely]]
		{
			const auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(arg2);
			auto* window = std::bit_cast<Window*>(createStruct->lpCreateParams);

			window->hWnd = hWnd;
			window->parentHwnd = createStruct->hwndParent;
			if (const auto result = window->logicalRect.SetDpi(window->GetDpi());
				!result.has_value())
			{
				Logger::Error(result.error(), L"SetDpi failed in WM_NCCREATE");
			}

			SetWindowPtrToHWND(hWnd, window);
		}

		if (LRESULT result = 0;
			DwmDefWindowProc(hWnd, msg, arg1, arg2, &result)) [[unlikely]]
		{
			return result;
		}

		const RawWindowPtr<Window> window = GetWindowPtrFromHWND(hWnd);
		if (window == nullptr) [[unlikely]]
		{
			return DefWindowProcW(hWnd, msg, arg1, arg2);
		}

		if (msg == WM_TIMER)
		{
			const auto timerId = arg1;
			if (const auto it = window->timerMap.find(timerId);
				it != window->timerMap.end())
			{
				const auto& callback = it->second;
				callback(timerId);
				return 0;
			}
		}

		MessageHandlerResult result{ 0 };
		if (msg == WM_NCCREATE) [[unlikely]]
		{
			result = 1;
		}

		if (auto handledResult = window->messageHandlerHost.HandleMessage(hWnd, msg, arg1, arg2);
			handledResult.has_value())
		{
			result = MoveChecked(handledResult.value());
		}
		else
		{
			result = DefWindowProcW(hWnd, msg, arg1, arg2);
		}

		if (msg == WM_NCCREATE) [[unlikely]]
		{
			const auto* createStruct = std::bit_cast<LPCREATESTRUCTW>(arg2);
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

	auto Window::RemoveChildWindow(const HWND childHwnd) -> WindowPtr<>
	{
		const auto found = std::ranges::find_if(
			childWindows,
			[childHwnd](const auto& wnd)
			{
				return wnd->hWnd == childHwnd;
			});

		if (found == childWindows.end())
		{
			return nullptr;
		}
		auto childWindow = MoveChecked(*found);

		SetParent(childHwnd, nullptr);
		childWindow->parentHwnd = nullptr;
		childWindow->ModifyStyle(WS_POPUP, WS_CHILD);

		childWindows.erase(found);
		OnChildRemoved(childHwnd);

		return childWindow;
	}

	auto Window::GetParentWindow() const noexcept -> Window*
	{
		if (parentHwnd == nullptr)
		{
			return nullptr;
		}
		return GetWindowPtrFromHWND(parentHwnd);
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
			try
			{
				timerMap[id] = *callback;
			}
			catch (const std::exception& e)
			{
				Logger::Error(
					Error{ SystemErrorCode::STLFailure }
					.AddDetail(L"Exception", StringToWString(e.what()))
				);

				KillTimer(hWnd, id);
				return 0;
			}
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

	auto Window::GetChildWindow(const HWND hwnd) const noexcept -> RawWindowPtr<Window>
	{
		const auto result = std::ranges::find_if(childWindows, [hwnd](const auto& wnd)
		{
			return wnd.get()->hWnd == hwnd;
		});

		if (result == childWindows.end())
		{
			return nullptr;
		}

		return result->get();
	}

	auto Window::ChildWindowFromPoint(const PointF point) const noexcept -> RawWindowPtr<Window>
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
			return result->get();
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

	auto Window::CenterAroundWindow(const RawWindowPtr<> wnd) noexcept -> void
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

		const RectF rect = PhysicalToLogical(RectF{ rc });
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
}

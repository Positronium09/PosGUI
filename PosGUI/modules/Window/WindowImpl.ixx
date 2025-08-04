module;
#include <Windows.h>

export module PGUI.Window:Impl;

import std;

import PGUI.Utils;
import PGUI.WindowClass;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
import :WindowInterface;

export namespace PGUI
{
	template <typename T>
	concept WindowType = std::derived_from<T, Window>;

	template <WindowType T = Window>
	using WindowPtr = std::shared_ptr<T>;

	template <WindowType T = Window>
	using RawWindowPtr = T*;

	constexpr auto DEFAULT_SCREEN_DPI = USER_DEFAULT_SCREEN_DPI;

	enum class HandlerReturnFlags
	{
		None,
		PassToDefProc = 1,
		ForceThisResult = 2,
		NoFurtherHandling = 4
	};
	DEFINE_ENUM_FLAG_OPERATORS(HandlerReturnFlags);

	struct MessageHandlerResult
	{
		LRESULT result;
		HandlerReturnFlags flags;

		explicit(false) MessageHandlerResult(const LRESULT result, const HandlerReturnFlags flags = HandlerReturnFlags::None) :
			result{ result }, flags{ flags }
		{ }

		explicit(false) operator LRESULT() const noexcept { return result; }
	};

	using HandlerHWND = std::function<MessageHandlerResult(HWND, UINT, WPARAM, LPARAM)>;
	using Handler = std::function<MessageHandlerResult(UINT, WPARAM, LPARAM)>;
	template <typename T>
	using HandlerHWNDMember = MessageHandlerResult (T::*)(HWND, UINT, WPARAM, LPARAM);
	template <typename T>
	using HandlerMember = MessageHandlerResult(T::*)(UINT, WPARAM, LPARAM);
	template <typename T>
	using HandlerHWNDCMember = MessageHandlerResult(T::*)(HWND, UINT, WPARAM, LPARAM) const;
	template <typename T>
	using HandlerCMember = MessageHandlerResult(T::*)(UINT, WPARAM, LPARAM) const;
	using MessageHandler = std::variant<HandlerHWND, Handler>;

	using MessageHandlerMap = std::unordered_map<UINT, std::vector<MessageHandler>>;

	using ChildWindowList = std::vector<WindowPtr<>>;

	using TimerId = UINT_PTR;
	using TimerCallback = std::function<void(TimerId)>;
	using TimerMap = std::unordered_map<TimerId, TimerCallback>;

	enum class WindowFlashFlags : DWORD
	{
		All = FLASHW_ALL,
		Caption = FLASHW_CAPTION,
		Stop = FLASHW_STOP,
		Timer = FLASHW_TIMER,
		TimerNoFG = FLASHW_TIMERNOFG,
		Tray = FLASHW_TRAY
	};
	DEFINE_ENUM_FLAG_OPERATORS(WindowFlashFlags);

	enum class WindowLongPtrIndex : int
	{
		Style = GWL_STYLE,
		ExStyle = GWL_EXSTYLE,
		UserData = GWLP_USERDATA,
		ID = GWLP_ID,
		HInstance = GWLP_HINSTANCE,
		WndProc = GWLP_WNDPROC
	};

	enum class GetWindowIndex : UINT
	{
		First = GW_HWNDFIRST,
		Last = GW_HWNDLAST,
		Next = GW_HWNDNEXT,
		Prev = GW_HWNDPREV,
		Owner = GW_OWNER,
		Child = GW_CHILD
	};

	enum class ShowWindowCommand : int
	{
		Hide = SW_HIDE,
		Normal = SW_SHOWNORMAL,
		Minimized = SW_SHOWMINIMIZED,
		NoActivate = SW_SHOWNOACTIVATE,
		Show = SW_SHOW,
		Maximize = SW_MAXIMIZE,
		Minimize = SW_MINIMIZE,
		MinNoActive = SW_SHOWMINNOACTIVE,
		ShowNA = SW_SHOWNA,
		Restore = SW_RESTORE,
		Default = SW_SHOWDEFAULT,
		ForceMinimize = SW_FORCEMINIMIZE
	};

	enum class PositionFlags
	{
		AsyncWindowPos = SWP_ASYNCWINDOWPOS,
		DeferErase = SWP_DEFERERASE,
		DrawFrame = SWP_DRAWFRAME,
		FrameChanged = SWP_FRAMECHANGED,
		HideWindow = SWP_HIDEWINDOW,
		NoActivate = SWP_NOACTIVATE,
		NoCopyBits = SWP_NOCOPYBITS,
		NoMove = SWP_NOMOVE,
		NoOwnerZOrder = SWP_NOOWNERZORDER,
		NoRedraw = SWP_NOREDRAW,
		NoReposition = SWP_NOZORDER,
		NoSendChanging = SWP_NOSENDCHANGING,
		NoSize = SWP_NOSIZE,
		NoZOrder = SWP_NOZORDER,
		ShowWindow = SWP_SHOWWINDOW
	};
	DEFINE_ENUM_FLAG_OPERATORS(PositionFlags);

	enum class WindowPlacementFlags
	{
		SetMinPosition = WPF_SETMINPOSITION,
		RestoreToMaximized = WPF_RESTORETOMAXIMIZED,
		AsyncWindowPlacement = WPF_ASYNCWINDOWPLACEMENT
	};
	DEFINE_ENUM_FLAG_OPERATORS(WindowPlacementFlags);

	namespace InsertAfter
	{
		inline const auto Top = HWND_TOP;
		inline const auto Bottom = HWND_BOTTOM;
		inline const auto TopMost = HWND_TOPMOST;
		inline const auto NoTopMost = HWND_NOTOPMOST;
	}

	struct WindowCreateInfo
	{
		std::wstring title;
		PointI position{ };
		SizeI size{ };
		DWORD style;
		DWORD exStyle;

		constexpr WindowCreateInfo(const std::wstring_view title,
		                           const PointI position, const SizeI size,
		                           const DWORD style, const DWORD exStyle) noexcept :
			title{ title },
			position{ position }, size{ size },
			style{ style }, exStyle{ exStyle }
		{ }
	};

	struct WindowPlacement
	{
		WindowPlacementFlags flags;
		ShowWindowCommand showCmd;
		PointL minPosition;
		PointL maxPosition;
		RectL normalPosition;

		constexpr WindowPlacement() noexcept :
			flags{ WindowPlacementFlags::SetMinPosition },
			showCmd{ ShowWindowCommand::Normal },
			minPosition{ 0, 0 },
			maxPosition{ 0, 0 },
			normalPosition{ 0, 0, 0, 0 }
		{ }

		explicit(false) constexpr WindowPlacement(const WINDOWPLACEMENT& placement) noexcept :
			flags{ static_cast<WindowPlacementFlags>(placement.flags) },
			showCmd{ static_cast<ShowWindowCommand>(placement.showCmd) },
			minPosition{ placement.ptMinPosition },
			maxPosition{ placement.ptMaxPosition },
			normalPosition{ placement.rcNormalPosition }
		{ }

		explicit(false) constexpr operator WINDOWPLACEMENT() const noexcept
		{
			WINDOWPLACEMENT placement;
			placement.length = sizeof(WINDOWPLACEMENT);
			placement.flags = static_cast<UINT>(flags);
			placement.showCmd = static_cast<UINT>(showCmd);
			placement.ptMinPosition = minPosition;
			placement.ptMaxPosition = maxPosition;
			placement.rcNormalPosition = normalPosition;
			return placement;
		}
	};

	[[nodiscard]] auto GetWindowPtrFromHWND(const HWND hWnd) noexcept
	{
		return std::bit_cast<RawWindowPtr<>>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	class MessageHooker
	{
		friend class Window;

		public:
		virtual ~MessageHooker() noexcept = default;

		[[nodiscard]] const auto& GetHandlers() const noexcept { return messageHandlerMap; }

		protected:
		auto RegisterHandler(const UINT msg, const HandlerHWND& handler) -> void { _RegisterHandler(msg, handler); }
		auto RegisterHandler(const UINT msg, const Handler& handler) -> void { _RegisterHandler(msg, handler); }

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerHWNDMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerHWNDCMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerCMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}

		const auto& GetHookedWindow() const noexcept { return hookedWindow; }

		private:
		RawWindowPtr<> hookedWindow = nullptr;
		MessageHandlerMap messageHandlerMap{ };

		// ReSharper disable CppInconsistentNaming
		auto _RegisterHandler(const UINT msg, const HandlerHWND& handler) -> void
		{
			messageHandlerMap[msg].push_back(handler);
		}

		auto _RegisterHandler(const UINT msg, const Handler& handler) -> void
		{
			messageHandlerMap[msg].push_back(handler);
		}

		// ReSharper restore CppInconsistentNaming
	};

	using MessageHookers = std::vector<std::reference_wrapper<MessageHooker>>;

	class Window
	{
		// ReSharper disable once CppInconsistentNaming
		friend auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

		public:
		virtual ~Window();

		Window(const Window&) = delete;

		auto operator=(const Window&) -> Window& = delete;

		Window(Window&&) noexcept = delete;

		auto operator=(Window&&) noexcept -> Window&& = delete;

		template <WindowType T, typename... Args>
		[[nodiscard]] static auto Create(const WindowCreateInfo& info, Args&&... args)
		{
			auto window = std::make_shared<T>(std::forward<Args>(args)...);
			auto wnd = window.get();

			CreateWindowExW(info.exStyle,
			                wnd->windowClass->ClassName().data(), info.title.data(),
			                info.style,
			                info.position.x, info.position.y,
			                info.size.cx, info.size.cy,
			                nullptr, nullptr, GetHInstance(),
			                static_cast<LPVOID>(wnd));

			if (window->hWnd == NULL)
			{
				const auto errCode = GetLastError();
				Error error{
					errCode
				};
				error
					.AddTag(ErrorTags::Window)
					.AddTag(ErrorTags::Creation);
				Logger::Error(
					error,
					L"Window creation failed"
				);
				throw Exception{ errCode };
			}

			return window;
		}

		template <WindowType T, typename... Args>
		auto CreateChildWindow(const WindowCreateInfo& info, Args&&... args)
		{
			auto window = std::make_shared<T>(std::forward<Args>(args)...);
			auto wnd = window.get();

			CreateWindowExW(info.exStyle,
			                wnd->windowClass->ClassName().data(), info.title.data(),
			                info.style | WS_CHILD,
			                info.position.x, info.position.y,
			                info.size.cx, info.size.cy,
			                Hwnd(), nullptr, GetHInstance(),
			                static_cast<LPVOID>(wnd));

			if (window->hWnd == NULL)
			{
				const auto errCode = GetLastError();
				Error error{
					errCode
				};
				error
					.AddTag(ErrorTags::Window)
					.AddTag(ErrorTags::Creation);
				Logger::Error(
					error,
					L"Window creation failed"
				);

				throw Exception{ errCode };
			}

			childWindows.push_back(window);

			OnChildAdded(window);

			return std::dynamic_pointer_cast<T>(childWindows.back());
		}

		template <WindowType T>
		auto AddChildWindow(const WindowPtr<T>& window)
		{
			auto wnd = window.get();

			auto style = GetWindowLongPtrW(wnd->Hwnd(), GWL_STYLE);
			SetWindowLongPtrW(wnd->Hwnd(), GWL_STYLE, static_cast<LONG_PTR>(style | WS_CHILD));

			SetParent(wnd->Hwnd(), Hwnd());
			wnd->parentHwnd = Hwnd();

			childWindows.push_back(window);

			OnChildAdded(window);

			return wnd;
		}

		auto RemoveChildWindow(HWND childHwnd) -> void;

		[[nodiscard]] auto GetParentWindow() const noexcept -> RawWindowPtr<>;

		template <WindowType T>
		[[nodiscard]] auto GetParentWindow() const noexcept -> RawWindowPtr<T>
		{
			if (parentHwnd == nullptr)
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<RawWindowPtr<T>>(GetWindowPtrFromHWND(parentHwnd));
		}

		auto RegisterHandler(const UINT msg, const HandlerHWND& handler) -> void { _RegisterHandler(msg, handler); }
		auto RegisterHandler(const UINT msg, const Handler& handler) -> void { _RegisterHandler(msg, handler); }

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerHWNDMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerHWNDCMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}

		template <typename T>
		auto RegisterHandler(UINT msg, HandlerCMember<T> func) -> void
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}

		auto Hook(MessageHooker& hooker) noexcept -> void;

		auto HookBefore(MessageHooker& hooker) noexcept -> void;

		auto HookAfter(MessageHooker& hooker) noexcept -> void;

		auto UnHook(MessageHooker& hooker) noexcept -> void;

		auto UnHookBefore(MessageHooker& hooker) noexcept -> void;

		auto UnHookAfter(MessageHooker& hooker) noexcept -> void;

		auto AddTimer(TimerId id, std::chrono::milliseconds delay,
		              const std::optional<TimerCallback>& callback = std::nullopt) noexcept -> TimerId;

		auto RemoveTimer(TimerId id) noexcept -> void;

		[[nodiscard]] auto HasTimer(const TimerId id) const noexcept { return timerMap.contains(id); }

		[[nodiscard]] auto WindowClass() const noexcept { return windowClass; }
		[[nodiscard]] auto Hwnd() const noexcept { return hWnd; }
		[[nodiscard]] auto ParentHwnd() const noexcept { return parentHwnd; }

		[[nodiscard]] const auto& GetChildWindows() const noexcept { return childWindows; }

		[[nodiscard]] auto GetChildWindow(HWND hWnd) const noexcept -> WindowPtr<Window>;

		[[nodiscard]] auto ChildWindowFromPoint(PointL point, UINT flags) const noexcept -> WindowPtr<Window>;

		auto AdjustForClientSize(SizeI size) const noexcept -> void;

		auto AdjustForRect(RectI rect) const noexcept -> void;

		[[nodiscard]] auto GetWindowRect() const noexcept -> RectL;

		[[nodiscard]] auto GetClientRect() const noexcept -> RectL;

		[[nodiscard]] auto GetWindowSize() const noexcept -> SizeL;

		[[nodiscard]] auto GetClientSize() const noexcept -> SizeL;

		[[nodiscard]] auto GetPlacement() const noexcept -> WindowPlacement;

		[[nodiscard]] auto ScreenToClient(PointL point) const noexcept -> PointL;

		[[nodiscard]] auto ScreenToClient(RectL rect) const noexcept -> RectL;

		[[nodiscard]] auto ClientToScreen(PointL point) const noexcept -> PointL;

		[[nodiscard]] auto ClientToScreen(RectL rect) const noexcept -> RectL;

		[[nodiscard]] auto GetDPI() const noexcept { return GetDpiForWindow(Hwnd()); }

		[[nodiscard]] auto IsInputEnabled() const noexcept -> bool { return IsWindowEnabled(Hwnd()); }

		[[nodiscard]] auto IsChild() const noexcept
		{
			return static_cast<bool>(GetWindowLongPtrW(Hwnd(), GWL_STYLE) & WS_CHILD);
		}

		[[nodiscard]] auto IsChildOf(const WindowPtr<Window>& parent) const noexcept
		{
			return parentHwnd == parent->Hwnd();
		}

		[[nodiscard]] auto IsChildOf(const HWND hwnd) const noexcept { return parentHwnd == hwnd; }
		[[nodiscard]] auto IsTopLevel() const noexcept { return !IsChild(); }

		[[nodiscard]] auto IsVisible() const noexcept -> bool { return IsWindowVisible(Hwnd()); }
		[[nodiscard]] auto IsMinimized() const noexcept -> bool { return IsIconic(Hwnd()); }
		[[nodiscard]] auto IsMaximized() const noexcept -> bool { return IsZoomed(Hwnd()); }

		auto CenterAroundWindow(WindowPtr<> wnd) const noexcept -> void;

		auto CenterAroundWindow(HWND hwnd) const noexcept -> void;

		auto CenterAroundPoint(PointL point) const noexcept -> void;

		auto CenterAroundRect(RectL rect) const noexcept -> void;

		auto CenterAroundParent() const noexcept -> void;

		auto VerticallyCenterAroundParent() const noexcept -> void;

		auto HorizontallyCenterAroundParent() const noexcept -> void;

		[[nodiscard]] auto GetStyle() const noexcept -> DWORD
		{
			return static_cast<DWORD>(GetWindowLongPtrW(Hwnd(), GWL_STYLE));
		}

		[[nodiscard]] auto GetExStyle() const noexcept -> DWORD
		{
			return static_cast<DWORD>(GetWindowLongPtrW(Hwnd(), GWL_EXSTYLE));
		}

		[[nodiscard]] auto GetClassStyle() const noexcept { return GetClassLongPtrW(Hwnd(), GCL_STYLE); }

		[[nodiscard]] auto GetTopWindow() const noexcept { return ::GetTopWindow(hWnd); }

		[[nodiscard]] auto GetWindow(GetWindowIndex index) const noexcept
		{
			return ::GetWindow(hWnd, static_cast<UINT>(index));
		}

		[[nodiscard]] auto GetLastActivePopup() const noexcept { return ::GetLastActivePopup(hWnd); }

		[[nodiscard]] auto GetLongPtr(WindowLongPtrIndex index) const noexcept
		{
			return GetWindowLongPtrW(hWnd, static_cast<UINT>(index));
		}

		auto ModifyStyle(DWORD add, DWORD remove) const noexcept -> void;

		auto ModifyExStyle(DWORD add, DWORD remove) const noexcept -> void;

		auto AddStyle(const DWORD style) const noexcept -> void { ModifyStyle(style, NULL); }
		auto AddExStyle(const DWORD style) const noexcept -> void { ModifyExStyle(style, NULL); }
		auto RemoveStyle(const DWORD style) const noexcept -> void { ModifyStyle(NULL, style); }
		auto RemoveExStyle(const DWORD style) const noexcept -> void { ModifyExStyle(NULL, style); }

		auto EnableInput() const noexcept -> void { EnableWindow(Hwnd(), true); }
		auto DisableInput() const noexcept -> void { EnableWindow(Hwnd(), false); }

		auto Show(const ShowWindowCommand show = ShowWindowCommand::Show) const noexcept -> void
		{
			ShowWindow(hWnd, ToUnderlying(show));
		}

		auto Hide() const noexcept -> void { Show(ShowWindowCommand::Hide); }
		auto Minimize() const noexcept -> void { Show(ShowWindowCommand::Minimize); }
		auto Maximize() const noexcept -> void { Show(ShowWindowCommand::Maximize); }
		auto BringToTop() const noexcept -> void { BringWindowToTop(Hwnd()); }
		auto Invalidate(const bool erase = true) const noexcept -> void { InvalidateRect(Hwnd(), nullptr, erase); }
		auto Update() const noexcept -> void { UpdateWindow(Hwnd()); }
		auto SetFocus() const noexcept -> void { ::SetFocus(Hwnd()); }
		auto SetForeground() const noexcept -> void { SetForegroundWindow(hWnd); }
		auto SetActive() const noexcept -> void { SetActiveWindow(hWnd); }

		auto Flash(WindowFlashFlags flags, UINT count, std::chrono::milliseconds timeout) const noexcept -> void;

		auto StopFlash() const noexcept -> void;

		auto SetCapture() const noexcept -> void { ::SetCapture(hWnd); }
		// ReSharper disable once CppMemberFunctionMayBeStatic
		auto ReleaseCapture() const noexcept -> void { ::ReleaseCapture(); }

		auto SetPosition(PointL position, SizeL size, PositionFlags flags,
		                 HWND insertAfter = nullptr) const noexcept -> void;

		auto SetPosition(RectL rect, PositionFlags flags, HWND insertAfter = nullptr) const noexcept -> void;

		auto Move(PointL newPos) const noexcept -> void;

		auto Resize(SizeL newSize) const noexcept -> void;

		auto MoveAndResize(RectL newRect) const noexcept -> void;

		auto MoveAndResize(PointL newPos, SizeL newSize) const noexcept -> void;

		[[nodiscard]] auto MapPoints(HWND hWndTo, std::span<PointL> points) const noexcept -> std::span<PointL>;

		[[nodiscard]] auto MapPoint(HWND hWndTo, PointL point) const noexcept -> PointL;

		[[nodiscard]] auto MapRects(HWND hWndTo, std::span<RectL> rects) const noexcept -> std::span<RectL>;

		[[nodiscard]] auto MapRect(HWND hWndTo, RectL rect) const noexcept -> RectL;

		auto SendMsg(const UINT msg, const WPARAM wParam, const LPARAM lParam) const noexcept -> void
		{
			SendMessageW(Hwnd(), msg, wParam, lParam);
		}

		auto PostMsg(const UINT msg, const WPARAM wParam, const LPARAM lParam) const noexcept -> void
		{
			PostMessageW(Hwnd(), msg, wParam, lParam);
		}

		protected:
		explicit Window(const WindowClassPtr& windowClass) noexcept;

		virtual auto OnDpiChanged(UINT dpi, RectL suggestedRect) -> MessageHandlerResult;

		virtual auto OnChildAdded(const WindowPtr<Window>&) -> void
		{
			/* E_NOTIMPL */
		}

		virtual auto OnChildRemoved(HWND) -> void
		{
			/* E_NOTIMPL */
		}

		virtual auto OnSizeChanged(SizeL) -> void
		{
			/* E_NOTIMPL */
		}

		private:
		// ReSharper disable CppInconsistentNaming
		auto _RegisterHandler(UINT msg, const HandlerHWND& handler) -> void;

		auto _RegisterHandler(UINT msg, const Handler& handler) -> void;

		auto _OnDpiChanged(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;
		auto _OnSize(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;

		// ReSharper restore CppInconsistentNaming

		MessageHandlerMap messageHandlerMap;
		ChildWindowList childWindows;
		TimerMap timerMap;
		MessageHookers beforeHookers;
		MessageHookers afterHookers;
		WindowClassPtr windowClass;
		HWND hWnd = nullptr;
		HWND parentHwnd = nullptr;
	};

	// ReSharper disable once CppInconsistentNaming
	auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
}

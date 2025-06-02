module;
#include <Windows.h>
#include <concepts>
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <bit>
#include <span>
#include <vector>
#include <chrono>
#include <optional>
#include <unordered_map>

export module PGUI.Window;

import PGUI.Utils;
import PGUI.WindowClass;
import PGUI.Shape2D;
import PGUI.Exceptions;
import PGUI.Logging;
import PGUI.EnumFlag;

export namespace PGUI
{
	constexpr auto DEFAULT_SCREEN_DPI = USER_DEFAULT_SCREEN_DPI;
	
	enum class ReturnFlags
	{
		None,
		PassToDefProc = 1,
		ForceThisResult = 2,
		NoFurtherHandling = 4
	};
	template <>
	struct IsEnumFlagEnabled<ReturnFlags> : Enabled { };

	struct MessageHandlerResult
	{
		LRESULT result;
		ReturnFlags flags;

		explicit(false) MessageHandlerResult(LRESULT result, ReturnFlags flags = ReturnFlags::None) : 
			result{ result }, flags{ flags }
		{
		}

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

	class Window;

	template <typename T>
	concept WindowType = std::derived_from<T, Window>;

	template <WindowType T = Window>
	using WindowPtr = std::shared_ptr<T>;

	template <WindowType T = Window>
	using RawWindowPtr = T*;

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
	template <>
	struct IsEnumFlagEnabled<WindowFlashFlags> : Enabled { };

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
	template <>
	struct IsEnumFlagEnabled<PositionFlags> : Enabled { };

	enum class WindowPlacementFlags
	{
		SetMinPosition =  WPF_SETMINPOSITION,
		RestoreToMaximized = WPF_RESTORETOMAXIMIZED,
		AsyncWindowPlacement = WPF_ASYNCWINDOWPLACEMENT
	};
	template <>
	struct IsEnumFlagEnabled<WindowPlacementFlags> : Enabled { };

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
		PointI position;
		SizeI size;
		DWORD style;
		DWORD exStyle;

		constexpr WindowCreateInfo(std::wstring_view title, 
			PointI position, SizeI size, 
			DWORD style, DWORD exStyle) noexcept :
			title{ title }, 
			position{ position }, size{ size },
			style{ style }, exStyle{ exStyle }
		{
		}
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
		{
		}

		explicit(false) constexpr WindowPlacement(const WINDOWPLACEMENT& placement) noexcept :
			flags{ static_cast<WindowPlacementFlags>(placement.flags) },
			showCmd{ static_cast<ShowWindowCommand>(placement.showCmd) },
			minPosition{ placement.ptMinPosition },
			maxPosition{ placement.ptMaxPosition },
			normalPosition{ placement.rcNormalPosition }
		{
		}
		explicit(false) constexpr operator WINDOWPLACEMENT() const noexcept
		{
			WINDOWPLACEMENT placement{ };
			placement.length = sizeof(WINDOWPLACEMENT);
			placement.flags = static_cast<UINT>(flags);
			placement.showCmd = static_cast<UINT>(showCmd);
			placement.ptMinPosition = minPosition;
			placement.ptMaxPosition = maxPosition;
			placement.rcNormalPosition = normalPosition;
			return placement;
		}
	};

	[[nodiscard]] auto GetWindowPtrFromHWND(HWND hWnd) noexcept
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
		void RegisterHandler(UINT msg, const HandlerHWND& handler) { _RegisterHandler(msg, handler); }
		void RegisterHandler(UINT msg, const Handler& handler) { _RegisterHandler(msg, handler); }
		template <typename T>
		void RegisterHandler(UINT msg, HandlerHWNDMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}
		template <typename T>
		void RegisterHandler(UINT msg, HandlerMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}
		template <typename T>
		void RegisterHandler(UINT msg, HandlerHWNDCMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}
		template <typename T>
		void RegisterHandler(UINT msg, HandlerCMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}

		const auto& GetHookedWindow() const noexcept { return hookedWindow; }

		private:
		RawWindowPtr<> hookedWindow = nullptr;
		MessageHandlerMap messageHandlerMap{ };

		void _RegisterHandler(UINT msg, const HandlerHWND& handler)
		{
			messageHandlerMap[msg].push_back(handler);
		}
		void _RegisterHandler(UINT msg, const Handler& handler)
		{
			messageHandlerMap[msg].push_back(handler);
		}
	};

	using MessageHookers = std::vector<std::reference_wrapper<const MessageHooker>>;

	class Window
	{
		friend auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
		
		public:
		virtual ~Window();

		Window(const Window&) = delete;
		auto operator=(const Window&) -> Window& = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(Window&&) noexcept -> Window&& = delete;

		template <WindowType T, typename ...Args>
		[[nodiscard]] static auto Create(const WindowCreateInfo& info, Args&&... args)
		{
			auto window = std::make_shared<T>(std::forward<Args>(args)...);
			auto wnd = window.get();

			CreateWindowExW(info.exStyle,
				wnd->windowClass->ClassName().data(), info.title.data(),
				info.style,
				info.position.x, info.position.y,
				info.size.cx, info.size.cy,
				nullptr, NULL, GetHInstance(),
				static_cast<LPVOID>(wnd));

			if (window->hWnd == NULL)
			{
				auto errCode = GetLastError();
				LogFailed(LogLevel::Error, errCode);
				throw Win32Exception{ errCode };
			}

			return window;
		}
		template <WindowType T, typename ...Args>
		auto CreateChildWindow(const WindowCreateInfo& info, Args&&... args)
		{
			auto window = std::make_shared<T>(std::forward<Args>(args)...);
			auto wnd = window.get();

			CreateWindowExW(info.exStyle,
				wnd->windowClass->ClassName().data(), info.title.data(),
				info.style | WS_CHILD,
				info.position.x, info.position.y,
				info.size.cx, info.size.cy,
				Hwnd(), NULL, GetHInstance(),
				static_cast<LPVOID>(wnd));

			if (window->hWnd == NULL)
			{
				auto errCode = GetLastError();
				LogFailed(LogLevel::Error, errCode);
				throw Win32Exception{ errCode };
			}

			childWindows.push_back(window);

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

			return wnd;
		}
		void RemoveChildWindow(HWND childHwnd);

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

		void RegisterHandler(UINT msg, const HandlerHWND& handler) { _RegisterHandler(msg, handler); }
		void RegisterHandler(UINT msg, const Handler& handler) { _RegisterHandler(msg, handler); }
		template <typename T>
		void RegisterHandler(UINT msg, HandlerHWNDMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}
		template <typename T>
		void RegisterHandler(UINT msg, HandlerMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<T*>(this)));
		}
		template <typename T>
		void RegisterHandler(UINT msg, HandlerHWNDCMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}
		template <typename T>
		void RegisterHandler(UINT msg, HandlerCMember<T> func)
		{
			_RegisterHandler(msg, std::bind_front(func, std::bit_cast<const T*>(this)));
		}

		void Hook(MessageHooker& hooker) noexcept;
		void UnHook(MessageHooker& hooker) noexcept;

		auto AddTimer(TimerId id, std::chrono::milliseconds delay,
			std::optional<TimerCallback> callback = std::nullopt) noexcept -> TimerId;
		void RemoveTimer(TimerId id) noexcept;
		[[nodiscard]] auto HasTimer(TimerId id) const noexcept { return timerMap.contains(id); }

		[[nodiscard]] auto WindowClass() const noexcept { return windowClass; }
		[[nodiscard]] auto Hwnd() const noexcept { return hWnd; }
		[[nodiscard]] auto ParentHwnd() const noexcept { return parentHwnd; }

		[[nodiscard]] const auto& GetChildWindows() const noexcept { return childWindows; }
		[[nodiscard]] auto GetChildWindow(HWND hWnd) const noexcept -> WindowPtr<Window>;
		[[nodiscard]] auto ChildWindowFromPoint(PointL point, UINT flags) const noexcept -> WindowPtr<Window>;

		void AdjustForClientSize(SizeI size) const noexcept;
		void AdjustForRect(RectI rect) const noexcept;
		
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
		[[nodiscard]] auto IsChild() const noexcept { return static_cast<bool>(GetWindowLongPtrW(Hwnd(), GWL_STYLE) & WS_CHILD); }
		[[nodiscard]] auto IsChildOf(const WindowPtr<Window>& parent) const noexcept { return parentHwnd == parent->Hwnd(); }
		[[nodiscard]] auto IsChildOf(HWND hwnd) const noexcept { return parentHwnd == hwnd; }
		[[nodiscard]] auto IsTopLevel() const noexcept { return !IsChild(); }

		[[nodiscard]] auto IsVisible() const noexcept { return IsWindowVisible(Hwnd()); }
		[[nodiscard]] auto IsMinimized() const noexcept { return IsIconic(Hwnd()); }
		[[nodiscard]] auto IsMaximized() const noexcept { return IsZoomed(Hwnd()); }

		void CenterAroundWindow(WindowPtr<> wnd) const noexcept;
		void CenterAroundWindow(HWND hwnd) const noexcept;
		void CenterAroundPoint(PointL point) const noexcept;
		void CenterAroundRect(RectL rect) const noexcept;
		void CenterAroundParent() const noexcept;
		void VerticallyCenterAroundParent() const noexcept;
		void HorizontallyCenterAroundParent() const noexcept;

		[[nodiscard]] auto GetStyle() const noexcept -> DWORD
		{ return static_cast<DWORD>(GetWindowLongPtrW(Hwnd(), GWL_STYLE)); }
		[[nodiscard]] auto GetExStyle() const noexcept-> DWORD
		{ return static_cast<DWORD>(GetWindowLongPtrW(Hwnd(), GWL_EXSTYLE)); }
		[[nodiscard]] auto GetClassStyle() const noexcept { return GetClassLongPtrW(Hwnd(), GCL_STYLE); }

		[[nodiscard]] auto GetTopWindow() const noexcept { return ::GetTopWindow(hWnd); }
		[[nodiscard]] auto GetWindow(GetWindowIndex index) const noexcept { return ::GetWindow(hWnd, static_cast<UINT>(index)); }
		[[nodiscard]] auto GetLastActivePopup() const noexcept { return ::GetLastActivePopup(hWnd); }
		[[nodiscard]] auto GetLongPtr(WindowLongPtrIndex index) const noexcept { return GetWindowLongPtrW(hWnd, static_cast<UINT>(index)); }

		void ModifyStyle(DWORD add, DWORD remove) const noexcept;
		void ModifyExStyle(DWORD add, DWORD remove) const noexcept;
		void AddStyle(DWORD style) const noexcept { ModifyStyle(style, NULL); }
		void AddExStyle(DWORD style) const noexcept { ModifyExStyle(style, NULL); }
		void RemoveStyle(DWORD style) const noexcept { ModifyStyle(NULL, style); }
		void RemoveExStyle(DWORD style) const noexcept { ModifyExStyle(NULL, style); }

		void EnableInput() const noexcept { EnableWindow(Hwnd(), true); }
		void DisableInput() const noexcept { EnableWindow(Hwnd(), false); }
		void Show(ShowWindowCommand show = ShowWindowCommand::Show) const noexcept { ShowWindow(hWnd, ToUnderlying(show)); }
		void Hide() const noexcept { Show(ShowWindowCommand::Hide); }
		void Minimize() const noexcept { Show(ShowWindowCommand::Minimize); }
		void Maximize() const noexcept { Show(ShowWindowCommand::Maximize); }
		void BringToTop() const noexcept { BringWindowToTop(Hwnd()); }
		void Invalidate(bool erase = true) const noexcept { InvalidateRect(Hwnd(), nullptr, erase); }
		void Update() const noexcept { UpdateWindow(Hwnd()); }
		void SetFocus() const noexcept { ::SetFocus(Hwnd()); }
		void SetForeground() const noexcept { SetForegroundWindow(hWnd); }
		void SetActive() const noexcept { SetActiveWindow(hWnd); }
		void Flash(WindowFlashFlags flags, UINT count, std::chrono::milliseconds timeout) const noexcept;
		void StopFlash() const noexcept;
		
		void SetCapture() const noexcept { ::SetCapture(hWnd); }
		void ReleaseCapture() const noexcept { ::ReleaseCapture(); }

		void SetPosition(PointL position, SizeL size, PositionFlags flags, HWND insertAfter = nullptr) const noexcept;
		void SetPosition(RectL rect, PositionFlags flags, HWND insertAfter = nullptr) const noexcept;
		void Move(PointL newPos) const noexcept;
		void Resize(SizeL newSize) const noexcept;
		void MoveAndResize(RectL newRect) const noexcept;
		void MoveAndResize(PointL newPos, SizeL newSize) const noexcept;

		[[nodiscard]] auto MapPoints(HWND hWndTo, std::span<PointL> points) const noexcept -> std::span<PointL>;
		[[nodiscard]] auto MapPoint(HWND hWndTo, PointL point) const noexcept -> PointL;
		[[nodiscard]] auto MapRects(HWND hWndTo, std::span<RectL> rects) const noexcept -> std::span<RectL>;
		[[nodiscard]] auto MapRect(HWND hWndTo, RectL rect) const noexcept -> RectL;

		void SendMsg(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept { SendMessageW(Hwnd(), msg, wParam, lParam); }
		void PostMsg(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept { PostMessageW(Hwnd(), msg, wParam, lParam); }

		protected:
		explicit Window(const WindowClassPtr& windowClass) noexcept;
		virtual auto OnDpiChanged(UINT dpi, RectL suggestedRect) -> MessageHandlerResult;

		private:
		void _RegisterHandler(UINT msg, const HandlerHWND& handler);
		void _RegisterHandler(UINT msg, const Handler& handler);

		auto _OnDpiChanged(UINT msg, WPARAM wParam, LPARAM lParam) -> MessageHandlerResult;

		MessageHandlerMap messageHandlerMap;
		ChildWindowList childWindows;
		TimerMap timerMap;
		MessageHookers hookers;
		WindowClassPtr windowClass;
		HWND hWnd = nullptr;
		HWND parentHwnd = nullptr;
	};

	auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
}

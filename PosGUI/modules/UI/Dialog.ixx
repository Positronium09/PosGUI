module;
#include <Windows.h>

export module PGUI.UI.Dialog;

import std;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.ErrorHandling;
import PGUI.ErrorHandling;
import PGUI.Shape2D;
import PGUI.UI.DirectXCompositionWindow;

export namespace PGUI::UI
{
	struct DialogCreateInfo
	{
		HWND parentHwnd = nullptr;
		DWORD style{ };

		constexpr DialogCreateInfo(const HWND parentHwnd, const DWORD style) noexcept :
			parentHwnd{ parentHwnd }, style{ style }
		{ }
	};

	class Dialog;

	template <typename T>
	concept DialogType = std::derived_from<T, Dialog>;

	class Dialog : public DirectXCompositionWindow
	{
		public:
		template <DialogType T, typename... Args>
		static auto Create(
			const DialogCreateInfo& dialogInfo,
			const WindowCreateInfo& wndInfo, Args&&... args)
		{
			auto style = dialogInfo.style | WS_CAPTION | WS_SYSMENU;
			auto exStyle = wndInfo.exStyle;

			if (dialogInfo.style & DS_MODALFRAME)
			{
				exStyle |= WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE;
			}
			if (dialogInfo.style & DS_CONTEXTHELP)
			{
				exStyle |= WS_EX_CONTEXTHELP;
			}
			if (dialogInfo.style & DS_CONTROL)
			{
				style &= ~(WS_CAPTION | WS_SYSMENU);
				exStyle |= WS_EX_CONTROLPARENT;
			}

			RectI dialogRect{
				wndInfo.position.x,
				wndInfo.position.y,
				wndInfo.position.x + wndInfo.size.cx,
				wndInfo.position.y + wndInfo.size.cy
			};

			const auto parent = GetWindowPtrFromHWND(dialogInfo.parentHwnd);

			dialogRect.CenterAround(parent->GetWindowRect().Center());

			const auto position = dialogRect.TopLeft();
			const auto size = dialogRect.Size();

			WindowCreateInfo inf{
				wndInfo.title,
				position,
				size,
				style,
				exStyle
			};

			auto window = std::make_shared<T>(std::forward<Args>(args)...);

			CreateWindowExW(
				exStyle,
				window->WindowClass()->ClassName().data(), wndInfo.title.data(),
				style,
				position.x, position.y,
				size.cx, size.cy,
				dialogInfo.parentHwnd, nullptr, GetHInstance(),
				static_cast<LPVOID>(window.get()));

			if (window->Hwnd() == NULL)
			{
				Error error{ GetLastError() };
				error
					.AddTag(ErrorTags::Window)
					.AddTag(ErrorTags::Creation);
				Logger::Critical(error, L"Dialog creation failed");
				throw Exception{ error };
			}

			SendMessageW(window->Hwnd(), WM_INITDIALOG,
			             std::bit_cast<WPARAM>(window->Hwnd()),
			             std::bit_cast<LPARAM>(window.get()));

			//window->CenterAroundParent();

			return window;
		}

		~Dialog() override = default;

		protected:
		Dialog() noexcept;

		explicit Dialog(const WindowClassPtr& wndClass) noexcept;

		private:
		[[nodiscard]] static auto OnInitDialog(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;
	};

	using ModelessDialog = Dialog;

	class ModalDialog : public Dialog
	{
		public:
		ModalDialog() noexcept;

		explicit ModalDialog(const WindowClassPtr& wndClass) noexcept;

		virtual auto RunModal() noexcept -> int;

		private:
		bool shouldClose = false;
		std::atomic_ref<bool> shouldCloseAtomic = std::atomic_ref(shouldClose);

		auto OnClose(UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> MessageHandlerResult;
	};
}

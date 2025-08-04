module;
#include <Windows.h>

module PGUI.UI.Dialog;

import PGUI.Window;
import PGUI.WindowClass;
import PGUI.MessageLoop;

namespace PGUI::UI
{
	Dialog::Dialog() noexcept :
		Dialog{ WindowClass::Create(L"PGUI_ModelessDialog") }
	{ }

	Dialog::Dialog(const WindowClassPtr& wndClass) noexcept :
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_INITDIALOG, &Dialog::OnInitDialog);
	}

	auto Dialog::OnInitDialog(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return { NULL, HandlerReturnFlags::ForceThisResult };
	}

	ModalDialog::ModalDialog() noexcept :
		ModalDialog{ WindowClass::Create(L"PGUI_ModalDialog") }
	{ }

	ModalDialog::ModalDialog(const WindowClassPtr& wndClass) noexcept :
		Dialog{ wndClass }
	{
		RegisterHandler(WM_CLOSE, &ModalDialog::OnClose);
	}

	auto ModalDialog::RunModal() noexcept -> int
	{
		return RunModalMessageLoop(Hwnd(), ParentHwnd(), shouldCloseAtomic);
	}

	auto ModalDialog::OnClose(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		shouldCloseAtomic = true;
		return 0;
	}
}

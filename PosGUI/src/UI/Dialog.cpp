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

	auto Dialog::OnInitDialog(UINT, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		return { NULL, MessageHandlerReturnFlags::ForceThisResult };
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

	auto ModalDialog::OnClose(UINT, Argument1, Argument2) noexcept -> MessageHandlerResult
	{
		shouldCloseAtomic = true;
		return 0;
	}
}

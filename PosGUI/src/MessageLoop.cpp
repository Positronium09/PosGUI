module;
#include <Windows.h>

module PGUI.MessageLoop;

import std;

import PGUI.Exceptions;
import PGUI.Logging;

namespace PGUI
{
	auto RunGetMessageLoop() -> int
	{
		MSG msg{ };

		int ret;
		while ((ret = GetMessageW(&msg, nullptr, 0, 0)) != 0)
		{
			if (ret == -1)
			{
				const auto errCode = GetLastError();
				LogFailed(LogLevel::Fatal, errCode);
				return static_cast<int>(errCode);
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return static_cast<int>(msg.wParam);
	}

	//TODO Implement
	auto RunPeekMessageLoop() -> int
	{
		throw HResultException{ E_NOTIMPL };
	}

	auto RunModalMessageLoop(const HWND modalDialog, const HWND parent, const std::atomic_ref<bool> shouldClose) -> int
	{
		ShowWindow(modalDialog, SW_SHOW);
		if (parent != nullptr)
		{
			EnableWindow(parent, FALSE);
		}

		MSG msg{ };
		auto ret = 1;
		while (ret != 0 && !shouldClose.load())
		{
			ret = GetMessageW(&msg, nullptr, 0, 0);
			if (ret == -1)
			{
				const auto errCode = GetLastError();
				LogFailed(LogLevel::Error, errCode);
				return errCode;
			}

			if (!IsDialogMessageW(modalDialog, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}

		if (parent != nullptr)
		{
			EnableWindow(parent, TRUE);
			SetForegroundWindow(parent);
		}

		return static_cast<int>(msg.wParam);
	}
}

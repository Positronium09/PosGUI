module;
#include <Windows.h>
#include <wil/resource.h>

module PGUI.MessageLoop;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

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

				Logger::Critical(
					Error{ errCode }
					.AddDetail(L"Message", WindowMsgToText(msg.message))
					.AddDetail(L"wParam", std::to_wstring(msg.wParam))
					.AddDetail(L"lParam", std::to_wstring(msg.lParam))
				);

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
		throw Exception{ E_NOTIMPL };
	}

	auto RunModalMessageLoop(const HWND modalDialog, const HWND parent, const std::atomic_ref<bool> shouldClose) -> int
	{
		ShowWindow(modalDialog, SW_SHOW);
		if (parent != nullptr)
		{
			EnableWindow(parent, FALSE);
		}
		auto enableParent = wil::scope_exit([parent]
		{
			if (parent != nullptr)
			{
				EnableWindow(parent, TRUE);
				SetForegroundWindow(parent);
			}
		});

		MSG msg{ };
		auto ret = 1;
		while (ret != 0 && !shouldClose.load())
		{
			ret = GetMessageW(&msg, nullptr, 0, 0);
			if (ret == -1)
			{
				const auto errCode = GetLastError();

				Logger::Critical(
					Error{ errCode }
					.AddDetail(L"Message", WindowMsgToText(msg.message))
					.AddDetail(L"wParam", std::to_wstring(msg.wParam))
					.AddDetail(L"lParam", std::to_wstring(msg.lParam))
				);

				return static_cast<int>(errCode);
			}

			if (!IsDialogMessageW(modalDialog, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}

		return static_cast<int>(msg.wParam);
	}
}

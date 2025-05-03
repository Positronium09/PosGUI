module;
#include <Windows.h>

module PGUI.MessageLoop;
import PGUI.Exceptions;

namespace PGUI
{
	auto RunGetMessageLoop() -> int
	{
		MSG msg{ };

		BOOL ret = 0;
		while ((ret = GetMessageW(&msg, nullptr, 0, 0)) != 0)
		{
			if (ret == -1)
			{
				auto errCode = GetLastError();
				// HR_L(HresultFromWin32(errCode));
				return static_cast<int>(errCode);
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return static_cast<int>(msg.wParam);
	}

	//TODO Implemet
	auto RunPeekMessageLoop() -> int
	{
		throw HResultException{ E_NOTIMPL };
	}
}

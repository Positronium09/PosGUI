module;
#include <atomic>
#include <Windows.h>

export module PGUI.MessageLoop;

export namespace PGUI
{
	[[nodiscard]] auto RunGetMessageLoop() -> int;
	[[nodiscard]] auto RunPeekMessageLoop() -> int;
	[[nodiscard]] auto RunModalMessageLoop(HWND modalDialog, HWND parent, std::atomic_ref<bool> shouldClose) -> int;
}

export module PGUI.MessageLoop;

export namespace PGUI
{
	[[nodiscard]] auto RunGetMessageLoop() -> int;
	[[nodiscard]] auto RunPeekMessageLoop() -> int;
}

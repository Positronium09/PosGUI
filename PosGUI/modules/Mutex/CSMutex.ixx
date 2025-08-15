module;
#include <Windows.h>

export module PGUI.Mutex:CSMutex;

export namespace PGUI::Mutex
{
	class CSMutex
	{
		public:
		CSMutex() noexcept
		{
			InitializeCriticalSection(&criticalSection);
		}
		~CSMutex() noexcept
		{
			DeleteCriticalSection(&criticalSection);
		}
		auto lock() noexcept -> void
		{
			EnterCriticalSection(&criticalSection);
		}
		auto unlock() noexcept -> void
		{
			LeaveCriticalSection(&criticalSection);
		}
		// ReSharper disable once CppInconsistentNaming
		auto try_lock() noexcept -> bool
		{
			return TryEnterCriticalSection(&criticalSection) != ERROR_SUCCESS;
		}
		private:
		CRITICAL_SECTION criticalSection;
	};
}

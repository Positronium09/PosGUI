module;
#include <Windows.h>

export module PGUI.Mutex:SRWMutex;

export namespace PGUI::Mutex
{
	class SRWMutex
	{
		public:
		SRWMutex() noexcept = default;

		auto lock() noexcept -> void
		{
			AcquireSRWLockExclusive(&srwLock);
		}
		auto unlock() noexcept -> void
		{
			ReleaseSRWLockExclusive(&srwLock);
		}
		// ReSharper disable once CppInconsistentNaming
		auto try_lock() noexcept -> bool
		{
			return TryAcquireSRWLockExclusive(&srwLock) != ERROR_SUCCESS;
		}

		private:
		SRWLOCK srwLock = SRWLOCK_INIT;
	};
}

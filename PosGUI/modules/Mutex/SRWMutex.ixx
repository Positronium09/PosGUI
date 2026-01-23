module;
#include <Windows.h>

export module PGUI.Mutex:SRWMutex;

export namespace PGUI::Mutex
{
	class SRWMutex
	{
		public:
		SRWMutex() noexcept
		{
			InitializeSRWLock(&srwLock);
		}
		SRWMutex(const SRWMutex&) = delete;
		auto operator=(const SRWMutex&) -> SRWMutex& = delete;
		SRWMutex(SRWMutex&& other) noexcept = delete;
		auto operator=(SRWMutex&& other) noexcept -> SRWMutex& = delete;

		auto lock() noexcept -> void
		{
			AcquireSRWLockExclusive(&srwLock);
		}
		auto unlock() noexcept -> void
		{
			ReleaseSRWLockExclusive(&srwLock);
		}
		// ReSharper disable CppInconsistentNaming
		
		auto try_lock() noexcept -> bool
		{
			return TryAcquireSRWLockExclusive(&srwLock) != ERROR_SUCCESS;
		}

		auto lock_shared() noexcept -> void
		{
			AcquireSRWLockShared(&srwLock);
		}
		auto unlock_shared() noexcept -> void
		{
			ReleaseSRWLockShared(&srwLock);
		}
		auto try_lock_shared() noexcept -> bool
		{
			return TryAcquireSRWLockShared(&srwLock) != ERROR_SUCCESS;
		}
		// ReSharper restore CppInconsistentNaming

		private:
		SRWLOCK srwLock = SRWLOCK_INIT;
	};
}

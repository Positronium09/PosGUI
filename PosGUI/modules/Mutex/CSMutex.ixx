module;
#include <Windows.h>

export module PGUI.Mutex:CSMutex;

export namespace PGUI::Mutex
{
	class CSMutex
	{
		public:
		explicit CSMutex(const int spinCount = 0) noexcept
		{
			InitializeCriticalSectionEx(&criticalSection, spinCount, 0);
		}
		CSMutex(const CSMutex&) = delete;
		auto operator=(CSMutex&) -> CSMutex& = delete;
		CSMutex(CSMutex&& other) noexcept = delete;
		auto operator=(CSMutex&& other) noexcept -> CSMutex& = delete;

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

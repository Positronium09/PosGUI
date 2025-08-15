module;
#include <Windows.h>

export module PGUI.Mutex:KMutex;

export namespace PGUI::Mutex
{
	class KMutex final
	{
		public:
		KMutex() noexcept
		{
			static_assert(true, "WIP");
			mutexHandle = CreateMutexW(nullptr, false, nullptr);
		}

		~KMutex() noexcept
		{
			if (mutexHandle)
			{
				CloseHandle(mutexHandle);
			}
		}

		auto lock() const noexcept -> void
		{
			WaitForSingleObject(mutexHandle, INFINITE);
		}

		auto unlock() const noexcept -> void
		{
			ReleaseMutex(mutexHandle);
		}

		// ReSharper disable once CppInconsistentNaming
		auto try_lock() const noexcept -> bool
		{
			const auto result = WaitForSingleObject(mutexHandle, 0);
			return result == WAIT_OBJECT_0;
		}

		private:
		HANDLE mutexHandle = nullptr;
	};
}

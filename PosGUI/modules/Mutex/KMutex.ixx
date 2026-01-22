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
		KMutex(const KMutex&) = delete;
		auto operator=(const KMutex&) -> KMutex & = delete;
		KMutex(KMutex&& other) noexcept :
			mutexHandle{ other.mutexHandle }
		{
			other.mutexHandle = nullptr;
		}
		auto operator=(KMutex&& other) noexcept -> KMutex &
		{
			if (this != &other)
			{
				if (mutexHandle)
				{
					CloseHandle(mutexHandle);
				}
				mutexHandle = other.mutexHandle;
				other.mutexHandle = nullptr;
			}

			return *this;
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

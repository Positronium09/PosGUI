export module PGUI.Mutex;

export import :NullMutex;
export import :CSMutex;
export import :SRWMutex;
export import :KMutex;

import std;

export namespace PGUI::Mutex
{
	template <typename T>
	concept MutexType = requires (T& mutex)
	{
		mutex.lock();
		mutex.unlock();
		{ mutex.try_lock() } -> std::convertible_to<bool>;
	};

	template <typename T>
	concept SharedMutexType = MutexType<T> && requires (T& mutex)
	{
		mutex.lock_shared();
		mutex.unlock_shared();
		{ mutex.try_lock_shared() } -> std::convertible_to<bool>;
	};
}

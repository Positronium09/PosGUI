module;
#include <functional>
#include <mutex>

export module PGUI.Property;

import PGUI.Event;

export namespace PGUI
{
	class NullMutex
	{
		public:
		void lock() noexcept
		{
		}
		void unlock() noexcept
		{
		}
	};;

	template <typename T, typename MutexType = std::mutex>
	class Property
	{
		using EventType = Event<T&>;

		public:
		Property() noexcept = default;
		explicit(false) Property(const T& value) : value{ value }
		{
		}

		Property(const Property& other) : value{ other.Get() }
		{
		}

		Property(Property&& other) noexcept : value{ std::move(other.Get()) }
		{
		}

		auto operator=(const Property& other) noexcept -> Property&
		{
			if (this != &other)
			{
				std::lock_guard lock{ mutex };
				value = other.Get();
				valueChangedEvent.Invoke(value);
			}
			return *this;
		}

		auto operator=(Property&& other) noexcept -> Property&
		{
			if (this != &other)
			{
				std::lock_guard lock{ mutex };
				value = std::move(other.Get());
				valueChangedEvent.Invoke(value);
			}
			return *this;
		}

		[[nodiscard]] const auto& Get() const noexcept
		{
			std::lock_guard lock{ mutex };
			return value;
		}
		[[nodiscard]] auto& Get() noexcept
		{
			std::lock_guard lock{ mutex };
			return value;
		}

		void Set(const T& newValue)
		{
			{
				std::lock_guard lock{ mutex };
				if constexpr (std::equality_comparable<T>)
				{
					if (value == newValue)
						return;
				}
				value = newValue;
			}
			valueChangedEvent.Invoke(Get());
		}
		void Set(T&& newValue)
		{
			{
				std::lock_guard lock{ mutex };
				if constexpr (std::equality_comparable<T>)
				{
					if (value == newValue)
						return;
				}
				value = std::move(newValue);
			}
			valueChangedEvent.Invoke(Get());
		}

		auto AddObserver(CallbackType<T&> auto callback) noexcept
		{
			return valueChangedEvent.AddCallback(callback);
		}

		void RemoveObserver(CallbackId id) noexcept
		{
			valueChangedEvent.RemoveCallback(id);
		}

		auto operator=(const T& val) noexcept -> Property&
		{
			Set(val);
			return *this;
		}
		auto operator=(T&& val) noexcept -> Property&
		{
			Set(val);
			return *this;
		}

		explicit(false) operator const T&() const { return Get(); }
		explicit(false) operator T&() { return Get(); }

		auto operator==(const Property& other) const noexcept -> bool
		{
			std::lock_guard lock{ mutex };
			return value == other.Get();
		}
		auto operator<=>(const Property& other) const noexcept
		{
			std::lock_guard lock{ mutex };
			return value <=> other.Get();
		}

		private:
		T value;
		MutexType mutex;
		EventType valueChangedEvent{ };
	};
}

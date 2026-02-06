export module PGUI.DataBinding.Property;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.Event;
import PGUI.Mutex;

export namespace PGUI::DataBinding
{
	template <typename T, Mutex::SharedMutexType Mutex = Mutex::SRWMutex>
	class Property
	{
		public:
		using ValueType = T;
		using MutexType = Mutex;
		
		struct AccessorProxy
		{
			AccessorProxy(const T& value, MutexType& mutex) : 
				value{ value }, lock{ mutex }
			{
			}

			AccessorProxy(const AccessorProxy&) = delete;
			auto operator=(const AccessorProxy&) -> AccessorProxy& = delete;
			AccessorProxy(AccessorProxy&&) = default;
			auto operator=(AccessorProxy&&) -> AccessorProxy& = default;

			[[nodiscard]] auto operator->() const noexcept -> const T*
			{
				return std::addressof(value);
			}
			[[nodiscard]] auto operator*() const noexcept -> const T&
			{
				return value;
			}
			
			[[nodiscard]] explicit(false) operator const T&() const noexcept
			{
				return value;
			}

			[[nodiscard]] auto Get() const noexcept -> const T&
			{
				return value;
			}

			private:
			const T& value;
			std::shared_lock<MutexType> lock;
		};

		using EventType = EventT<Mutex, const AccessorProxy&>;

		Property() noexcept(std::is_nothrow_default_constructible_v<T>) = default;

		explicit(false) Property(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) :
			value{ value }
		{ }

		Property(const Property& other) noexcept(std::is_nothrow_copy_constructible_v<T>) :
			value{ other.Get() }
		{ }

		Property(Property&& other) noexcept(std::is_nothrow_move_constructible_v<T>) :
			value{ std::move(other.Get()) }
		{ }

		virtual ~Property() = default;

		virtual auto operator=(const Property& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> Property&
		{
			if (this != &other)
			{
				Set(other.Get());
				valueChangedEvent.Invoke(Get());
			}
			return *this;
		}

		virtual auto operator=(Property&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> Property&
		{
			if (this != &other)
			{
				Set(std::move(other.Get()));
				valueChangedEvent.Invoke(Get());
			}
			return *this;
		}

		[[nodiscard]] auto Get() const noexcept
		{
			return AccessorProxy{ value, mutex };
		}

		virtual auto Set(const T& newValue) -> void
		{
			{
				std::scoped_lock lock{ mutex };
				if constexpr (std::equality_comparable<T>)
				{
					if (value == newValue)
					{
						return;
					}
				}
				value = newValue;
			}
			valueChangedEvent.Invoke(Get());
		}

		virtual auto Set(T&& newValue) -> void
		{
			{
				std::scoped_lock lock{ mutex };
				if constexpr (std::equality_comparable<T>)
				{
					if (value == newValue)
					{
						return;
					}
				}
				value = std::move(newValue);
			}
			valueChangedEvent.Invoke(Get());
		}

		auto AddObserver(CallbackType<const AccessorProxy&> auto callback) noexcept
		{
			return valueChangedEvent.AddCallback(callback);
		}

		auto RemoveObserver(CallbackId id) noexcept -> void
		{
			valueChangedEvent.RemoveCallback(id);
		}

		auto ClearObservers() noexcept -> void
		{
			valueChangedEvent.ClearCallbacks();
		}

		virtual auto operator=(const T& val) noexcept -> Property&
		{
			Set(val);
			return *this;
		}

		virtual auto operator=(T&& val) noexcept -> Property&
		{
			Set(val);
			return *this;
		}

		virtual auto operator==(const T& val) const noexcept -> bool
		{
			return *Get() == val;
		}

		virtual auto operator<=>(const T& val) const noexcept -> decltype(std::declval<const T&>() <=> std::declval<const T&>())
		{
			return *Get() <=> val;
		}

		virtual auto operator*() const noexcept -> AccessorProxy
		{
			return AccessorProxy{ value, mutex };
		}

		private:
		T value;
		mutable Mutex mutex;
		EventType valueChangedEvent{ };
	};

	template <typename T>
	using PropertyNM = Property<T, Mutex::NullMutex>;
}

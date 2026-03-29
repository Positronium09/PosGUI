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
		
		using AccessorProxyType = AccessorProxy<T, Mutex>;

		using EventType = EventT<Mutex, const AccessorProxyType&>;

		Property() noexcept(std::is_nothrow_default_constructible_v<T>) = default;

		explicit(false) Property(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) :
			value{ value }
		{ }

		Property(const Property& other) noexcept(std::is_nothrow_copy_constructible_v<T>) :
			value{ other.Get() }
		{ }

		Property(Property&& other) noexcept(std::is_nothrow_move_constructible_v<T>) : 
			value{ other.MoveValue() }
		{
		}

		virtual ~Property() = default;

		virtual auto operator=(const Property& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> Property&
		{
			if (this != &other)
			{
				Set(other.Get());
			}
			return *this;
		}

		virtual auto operator=(Property&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> Property&
		{
			if (this != &other)
			{
				Set(other.MoveValue());
			}
			return *this;
		}

		[[nodiscard]] auto Get() const noexcept
		{
			return AccessorProxyType{ value, mutex };
		}

		virtual auto Set(const T& newValue) noexcept(std::is_nothrow_copy_assignable_v<T>) -> void
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

		virtual auto Set(T&& newValue) noexcept(std::is_nothrow_move_assignable_v<T>) -> void
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

		auto MoveValue() noexcept(std::is_nothrow_move_constructible_v<T>) -> T
		{
			std::scoped_lock lock{ mutex };
			return std::move(value);
		}

		auto AddObserver(CallbackType<const AccessorProxyType&> auto callback) noexcept
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

		virtual auto operator=(const T& val) noexcept(std::is_nothrow_copy_assignable_v<T>) -> Property&
		{
			Set(val);
			return *this;
		}

		virtual auto operator=(T&& val) noexcept(std::is_nothrow_move_assignable_v<T>) -> Property&
		{
			Set(std::move(val));
			return *this;
		}

		virtual auto operator==(const T& val) const noexcept -> bool
		{
			return *Get() == val;
		}

		virtual auto operator<=>(const T& val) const noexcept -> std::compare_three_way_result_t<T>
		{
			return *Get() <=> val;
		}

		virtual auto operator*() const noexcept -> AccessorProxyType
		{
			return AccessorProxyType{ value, mutex };
		}

		private:
		T value;
		mutable Mutex mutex;
		EventType valueChangedEvent{ };
	};

	template <typename T>
	using PropertyNM = Property<T, Mutex::NullMutex>;
}

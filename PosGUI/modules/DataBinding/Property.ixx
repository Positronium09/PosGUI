export module PGUI.DataBinding.Property;

import std;

import PGUI.Event;
import PGUI.Mutex;

export namespace PGUI::DataBinding
{
	template <typename T, typename Mutex = Mutex::SRWMutex>
	class Property
	{
		public:
		using ValueType = T;
		using MutexType = Mutex;
		using EventType = EventT<Mutex, T&>;

		Property() noexcept = default;

		explicit(false) Property(const T& value) noexcept :
			value{ value }
		{ }

		Property(const Property& other) noexcept :
			value{ other.Get() }
		{ }

		Property(Property&& other) noexcept :
			value{ std::move(other.Get()) }
		{ }

		virtual ~Property() = default;

		virtual auto operator=(const Property& other) noexcept -> Property&
		{
			if (this != &other)
			{
				Set(other.Get());
				valueChangedEvent.Invoke(value);
			}
			return *this;
		}

		virtual auto operator=(Property&& other) noexcept -> Property&
		{
			if (this != &other)
			{
				Set(std::move(other.Get()));
				valueChangedEvent.Invoke(value);
			}
			return *this;
		}

		[[nodiscard]] const auto& Get() const noexcept
		{
			std::scoped_lock lock{ mutex };
			return value;
		}

		[[nodiscard]] auto& Get() noexcept
		{
			std::scoped_lock lock{ mutex };
			return value;
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

		auto AddObserver(CallbackType<T&> auto callback) noexcept
		{
			return valueChangedEvent.AddCallback(callback);
		}

		auto RemoveObserver(CallbackId id) noexcept -> void
		{
			valueChangedEvent.RemoveCallback(id);
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
			std::scoped_lock lock{ mutex };
			return value == val;
		}

		virtual auto operator<=>(const T& val) const noexcept -> decltype(std::declval<T&>() <=> std::declval<const T&>())
		{
			std::scoped_lock lock{ mutex };
			return value <=> val;
		}

		virtual auto operator*() noexcept -> T&
		{
			std::scoped_lock lock{ mutex };
			return value;
		}
		virtual auto operator*() const noexcept -> const T&
		{
			std::scoped_lock lock{ mutex };
			return value;
		}

		private:
		T value;
		mutable Mutex mutex;
		EventType valueChangedEvent{ };
	};

	template <typename T>
	using PropertyNM = Property<T, Mutex::NullMutex>;
}

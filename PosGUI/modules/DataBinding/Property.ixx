export module PGUI.DataBinding.Property;

import std;

import PGUI.Event;

export namespace PGUI::DataBinding
{
	class NullMutex
	{
		public:
		// ReSharper disable CppMemberFunctionMayBeStatic
		auto lock() const noexcept -> void { }
		auto unlock() const noexcept -> void { }
		// ReSharper disable once CppInconsistentNaming
		auto try_lock() const noexcept -> bool { return true; }
		// ReSharper restore CppMemberFunctionMayBeStatic
	};

	template <typename T, typename Mutex = std::mutex>
	class Property
	{
		public:
		using ValueType = T;
		using MutexType = Mutex;
		using EventType = Event<T&>;

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

		auto operator=(const Property& other) noexcept -> Property&
		{
			if (this != &other)
			{
				Set(other.Get());
				valueChangedEvent.Invoke(value);
			}
			return *this;
		}

		auto operator=(Property&& other) noexcept -> Property&
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

		explicit(false) operator const T&() const noexcept { return Get(); }
		explicit(false) operator T&() noexcept { return Get(); }

		auto operator==(const Property& other) const noexcept -> bool
		{
			std::scoped_lock lock{ mutex };
			return value == other.Get();
		}

		auto operator==(const T& val) const noexcept -> bool
		{
			std::scoped_lock lock{ mutex };
			return value == val;
		}

		auto operator<=>(const Property& other) const noexcept
		{
			std::scoped_lock lock{ mutex };
			return value <=> other.Get();
		}

		auto operator<=>(const T& val) const noexcept
		{
			std::scoped_lock lock{ mutex };
			return value <=> val;
		}

		private:
		T value;
		mutable Mutex mutex;
		EventType valueChangedEvent{ };
	};

	template <typename T>
	using PropertyNM = Property<T, NullMutex>;
}

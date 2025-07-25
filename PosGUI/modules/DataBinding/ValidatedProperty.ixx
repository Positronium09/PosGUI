export module PGUI.DataBinding.ValidatedProperty;

import std;

import PGUI.DataBinding.Property;

export namespace PGUI::DataBinding
{
	template <typename T, typename Mutex = std::mutex>
	class ValidatedProperty final : public Property<T, Mutex>
	{
		using Validator = std::function<bool(const T&)>;

		public:
		ValidatedProperty() noexcept = default;

		ValidatedProperty(
			const T& value, 
			const std::initializer_list<Validator>& validators) noexcept :
			Property<T, Mutex>{ value }, validators{ validators }
		{ }

		explicit(false) ValidatedProperty(const T& value) noexcept :
			Property<T, Mutex>{ value }
		{ }

		ValidatedProperty(const ValidatedProperty& other) noexcept :
			Property<T, Mutex>{ other }, validators{ other.validators }
		{ }

		ValidatedProperty(ValidatedProperty&& other) noexcept :
			Property<T, Mutex>{ std::move(other) }, validators{ std::move(other.validators) }
		{ }

		~ValidatedProperty() override = default;

		auto AddValidator(const Validator& validator) -> void
		{
			validators.push_back(validator);
		}

		auto Set(const T& newValue) -> void override
		{
			if (IsValueValid(newValue))
			{
				Property<T, Mutex>::Set(newValue);
			}
		}

		auto Set(T&& newValue) -> void override
		{
			if (IsValueValid(newValue))
			{
				Property<T, Mutex>::Set(newValue);
			}
		}

		auto operator=(const T& val) noexcept -> ValidatedProperty&
		{
			Set(val);
			return *this;
		}

		auto operator=(T&& val) noexcept -> ValidatedProperty&
		{
			Set(val);
			return *this;
		}

		explicit(false) operator const T&() const noexcept { return Property<T, Mutex>::Get(); }
		explicit(false) operator T&() noexcept { return Property<T, Mutex>::Get(); }

		auto operator==(const Property<T, Mutex>& other) const noexcept -> bool
		{
			return Property<T, Mutex>::operator==(other);
		}

		auto operator==(const T& val) const noexcept -> bool
		{
			return Property<T, Mutex>::operator==(val);
		}

		auto operator<=>(const Property<T, Mutex>& other) const noexcept
		{
			return Property<T, Mutex>::operator<=>(other);
		}

		auto operator<=>(const T& val) const noexcept
		{
			return Property<T, Mutex>::operator<=>(val);
		}

		auto operator=(const Property<T, Mutex>& other) noexcept -> ValidatedProperty&
		{
			if (this != &other)
			{
				Set(other.Get());
			}
			return *this;
		}

		auto operator=(const ValidatedProperty& other) noexcept -> ValidatedProperty&
		{
			if (this != &other)
			{
				Set(other.Get());
			}
			return *this;
		}

		auto operator=(Property<T, Mutex>&& other) noexcept -> ValidatedProperty&
		{
			if (this != &other)
			{
				Set(std::move(other.Get()));
			}
			return *this;
		}

		auto operator=(ValidatedProperty&& other) noexcept -> ValidatedProperty&
		{
			if (this != &other)
			{
				Set(std::move(other.Get()));
			}
			return *this;
		}

		private:
		std::vector<Validator> validators{ };

		[[nodiscard]] auto IsValueValid(const T& value) const
		{
			return std::ranges::all_of(validators, [&value](const auto& validator)
			{
				return std::invoke(validator, value);
			});
		}
		[[nodiscard]] auto IsValueValid(T&& value) const
		{
			return std::ranges::all_of(validators, [&value](const auto& validator)
			{
				return std::invoke(validator, value);
			});
		}
	};

	template <typename T>
	using ValidatedPropertyNM = ValidatedProperty<T, NullMutex>;
}

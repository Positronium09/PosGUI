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

		auto operator=(const T& val) noexcept -> ValidatedProperty& override
		{
			Set(val);
			return *this;
		}

		auto operator=(T&& val) noexcept -> ValidatedProperty& override
		{
			Set(val);
			return *this;
		}

		auto operator==(const T& val) const noexcept -> bool override
		{
			return Property<T, Mutex>::operator==(val);
		}

		auto operator<=>(const T& val) const noexcept -> decltype(std::declval<T&>() <=> std::declval<const T&>()) override
		{
			return Property<T, Mutex>::operator<=>(val);
		}

		auto operator=(const Property<T, Mutex>& other) noexcept -> ValidatedProperty& override
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

		auto operator=(Property<T, Mutex>&& other) noexcept -> ValidatedProperty& override
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
		auto operator*() noexcept -> T& override
		{
			return Property<T, Mutex>::operator*();
		}
		auto operator*() const noexcept -> const T& override
		{
			return Property<T, Mutex>::operator*();
		}

		[[nodiscard]] auto GetValidators() const noexcept -> const std::vector<Validator>&
		{
			return validators;
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

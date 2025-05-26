module;
#include <mutex>
#include <ranges>
#include <algorithm>
#include <functional>
#include <type_traits>

export module PGUI.DataBinding.ValidatedProperty;

import PGUI.DataBinding.Property;

export namespace PGUI::DataBinding
{
	template <typename T, typename Mutex = std::mutex>
	class ValidatedProperty : public Property<T, Mutex>
	{
		using Validator = std::function<bool(const T&)>;

		public:
		ValidatedProperty() noexcept = default;
		explicit(false) ValidatedProperty(const T& value) noexcept :
			Property<T, Mutex>{ value }
		{
		}

		ValidatedProperty(const ValidatedProperty& other) noexcept :
			Property<T, Mutex>{ other }
		{
		}

		ValidatedProperty(ValidatedProperty&& other) noexcept :
			Property<T, Mutex>{ other }
		{
		}
		virtual ~ValidatedProperty() = default;

		void AddValidator(const Validator& validator)
		{
			validators.push_back(validator);
		}

		void Set(const T& newValue) override
		{
			auto validated = std::ranges::all_of(validators, [&newValue](const auto& validator)
			{
				return std::invoke(validator, newValue);
			});
			if (validated)
			{
				Property<T, Mutex>::Set(newValue);
			}
		}
		void Set(T&& newValue) override
		{
			auto validated = std::ranges::all_of(validators, [&newValue](const auto& validator)
			{
				return std::invoke(validator, newValue);
			});
			if (validated)
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

		explicit(false) operator const T& () const noexcept { return Property<T, Mutex>::Get(); }
		explicit(false) operator T& () noexcept { return Property<T, Mutex>::Get(); }

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
	};
}

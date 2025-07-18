export module PGUI.DataBinding.DerivedProperty;

import std;

import PGUI.Event;
import PGUI.DataBinding.Property;

export namespace PGUI::DataBinding
{
	template <typename T, typename Mutex = std::mutex>
	class DerivedProperty final : public Property<T, Mutex>
	{
		public:
		template <typename Deriver, typename... Dependencies>
			requires std::invocable<Deriver, const typename Dependencies::ValueType&...> &&
			         std::convertible_to<std::invoke_result_t<Deriver, const typename Dependencies::ValueType&...>, T>
		explicit DerivedProperty(const Deriver& deriver,
		                         Dependencies&... dependencies) noexcept
		{
			auto update = [this, deriver, &dependencies...]
			{
				Property<T, Mutex>::Set(deriver(dependencies.Get()...));
			};

			(dependencies.AddObserver([update](const auto&) { update(); }), ...);
			update();
		};

		DerivedProperty(const DerivedProperty& other) noexcept :
			Property<T, Mutex>{ other }
		{ }

		DerivedProperty(DerivedProperty&& other) noexcept :
			Property<T, Mutex>{ other }
		{ }

		~DerivedProperty() override = default;

		explicit(false) operator const T&() const { return Property<T, Mutex>::Get(); }
		explicit(false) operator T&() { return Property<T, Mutex>::Get(); }

		template <typename OtherMutex>
		auto operator==(const Property<T, OtherMutex>& other) const noexcept -> bool
		{
			return Property<T, Mutex>::operator==(other);
		}

		auto operator==(const T& val) const noexcept -> bool
		{
			return Property<T, Mutex>::operator==(val);
		}

		template <typename OtherMutex>
		auto operator<=>(const Property<T, OtherMutex>& other) const noexcept
		{
			return Property<T, Mutex>::operator<=>(other);
		}

		auto operator<=>(const T& val) const noexcept
		{
			return Property<T, Mutex>::operator<=>(val);
		}
	};

	template <typename T>
	using DerivedPropertyNM = DerivedProperty<T, NullMutex>;
}

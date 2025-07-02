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
			requires std::invocable<Deriver, const Dependencies&...> &&
			         std::convertible_to<std::invoke_result_t<Deriver, const Dependencies&...>, T>
		explicit DerivedProperty(const Deriver& deriver,
		                         Property<Dependencies>&... dependencies) noexcept
		{
			auto update = [this, deriver, &dependencies...]()
			{
				Property<T, Mutex>::Set(deriver(dependencies.Get()...));
			};

			(dependencies.AddObserver([update](const auto&) { update(); }), ...);
			update();
		};

		DerivedProperty(const DerivedProperty& other) noexcept :
			Property<T>{ other }
		{ }

		DerivedProperty(DerivedProperty&& other) noexcept :
			Property<T>{ other }
		{ }

		~DerivedProperty() override = default;

		explicit(false) operator const T&() const { return Property<T>::Get(); }
		explicit(false) operator T&() { return Property<T>::Get(); }

		auto operator==(const Property<T>& other) const noexcept -> bool
		{
			return Property<T>::operator==(other);
		}

		auto operator==(const T& val) const noexcept -> bool
		{
			return Property<T>::operator==(val);
		}

		auto operator<=>(const Property<T>& other) const noexcept
		{
			return Property<T>::operator<=>(other);
		}

		auto operator<=>(const T& val) const noexcept
		{
			return Property<T>::operator<=>(val);
		}
	};
}

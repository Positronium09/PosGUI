export module PGUI.DataBinding.DerivedProperty;

import std;

import PGUI.Event;
import PGUI.Mutex;
import PGUI.DataBinding.Property;

export namespace PGUI::DataBinding
{
	template <typename T, typename Mutex = Mutex::SRWMutex>
	class DerivedProperty final : public Property<T, Mutex>
	{
		public:
		template <typename Deriver, typename... Dependencies>
			requires std::invocable<Deriver, const Dependencies::ValueType&...> &&
			         std::convertible_to<std::invoke_result_t<Deriver, const Dependencies::ValueType&...>, T>
		explicit DerivedProperty(const Deriver& deriver,
		                         Dependencies&... dependencies) noexcept
		{
			auto update = [this, deriver, &dependencies...]
			{
				Property<T, Mutex>::Set(deriver(dependencies.Get()...));
			};

			(unsubscribers.push_back(
				[&dep = dependencies, 
				id = dependencies.AddObserver([update](const auto&) { update(); })]
			{
				dep.RemoveObserver(id);
			}), ...);
			update();
		};

		DerivedProperty(const DerivedProperty&) noexcept = delete;
		DerivedProperty(DerivedProperty&&) noexcept = delete;
		auto operator=(const DerivedProperty&) noexcept -> DerivedProperty&  = delete;
		auto operator=(DerivedProperty&&) noexcept -> DerivedProperty& = delete;

		~DerivedProperty() override
		{
			for (auto& unsubscriber : unsubscribers)
			{
				unsubscriber();
			}
		}

		template <typename OtherMutex>
		auto operator==(const Property<T, OtherMutex>& other) const noexcept -> bool
		{
			return Property<T, Mutex>::operator==(other.Get());
		}

		auto operator==(const T& val) const noexcept -> bool override
		{
			return Property<T, Mutex>::operator==(val);
		}

		template <typename OtherMutex>
		auto operator<=>(const Property<T, OtherMutex>& other) const noexcept
		{
			return Property<T, Mutex>::operator<=>(other.Get());
		}

		auto operator<=>(const T& val) const noexcept -> std::compare_three_way_result_t<T> override
		{
			return Property<T, Mutex>::operator<=>(val);
		}

		private:
		std::vector<std::move_only_function<void()>> unsubscribers;
	};

	template <typename T>
	using DerivedPropertyNM = DerivedProperty<T, Mutex::NullMutex>;
}


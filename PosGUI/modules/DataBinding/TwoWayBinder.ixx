export module PGUI.DataBinding.TwoWayBinder;

import std;

import PGUI.Utils;
import PGUI.Event;
import PGUI.DataBinding.Property;

export namespace PGUI::DataBinding
{
	template <typename Type1, typename Type2>
	class TwoWayBinder
	{
		public:
		TwoWayBinder(Property<Type1>& property1, Property<Type2>& property2) noexcept :
			property1{ property1 }, property2{ property2 }
		{
			observer1Id = property1.AddObserver([this, &property2](const auto& value)
			{
				if (isUpdating.load(std::memory_order_relaxed))
				{
					return;
				}
				isUpdating.store(true, std::memory_order_relaxed);
				ScopedDefer resetUpdating{ [this]
				{
					isUpdating.store(false, std::memory_order_relaxed);
				} };

				property2.Set(value);
			});
			observer2Id = property2.AddObserver([this, &property1](const auto& value)
			{
				if (isUpdating.load(std::memory_order_relaxed))
				{
					return;
				}

				isUpdating.store(true, std::memory_order_relaxed);
				ScopedDefer resetUpdating{ [this]
				{
					isUpdating.store(false, std::memory_order_relaxed);
				} };

				property1.Set(value);
			});
		}

		template <std::invocable<const Type1&> Func1, std::invocable<const Type2&> Func2>
			requires std::convertible_to<std::invoke_result_t<Func1, const Type1&>, Type2> &&
			         std::convertible_to<std::invoke_result_t<Func2, const Type2&>, Type1>
		TwoWayBinder(Property<Type1>& property1, Property<Type2>& property2,
		             Func1 converter1, Func2 converter2) noexcept :
			property1{ property1 }, property2{ property2 }
		{
			observer1Id = property1.AddObserver([this, &property2, converter1 = std::move(converter1)](const auto& value)
			{
				if (isUpdating.load(std::memory_order_relaxed))
				{
					return;
				}
				
				isUpdating.store(true, std::memory_order_relaxed);
				ScopedDefer resetUpdating{ [this]
				{
					isUpdating.store(false, std::memory_order_relaxed);
				} };

				property2.Set(static_cast<Type2>(converter1(value)));
			});
			observer2Id = property2.AddObserver([this, &property1, converter2 = std::move(converter2)](const auto& value)
			{
				if (isUpdating.load(std::memory_order_relaxed))
				{
					return;
				}

				isUpdating.store(true, std::memory_order_relaxed);
				ScopedDefer resetUpdating{ [this]
				{
					isUpdating.store(false, std::memory_order_relaxed);
				} };

				property1.Set(static_cast<Type1>(converter2(value)));
			});
		}

		TwoWayBinder(const TwoWayBinder&) noexcept = delete;
		auto operator=(const TwoWayBinder&) noexcept -> TwoWayBinder & = delete;
		TwoWayBinder(TwoWayBinder&&) noexcept = delete;
		auto operator=(TwoWayBinder&&) noexcept -> TwoWayBinder & = delete;

		~TwoWayBinder() noexcept
		{
			property1.get().RemoveObserver(observer1Id);
			property2.get().RemoveObserver(observer2Id);
		}

		private:
		std::atomic_bool isUpdating{ false };
		std::reference_wrapper<Property<Type1>> property1;
		std::reference_wrapper<Property<Type2>> property2;
		CallbackId observer1Id{ };
		CallbackId observer2Id{ };
	};
}

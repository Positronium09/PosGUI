export module PGUI.DataBinding.OneWayBinder;

import std;

import PGUI.Event;
import PGUI.DataBinding.Property;

export namespace PGUI::DataBinding
{
	template <typename SourceType, typename TargetType>
	class OneWayBinder
	{
		public:
		OneWayBinder(Property<SourceType>& source, Property<TargetType>& target) noexcept :
			source{ source }
		{
			observerId = source.AddObserver([&target](const auto& value)
			{
				target.Set(value);
			});
		}

		template <std::invocable<const SourceType&> Func>
			requires std::convertible_to<std::invoke_result_t<Func, const SourceType&>, TargetType>
		OneWayBinder(Property<SourceType>& source, Property<TargetType>& target,
		             Func converter) noexcept :
			source{ source }
		{
			observerId = source.AddObserver([&target, converter = std::move(converter)](const auto& value)
			{
				target.Set(static_cast<TargetType>(converter(value)));
			});
		}

		OneWayBinder(const OneWayBinder&) noexcept = delete;
		OneWayBinder(OneWayBinder&&) noexcept = delete;
		auto operator=(const OneWayBinder&) noexcept -> OneWayBinder& = delete;
		auto operator=(OneWayBinder&&) noexcept -> OneWayBinder& = delete;

		~OneWayBinder() noexcept
		{
			source.get().RemoveObserver(observerId);
		}

		private:
		std::reference_wrapper<Property<SourceType>> source;
		CallbackId observerId{ };
	};
}

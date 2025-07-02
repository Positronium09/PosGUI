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
		             const Func& converter) noexcept :
			source{ source }
		{
			observerId = source.AddObserver([&target, &converter](const auto& value)
			{
				target.Set(static_cast<TargetType>(converter(value)));
			});
		}

		~OneWayBinder() noexcept
		{
			source.get().RemoveObserver(observerId);
		}

		private:
		std::reference_wrapper<Property<SourceType>> source;
		CallbackId observerId{ };
	};
}

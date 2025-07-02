export module PGUI.DataBinding;

import std;

export import PGUI.DataBinding.Property;
export import PGUI.DataBinding.OneWayBinder;
export import PGUI.DataBinding.TwoWayBinder;
export import PGUI.DataBinding.ValidatedProperty;
export import PGUI.DataBinding.DerivedProperty;

export namespace PGUI::DataBinding
{
	template <typename SourceType, typename TargetType>
	[[nodiscard]] auto Bind(Property<SourceType>& source, Property<TargetType>& target)
	{
		return OneWayBinder{ source, target };
	}

	template <typename SourceType, typename TargetType, typename Func>
		requires std::invocable<Func, const SourceType&> &&
		         std::convertible_to<std::invoke_result_t<Func, const SourceType&>, TargetType>
	[[nodiscard]] auto Bind(Property<SourceType>& source, Property<TargetType>& target,
	                        const Func& converter)
	{
		return OneWayBinder{ source, target, converter };
	}

	template <typename Type1, typename Type2>
	[[nodiscard]] auto BindTwoWay(Property<Type1>& source, Property<Type2>& target)
	{
		return TwoWayBinder{ source, target };
	}

	template <typename Type1, typename Type2, typename Func1, typename Func2>
		requires std::invocable<Func1, const Type1&> &&
		         std::invocable<Func2, const Type2&> &&
		         std::convertible_to<std::invoke_result_t<Func1, const Type1&>, Type2> &&
		         std::convertible_to<std::invoke_result_t<Func2, const Type2&>, Type1>
	[[nodiscard]] auto BindTwoWay(Property<Type1>& property1, Property<Type2>& property2,
	                              const Func1& converter1, const Func2& converter2)
	{
		return TwoWayBinder{ property1, property2, converter1, converter2 };
	}
}

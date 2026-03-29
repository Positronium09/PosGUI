export module PGUI.Utils:MetaUtils;

import std;

namespace PGUI::Detail
{
	struct DummyTemplateCheck { };

	template <typename, template <auto...> typename>
	struct IsSpecializationHelper : std::false_type { };

	template <template <auto...> typename Template, auto... Args>
	struct IsSpecializationHelper<Template<Args...>, Template> : std::true_type { };
}

export namespace PGUI
{
	template <typename T, typename... Types>
	concept IsInTypeList = (std::same_as<T, Types> || ...);

	template <size_t N, typename... Types>
	using NthTypeOf = std::tuple_element_t<N, std::tuple<Types...>>;

	template <typename... Types>
	using FirstTypeOf = NthTypeOf<0, Types...>;

	template <typename... Types>
	constexpr auto TypeCount = sizeof...(Types);

	template <typename... Operands>
	constexpr auto LogicalOr = std::disjunction_v<Operands...>;

	template <typename... Operands>
	constexpr auto LogicalAnd = std::conjunction_v<Operands...>;

	template <typename Operand>
	constexpr auto LogicalNot = std::negation_v<Operand>;

	template <typename T, typename U>
	concept NotSameAs = !std::same_as<T, U>;

	template <typename... Types>
	struct TypeList
	{
		static constexpr auto Count = TypeCount<Types...>;

		template <std::size_t N>
		using GetType = NthTypeOf<N, Types...>;

		using FirstType = FirstTypeOf<Types...>;

		using LastType = NthTypeOf<Count - 1, Types...>;

		template <template <typename...> typename T>
		using Rebind = T<Types...>;

		template <typename T>
		static constexpr auto Contains = IsInTypeList<T, Types...>;
	};

	template <typename T, template <auto...> typename Template>
	concept IsSpecialization = Detail::IsSpecializationHelper<T, Template>::value;

	template <typename Variant, typename... Visitors>
	constexpr auto Match(Variant&& variant, Visitors&&... visitors)
	{
		struct Overloaded : Visitors...
		{
			using Visitors::operator()...;
		};

		return std::visit(
			Overloaded{ std::forward<Visitors>(visitors)... }, 
			std::forward<Variant>(variant));
	}
}

export module PGUI.Utils:MetaUtils;

import std;

namespace PGUI::Detail
{
	struct DummyTemplateCheck { };

	template <typename, template <typename...> typename>
	struct IsSpecializationHelper : std::false_type { };

	template <template <typename...> typename Template, typename... Args>
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

	template <typename... Types>
	struct TypeList
	{
		static constexpr auto Count = sizeof...(Types);

		template <std::size_t N>
		using GetType = NthTypeOf<N, Types...>;

		using FirstType = FirstTypeOf<Types...>;

		using LastType = NthTypeOf<Count - 1, Types...>;

		template <template <typename...> typename T>
		using Rebind = T<Types...>;
	};

	template <typename T, template <typename...> typename Template>
	concept IsSpecialization = Detail::IsSpecializationHelper<T, Template>::value;
}
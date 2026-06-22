export module PGUI.Utils:MetaUtils;

import std;

namespace PGUI::Detail
{
	struct DummyTemplateCheck { };

	template <typename, template <typename...> typename>
	struct IsSpecializationHelper : std::false_type { };

	template <template <typename...> typename Template, typename... Args>
	struct IsSpecializationHelper<Template<Args...>, Template> : std::true_type { };

	template <typename, typename>
	struct IsCallable : std::false_type { };

	template <typename T, typename ReturnT, typename... Args>
	struct IsCallable<T, ReturnT(Args...)> : std::bool_constant<std::is_invocable_r_v<ReturnT, T, Args...>>{ };

	template <typename... Lists>
	struct ConcatTypeLists;

	template <typename>
	struct MemberFunctionClass;

	template <typename T, typename ReturnT, typename... Args>
	struct MemberFunctionClass<ReturnT(T::*)(Args...)>
	{
		using type = T;
	};

	template <typename T, typename ReturnT, typename... Args>
	struct MemberFunctionClass<ReturnT(T::*)(Args...) noexcept>
	{
		using type = T;
	};

	template <typename T, typename ReturnT, typename... Args>
	struct MemberFunctionClass<ReturnT(T::*)(Args...) const>
	{
		using type = T;
	};

	template <typename T, typename ReturnT, typename... Args>
	struct MemberFunctionClass<ReturnT(T::*)(Args...) const noexcept>
	{
		using type = T;
	};

	template <typename>
	struct IsConstMemberFunction : std::false_type { };

	template <typename T, typename ReturnT, typename... Args>
	struct IsConstMemberFunction<ReturnT(T::*)(Args...) const> : std::true_type { };

	template <typename T, typename ReturnT, typename... Args>
	struct IsConstMemberFunction<ReturnT(T::*)(Args...) const noexcept> : std::true_type {};
}

export namespace PGUI
{
	template <typename T, typename Signature>
	concept Callable = Detail::IsCallable<T, Signature>::value;

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

	template <typename T>
	using MemberFunctionClass = Detail::MemberFunctionClass<T>::type;

	template <typename T>
	concept ConstMemberFunction = Detail::IsConstMemberFunction<T>::value;

	template <typename T>
	concept NonConstMemberFunction = !ConstMemberFunction<T>;

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

		template <typename... Ts>
		using PushBack = TypeList<Types..., Ts...>;

		template <typename... Ts>
		using PushFront = TypeList<Ts..., Types...>;

		template <typename... Lists>
		using Append = Detail::ConcatTypeLists<TypeList, Lists...>::Type;
	};

	template <typename T, template <typename...> typename Template>
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

namespace PGUI::Detail
{
	template <>
	struct ConcatTypeLists<>
	{
		using Type = TypeList<>;
	};

	template <typename... Ts>
	struct ConcatTypeLists<TypeList<Ts...>>
	{
		using Type = TypeList<Ts...>;
	};

	template <typename... Ts, typename... Us, typename... Rest>
	struct ConcatTypeLists<TypeList<Ts...>, TypeList<Us...>, Rest...>
	{
		using Type = typename ConcatTypeLists<TypeList<Ts..., Us...>, Rest...>::Type;
	};
}

export namespace PGUI
{
	template <typename... Lists>
	using CombineTypeLists = typename Detail::ConcatTypeLists<Lists...>::Type;
}
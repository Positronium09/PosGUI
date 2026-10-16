export module PGUI.Utils:MetaUtils;

import std;

namespace PGUI::MetaUtilsDetail
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

	template <typename T, typename... Types>
	constexpr auto FindTypeIndex() noexcept -> std::size_t
	{
		const auto matches = std::array{ std::same_as<T, Types>... };
		return static_cast<std::size_t>(std::ranges::find(matches, true) - matches.begin());
	}
}

export namespace PGUI
{
	template <typename T, typename Signature>
	concept Callable = MetaUtilsDetail::IsCallable<T, Signature>::value;

	template <typename T, typename... Types>
	concept IsInTypeList = (std::same_as<T, Types> || ...);

	template <size_t N, typename... Types>
	using NthTypeOf = std::tuple_element_t<N, std::tuple<Types...>>;

	template <typename... Types>
	using FirstTypeOf = NthTypeOf<0, Types...>;

	template <typename... Types>
	constexpr auto TypeCount = sizeof...(Types);

	template <bool... Operands>
	constexpr auto LogicalOr = (Operands || ...);

	template <bool... Operands>
	constexpr auto LogicalAnd = (Operands && ...);

	template <bool Operand>
	constexpr auto LogicalNot = !Operand;

	template <typename T, typename U>
	concept NotSameAs = !std::same_as<T, U>;

	template <typename T>
	using MemberFunctionClass = MetaUtilsDetail::MemberFunctionClass<T>::type;

	template <typename T>
	concept ConstMemberFunction = MetaUtilsDetail::IsConstMemberFunction<T>::value;

	template <typename T>
	concept NonConstMemberFunction = !ConstMemberFunction<T>;

	template <typename T, typename... Types> requires IsInTypeList<T, Types...>
	constexpr auto TypeIndex = MetaUtilsDetail::FindTypeIndex<T, Types...>();

	template <typename... Types>
	struct TypeList
	{
		static constexpr auto Count = TypeCount<Types...>;

		static constexpr auto IsEmpty = Count == 0;

		template <typename T>
		static constexpr auto IndexOf = TypeIndex<T, Types...>;

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
		using Append = MetaUtilsDetail::ConcatTypeLists<TypeList, Lists...>::Type;
	};

	template <typename T1, typename T2>
	struct TypePair
	{
		using First = T1;
		using Second = T2;
	};

	template <typename T, template <typename...> typename Template>
	concept IsSpecialization = MetaUtilsDetail::IsSpecializationHelper<T, Template>::value;

	template <IsSpecialization<TypePair>... Pairs>
	struct TypeMap : TypeList<Pairs...>
	{
		using Keys = TypeList<typename Pairs::First...>;
		using Values = TypeList<typename Pairs::Second...>;
		using Map = TypeList<Pairs...>;

		template <typename T> requires Keys::template Contains<T>
		using GetMappedType = Values::template GetType<Keys::template IndexOf<T>>;
	};

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

	template <typename CharT, std::size_t N>
	struct TStringLiteral
	{
		explicit(false) constexpr TStringLiteral(const CharT (&str)[N]) noexcept
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				data[i] = str[i];
			}
		}

		CharT data[N];
	};

	template <std::size_t N>
	using WStringLiteral = TStringLiteral<wchar_t, N>;

	template <std::size_t N>
	using StringLiteral = TStringLiteral<char, N>;

	template <typename CharT, std::size_t... Ns>
	consteval auto Concat(TStringLiteral<CharT, Ns>... literals) noexcept
	{
		constexpr std::size_t totalSize{ (Ns + ... + 0) - sizeof...(Ns) + 1 };
		CharT result[totalSize]{ };
		std::size_t index = 0;

		((std::copy_n(literals.data, Ns - 1, result + index), index += Ns - 1), ...);
		result[totalSize - 1] = CharT{ 0 };

		return TStringLiteral<CharT, totalSize>{ result };
	}
}

namespace PGUI::MetaUtilsDetail
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
		using Type = ConcatTypeLists<TypeList<Ts..., Us...>, Rest...>::Type;
	};
}

export namespace PGUI
{
	template <typename... Lists>
	using CombineTypeLists = MetaUtilsDetail::ConcatTypeLists<Lists...>::Type;
}

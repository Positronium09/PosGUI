export module PGUI.Utils;

import std;

export import :WindowsUtils;
export import :StringUtils;
export import :ArithmeticUtils;
export import :EnumUtils;

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
	template <typename T> requires !std::is_same_v<T, void>
	using RawPtr = T*;
	template <typename T> requires !std::is_same_v<T, void>
	using ConstRawPtr = const T*;

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

	enum class PositioningMode
	{
		Absolute,
		Relative
	};

	template <typename T, template <typename...> typename Template>
	concept IsSpecialization = Detail::IsSpecializationHelper<T, Template>::value;

	template <typename T> requires std::is_trivially_copyable_v<T>
	struct ScopedValue
	{
		constexpr ScopedValue(T& variableRef, T newValue) noexcept :
			variable{ variableRef }, oldValue{ std::exchange(variableRef, newValue) }
		{
		}
		constexpr ~ScopedValue() noexcept
		{
			variable.get() = oldValue;
		}

		private:
		std::reference_wrapper<T> variable;
		T oldValue;
	};

	namespace Hash
	{
		constexpr auto GoldenRatioSeed = std::size_t{ 0x9E3779B9 };

		template <typename T>
		auto CombineHash(std::size_t& seed, const T& value) noexcept -> void
		{
			std::hash<T> hasher;

			seed ^= hasher(value) + GoldenRatioSeed + (seed << 6) + (seed >> 2);
		}

		template <typename... ToHash>
		[[nodiscard]] auto ComputeHash(const ToHash&... values) noexcept
		{
			if constexpr (TypeCount<ToHash...> == 1)
			{
				return std::hash<FirstTypeOf<ToHash...>>{ }(std::get<0>(std::forward_as_tuple(values...)));
			}

			std::size_t seed = 0;

			(CombineHash(seed, values), ...);
			
			return seed;
		}
	}

	struct StringHash
	{
		// ReSharper disable once CppInconsistentNaming
		using is_transparent = void;

		[[nodiscard]] auto operator()(const char* txt) const -> size_t { return std::hash<std::string_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::string_view txt) const -> size_t { return std::hash<std::string_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::string& txt) const -> size_t { return std::hash<std::string_view>{ }(txt); }
	};

	struct WStringHash
	{
		// ReSharper disable once CppInconsistentNaming
		using is_transparent = void;

		[[nodiscard]] auto operator()(const wchar_t* txt) const -> size_t { return std::hash<std::wstring_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::wstring_view txt) const -> size_t { return std::hash<std::wstring_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::wstring& txt) const -> size_t { return std::hash<std::wstring_view>{ }(txt); }
	};
}

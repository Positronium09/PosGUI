export module PGUI.Utils;

import std;

export import :WindowsUtils;
export import :StringUtils;
export import :ArithmeticUtils;
export import :EnumUtils;

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

	enum class PositioningMode
	{
		Absolute,
		Relative
	};

	template <typename T> requires std::is_trivially_copyable_v<T>
	struct ScopedValue
	{
		ScopedValue(T& variableRef, T newValue) noexcept :
			variable{ variableRef }, oldValue{ variableRef }
		{
			variable = newValue;
		}
		~ScopedValue() noexcept
		{
			variable = oldValue;
		}

		private:
		std::reference_wrapper<T> variable;
		T oldValue;
	};
}

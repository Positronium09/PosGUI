export module PGUI.Utils;

import std;

export import :WindowsUtils;
export import :StringUtils;
export import :ArithmeticUtils;
export import :EnumUtils;
export import :MetaUtils;
export import :HashUtils;

export namespace PGUI
{
	template <typename T> requires !std::is_same_v<T, void>
	using RawPtr = T*;
	template <typename T> requires !std::is_same_v<T, void>
	using ConstRawPtr = const T*;
	
	enum class PositioningMode
	{
		Absolute,
		Relative
	};

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
}

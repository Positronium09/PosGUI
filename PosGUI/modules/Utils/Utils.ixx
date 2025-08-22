export module PGUI.Utils;

import std;

export import :WindowsUtils;
export import :StringUtils;
export import :ArithmeticUtils;


export namespace PGUI
{
	template <typename T> requires !std::is_same_v<T, void>
	using RawPtr = T*;
	template <typename T> requires !std::is_same_v<T, void>
	using ConstRawPtr = const T*;

	template <typename T>
	concept Enumeration = std::is_enum_v<T>;

	template <Enumeration T>
	using UnderlyingType = std::underlying_type_t<T>;

	template <typename T, typename... Types>
	concept IsAnyOf = (std::same_as<T, Types> || ...);

	enum class PositioningMode
	{
		Absolute,
		Relative
	};

	template <typename T>
	auto IsFlagSet(T var, T flag)
	{
		return (var & flag) != static_cast<T>(0);
	}

	template <typename T>
	auto ToUnderlying(T e) noexcept
	{
		return static_cast<UnderlyingType<T>>(e);
	}
}

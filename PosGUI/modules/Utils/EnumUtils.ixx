export module PGUI.Utils:EnumUtils;

import std;

export namespace PGUI
{
	template <typename T>
	concept Enumeration = std::is_enum_v<T>;

	template <typename T>
	concept EnumFlag = Enumeration<T> && requires(T a, T b)
	{
		{ ~a } -> std::same_as<T>;
		{ a | b } -> std::same_as<T>;
		{ a& b } -> std::same_as<T>;
		{ a^ b } -> std::same_as<T>;
		{ a |= b } -> std::same_as<T&>;
		{ a &= b } -> std::same_as<T&>;
		{ a ^= b } -> std::same_as<T&>;
	};

	template <Enumeration T>
	using UnderlyingType = std::underlying_type_t<T>;

	template <EnumFlag T>
	[[nodiscard]] auto IsFlagSet(T var, T flag)
	{
		return (var & flag) != static_cast<T>(0);
	}

	template <Enumeration T>
	auto ToUnderlying(T val) noexcept
	{
		return static_cast<UnderlyingType<T>>(val);
	}

	template <EnumFlag T>
	[[nodiscard]] constexpr auto ZeroFlag() noexcept
	{
		return static_cast<T>(0);
	}

	auto SetFlag(EnumFlag auto& var, EnumFlag auto flag) -> void
	{
		var |= flag;
	}
	template <EnumFlag T>
	constexpr auto ClearFlag(T& var, T flag) noexcept -> void
	{
		var &= static_cast<decltype(var)>(~ToUnderlying(flag));
	}
	template <EnumFlag T>
	constexpr auto ClearEnum(T& var) -> void
	{
		var = ZeroFlag<T>();
	}
}

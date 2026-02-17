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
		{ a & b } -> std::same_as<T>;
		{ a ^ b } -> std::same_as<T>;
		{ a |= b } -> std::same_as<T&>;
		{ a &= b } -> std::same_as<T&>;
		{ a ^= b } -> std::same_as<T&>;
	};

	template <Enumeration T>
	using UnderlyingType = std::underlying_type_t<T>;

	template <EnumFlag T>
	[[nodiscard]] constexpr auto ZeroFlag() noexcept
	{
		return static_cast<T>(0);
	}

	template <EnumFlag T>
	[[nodiscard]] constexpr auto IsFlagSet(const T var, const T flag)
	{
		return (var & flag) != ZeroFlag<T>();
	}
	template <EnumFlag T>
    [[nodiscard]] constexpr auto AreAllFlagsSet(const T var, const T flag)
    {
        return (var & flag) == flag;
    }

	template <Enumeration T>
	constexpr auto ToUnderlying(const T val) noexcept
	{
		return std::to_underlying(val);
	}

	template <Enumeration T>
	constexpr auto FromUnderlying(const UnderlyingType<T> val) noexcept
	{
		return static_cast<T>(val);
	}

	template <EnumFlag T>
	constexpr auto SetFlag(T& var, const T flag) -> void
	{
		var |= flag;
	}
	template <EnumFlag T>
	constexpr auto ToggleFlag(T& var, const T flag) noexcept -> void
	{
		var ^= flag;
	}
	template <EnumFlag T>
	constexpr auto ClearFlag(T& var, const T flag) noexcept -> void
	{
		var &= ~flag;
	}
	template <EnumFlag T>
	constexpr auto ClearEnum(T& var) -> void
	{
		var = ZeroFlag<T>();
	}
}

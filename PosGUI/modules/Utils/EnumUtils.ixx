export module PGUI.Utils:EnumUtils;

import std;

export namespace PGUI
{
	template <typename T>
	concept Enumeration = std::is_enum_v<T>;

	template <typename T>
	concept EnumFlag = Enumeration<T> && requires(T t)
	{
		MakeEnumFlag(t);
	};

	template <Enumeration T>
	using UnderlyingType = std::underlying_type_t<T>;
}

export
{
	template <PGUI::EnumFlag T>
	[[nodiscard]] constexpr auto operator|(T lhs, T rhs) noexcept -> T
	{
		return static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs));
	}

	template <PGUI::EnumFlag T>
	[[nodiscard]] constexpr auto operator&(T lhs, T rhs) noexcept -> T
	{
		return static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs));
	}

	template <PGUI::EnumFlag T>
	[[nodiscard]] constexpr auto operator^(T lhs, T rhs) noexcept -> T
	{
		return static_cast<T>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
	}

	template <PGUI::EnumFlag T>
	[[nodiscard]] constexpr auto operator~(T value) noexcept -> T
	{
		return static_cast<T>(~std::to_underlying(value));
	}

	template <PGUI::EnumFlag T>
	constexpr auto operator|=(T& lhs, T rhs) noexcept -> T&
	{
		lhs = lhs | rhs;

		return lhs;
	}

	template <PGUI::EnumFlag T>
	constexpr auto operator&=(T& lhs, T rhs) noexcept -> T&
	{
		lhs = lhs & rhs;

		return lhs;
	}

	template <PGUI::EnumFlag T>
	constexpr auto operator^=(T& lhs, T rhs) noexcept -> T&
	{
		lhs = lhs ^ rhs;

		return lhs;
	}
}

export namespace PGUI
{
	template <EnumFlag T>
	[[nodiscard]] constexpr auto ZeroFlag() noexcept
	{
		return static_cast<T>(0);
	}

	template <typename UnderlyingT>
	[[nodiscard]] constexpr auto ToUnderlying(const Enumeration auto val) noexcept
	{
		return static_cast<UnderlyingT>(val);
	}

	[[nodiscard]] constexpr auto ToUnderlying(const Enumeration auto val) noexcept
	{
		return std::to_underlying(val);
	}

	template <EnumFlag T>
	[[nodiscard]] constexpr auto IsFlagSet(const T var, const T flag) noexcept
	{
		return (var & flag) != ZeroFlag<T>();
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto AreFlagsSet(const T var, const std::same_as<T> auto... flags) noexcept
	{
		return (IsFlagSet(var, flags) && ...);
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto IsAnyFlagSet(const T var, const std::same_as<T> auto... flags) noexcept
	{
		return (IsFlagSet(var, flags) || ...);
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto AreAllFlagsSet(const T var, const T flag) noexcept
	{
		return (var & flag) == flag;
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto AreAllFlagsSet(const T var, const std::same_as<T> auto... flags) noexcept
	{
		return (AreAllFlagsSet(var, flags) && ...);
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto IsNoFlagSet(const T var, const T flag) noexcept
	{
		return (var & flag) == ZeroFlag<T>();
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto AreNoFlagsSet(const T var, const std::same_as<T> auto... flags) noexcept
	{
		return (IsNoFlagSet(var, flags) && ...);
	}
	template <EnumFlag T>
	[[nodiscard]] constexpr auto HasMultipleFlagsSet(const T var) noexcept
	{
		const auto underlyingValue = ToUnderlying(var);
		if (underlyingValue == 0)
		{
			return false;
		}

		return (underlyingValue & underlyingValue - 1) != 0;
	}

	template <Enumeration T>
	[[nodiscard]] constexpr auto FromUnderlying(const UnderlyingType<T> val) noexcept
	{
		return static_cast<T>(val);
	}

	template <EnumFlag T>
	constexpr auto SetFlag(T& var, const T flag) noexcept -> void
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
	constexpr auto ClearEnum(T& var) noexcept -> void
	{
		var = ZeroFlag<T>();
	}
}

export module PGUI.EnumFlag;

import std;

export namespace PGUI
{
	template<typename T>
	concept Enumeration = std::is_enum_v<T>;

	using Disabled = std::false_type;
	using Enabled = std::true_type;

	template <typename T>
	struct IsEnumFlagEnabled : public Disabled { };

	template <typename T>
	concept EnumFlag = Enumeration<T> && IsEnumFlagEnabled<T>::value;
	template <Enumeration T>
	using UnderlyingType = std::underlying_type_t<T>;

	template <
		EnumFlag EnumFlagType,
		typename UnderlyingType = UnderlyingType<EnumFlagType>>
		constexpr auto operator|(EnumFlagType lhs, EnumFlagType rhs) noexcept
	{
		return static_cast<EnumFlagType>(
			static_cast<UnderlyingType>(lhs) | static_cast<UnderlyingType>(rhs)
			);
	}
	constexpr auto& operator|=(EnumFlag auto& lhs, EnumFlag auto rhs) noexcept
	{
		lhs = lhs | rhs;
		return lhs;
	}
	template <
		EnumFlag EnumFlagType,
		typename UnderlyingType = UnderlyingType<EnumFlagType>>
		constexpr auto operator&(EnumFlagType lhs, EnumFlagType rhs) noexcept
	{
		return static_cast<EnumFlagType>(
			static_cast<UnderlyingType>(lhs) & static_cast<UnderlyingType>(rhs)
			);
	}
	constexpr auto& operator&=(EnumFlag auto& lhs, EnumFlag auto rhs) noexcept
	{
		lhs = lhs & rhs;
		return lhs;
	}
	template <
		EnumFlag EnumFlagType,
		typename UnderlyingType = UnderlyingType<EnumFlagType>>
		constexpr auto operator^(EnumFlagType lhs, EnumFlagType rhs) noexcept
	{
		return static_cast<EnumFlagType>(
			static_cast<UnderlyingType>(lhs) ^ static_cast<UnderlyingType>(rhs)
			);
	}
	constexpr auto& operator^=(EnumFlag auto& lhs, EnumFlag auto rhs) noexcept
	{
		lhs = lhs ^ rhs;
		return lhs;
	}
	template <
		EnumFlag EnumFlagType,
		typename UnderlyingType = UnderlyingType<EnumFlagType>>
		constexpr auto operator~(EnumFlagType lhs) noexcept
	{
		return static_cast<EnumFlagType>(
			~static_cast<UnderlyingType>(lhs));
	}
	template <
		EnumFlag EnumFlagType,
		typename UnderlyingType = UnderlyingType<EnumFlagType>>
		constexpr auto operator!(EnumFlagType lhs) -> bool
	{
		return !static_cast<UnderlyingType>(lhs);
	}
	template <EnumFlag EnumFlagType>
	auto IsFlagSet(EnumFlagType var, EnumFlagType flag)
	{
		return (var & flag) != static_cast<EnumFlagType>(0);
	}

	template <Enumeration Enum>
	auto ToUnderlying(Enum e) noexcept
	{
		return static_cast<UnderlyingType<Enum>>(e);
	}
}

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
	concept IsAnyOf = (std::same_as<T, Types> || ...);

	enum class PositioningMode
	{
		Absolute,
		Relative
	};
}

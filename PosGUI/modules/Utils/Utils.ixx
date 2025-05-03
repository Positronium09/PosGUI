module;
#include <concepts>


export module PGUI.Utils;

export import :WindowsUtils;
export import :StringUtils;
export import :ArithmeticUtils;


export namespace PGUI
{
	template <typename T, typename ...Types>
	concept IsAnyOf = (std::same_as<T, Types> || ...);

	enum class PositioningMode
	{
		Absolute,
		Relative
	};
}

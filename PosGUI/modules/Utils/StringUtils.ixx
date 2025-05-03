module;
#include <string>
#include <string_view>

export module PGUI.Utils:StringUtils;

export namespace PGUI
{
	[[nodiscard]] auto StringToWString(std::string_view string) noexcept -> std::wstring;
	[[nodiscard]] auto WStringToString(std::wstring_view string) noexcept -> std::string;
}

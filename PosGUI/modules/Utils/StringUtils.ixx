export module PGUI.Utils:StringUtils;

import std;

export namespace PGUI
{
	[[nodiscard]] auto StringToWString(std::string_view string) noexcept -> std::wstring;
	[[nodiscard]] auto WStringToString(std::wstring_view string) noexcept -> std::string;
}

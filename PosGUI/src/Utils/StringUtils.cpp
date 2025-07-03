module;
#include <Windows.h>

module PGUI.Utils:StringUtils;

import std;

namespace PGUI
{
	auto StringToWString(const std::string_view string) noexcept -> std::wstring
	{
		if (string.empty())
		{
			return std::wstring{ };
		}

		const auto size = MultiByteToWideChar(
			CP_UTF8, NULL, string.data(), static_cast<int>(string.size()), nullptr, NULL);
		std::wstring converted(size, L'\0');

		MultiByteToWideChar(CP_UTF8, 0, string.data(), static_cast<int>(string.size()), converted.data(), size);

		return converted;
	}

	auto WStringToString(const std::wstring_view string) noexcept -> std::string
	{
		if (string.empty())
		{
			return std::string{ };
		}

		const auto size = WideCharToMultiByte(
			CP_UTF8, 0, string.data(), static_cast<int>(string.size()), nullptr, 0, nullptr, nullptr);
		std::string converted(size, '\0');

		WideCharToMultiByte(
			CP_UTF8, 0, string.data(), static_cast<int>(string.size()), converted.data(), size, nullptr, nullptr);

		return converted;
	}
}

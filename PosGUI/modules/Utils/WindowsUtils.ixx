module;
#include <Windows.h>

export module PGUI.Utils:WindowsUtils;

import std;

export namespace PGUI
{
	[[nodiscard]] auto GetHInstance() -> HINSTANCE;

	[[nodiscard]] auto GetUserLocaleName() noexcept -> std::wstring;
	[[nodiscard]] auto GetCurrentInputMethodLanguage() -> std::wstring;

	[[nodiscard]] auto HresultFromWin32() noexcept -> HRESULT;
	[[nodiscard]] auto HresultFromWin32(DWORD errCode) noexcept -> HRESULT;

	auto MsgToText(UINT msg) noexcept -> std::wstring_view;

	void EnableDarkTitleBar(HWND hWnd, bool enable = true) noexcept;

	auto HResultToString(HRESULT hresult) noexcept -> std::wstring;
	auto WinErrToString(DWORD errorCode) noexcept -> std::wstring;

	template <typename T, std::floating_point U>
	[[nodiscard]] auto AdjustForDPI(T value, U dpi) noexcept -> T
	{
		return static_cast<T>(value * dpi / static_cast<U>(96.0));
	}
	template <typename T, std::floating_point U>
	[[nodiscard]] auto RemoveDPIAdjustment(T value, U dpi) noexcept -> T
	{
		return static_cast<T>(value * static_cast<U>(96.0) / dpi);
	}
}

module;
#include <Windows.h>

export module PGUI.Utils:WindowsUtils;

import std;

export
{
	// ReSharper disable CommentTypo
	// ReSharper disable IdentifierTypo
	// ReSharper disable CppInconsistentNaming
	// ReSharper disable CppEnforceTypeAliasCodeStyle

	constexpr auto WM_FULLSCREEN = 0x003A;
	constexpr auto WM_CLIENTSHUTDOWN = 0x003B;
	constexpr auto WM_DDEMLEVENT = 0x003C;
	constexpr auto WM_CALCSCROLL = 0x003F;
	// Handled by DefWindowProc
	constexpr auto WM_UAHDESTROYWINDOW = 0x0090;
	// lParam is UAHMENU
	constexpr auto WM_UAHDRAWMENU = 0x0091;
	// lParam is UAHDRAWMENUITEM
	constexpr auto WM_UAHDRAWMENUITEM = 0x0092;
	// Handled by DefWindowProc
	constexpr auto WM_UAHINITMENU = 0x0093;
	// lParam is UAHMEASUREMENUITEM
	constexpr auto WM_UAHMEASUREMENUITEM = 0x0094;
	// Handled by DefWindowProc
	constexpr auto WM_UAHNCPAINTMENUPOPUP = 0x0095;
	constexpr auto WM_UAHUPDATE = 0x0096;
	// Draw Caption mode.
	// wParam are DC_* flags.
	constexpr auto WM_NCUAHDRAWCAPTION = 0x00AE;
	// Draw Frame mode.
	// wParam is HDC, lParam are DC_ACTIVE and or DC_REDRAWHUNGWND.
	constexpr auto WM_NCUAHDRAWFRAME = 0x00AF;
	constexpr auto EM_MSGMAX = 0x00DA;
	constexpr auto WM_SYSTIMER = 0x0118;
	constexpr auto WM_GETISHELLBROWSER = (WM_USER + 7);

	constexpr auto SWP_STATECHANGED = 0x8000;
	constexpr auto SWP_NOCLIENTSIZE = 0x0800;
	constexpr auto SWP_NOCLIENTMOVE = 0x1000;

	// describes the sizes of the menu bar or menu item
	typedef union tagUAHMENUITEMMETRICS
	{
		// cx appears to be 14 / 0xE less than rcItem's width!
		// cy 0x14 seems stable, i wonder if it is 4 less than rcItem's height which is always 24 atm
		struct
		{
			DWORD cx;
			DWORD cy;
		} rgsizeBar[2];
		struct
		{
			DWORD cx;
			DWORD cy;
		} rgsizePopup[4];
	} UAHMENUITEMMETRICS;

	// not really used in our case but part of the other structures
	typedef struct tagUAHMENUPOPUPMETRICS
	{
		DWORD rgcx[4];
		DWORD fUpdateMaxWidths : 2; // from kernel symbols, padded to full dword
	} UAHMENUPOPUPMETRICS;

	// hmenu is the main window menu; hdc is the context to draw in
	typedef struct tagUAHMENU
	{
		HMENU hmenu;
		HDC hdc;
		DWORD dwFlags; // no idea what these mean, in my testing it's either 0x00000a00 or sometimes 0x00000a10
	} UAHMENU;

	// menu items are always referred to by iPosition here
	typedef struct tagUAHMENUITEM
	{
		INT iPosition; // 0-based position of menu item in menubar
		UAHMENUITEMMETRICS umim;
		UAHMENUPOPUPMETRICS umpm;
	} UAHMENUITEM;

	// the DRAWITEMSTRUCT contains the states of the menu items, as well as
	// the position index of the item in the menu, which is duplicated in
	// the UAHMENUITEM's iPosition as well
	typedef struct UAHDRAWMENUITEM
	{
		DRAWITEMSTRUCT dis; // itemID looks uninitialized
		UAHMENU um;
		UAHMENUITEM umi;
	} UAHDRAWMENUITEM;

	// the MEASUREITEMSTRUCT is intended to be filled with the size of the item
	// height appears to be ignored, but width can be modified
	typedef struct tagUAHMEASUREMENUITEM
	{
		MEASUREITEMSTRUCT mis;
		UAHMENU um;
		UAHMENUITEM umi;
	} UAHMEASUREMENUITEM;

	// ReSharper restore CommentTypo
	// ReSharper restore IdentifierTypo
	// ReSharper restore CppInconsistentNaming
	// ReSharper restore CppEnforceTypeAliasCodeStyle
}

export namespace PGUI
{
	[[nodiscard]] auto GetHInstance() -> HINSTANCE;

	[[nodiscard]] auto GetUserLocaleName() noexcept -> std::wstring;

	[[nodiscard]] auto GetCurrentInputMethodLanguage() -> std::wstring;

	[[nodiscard]] auto HresultFromWin32() noexcept -> HRESULT;

	[[nodiscard]] auto HresultFromWin32(DWORD errCode) noexcept -> HRESULT;

	auto WindowMsgToText(UINT msg) noexcept -> std::wstring_view;

	auto EnableDarkTitleBar(HWND hWnd, bool enable = true) noexcept -> void;

	auto HResultToString(HRESULT hresult) noexcept -> std::wstring;

	auto WinErrToString(DWORD errorCode) noexcept -> std::wstring;

	template <typename T, std::floating_point U>
	[[nodiscard]] auto AdjustForDpi(T value, U dpi) noexcept -> T
	{
		return static_cast<T>(value * dpi / static_cast<U>(96.0));
	}

	template <typename T, std::floating_point U>
	[[nodiscard]] auto RemoveDpiAdjustment(T value, U dpi) noexcept -> T
	{
		return static_cast<T>(value * static_cast<U>(96.0) / dpi);
	}
}

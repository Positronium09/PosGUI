module;
#include <Windows.h>
#include <string>
#include <memory>
#include <unordered_map>

module PGUI.WindowClass;

import PGUI.Utils;
import PGUI.Exceptions;


namespace PGUI
{
	extern auto _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

	template<typename ...Arg> auto static CreateWindowClassSharedPtr(Arg&&...arg) -> std::shared_ptr<WindowClass>
	{
		struct EnableMakeShared : public WindowClass
		{
			explicit(false) EnableMakeShared(Arg&&...arg) : WindowClass(arg...) { }
		};
		return std::make_shared<EnableMakeShared>(std::forward<Arg>(arg)...);
	}


	WindowClass::WindowClass(std::wstring_view _className, UINT style,
		HBRUSH backgroundBrush, HICON icon, HCURSOR cursor, HICON smIcon) :
		className(_className)
	{
		WNDCLASSEXW wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = style;
		wc.lpfnWndProc = _WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetHInstance();
		wc.hbrBackground = backgroundBrush;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = _className.data();
		wc.hIcon = icon;
		wc.hCursor = cursor ? cursor : static_cast<HCURSOR>(LoadImageW(nullptr, IDC_ARROW, IMAGE_CURSOR, NULL, NULL, LR_SHARED | LR_DEFAULTSIZE));
		wc.hIconSm = smIcon;

		classAtom = RegisterClassExW(&wc);

		if (!classAtom)
		{
			throw Win32Exception{ };
		}
	}

	auto WindowClass::Create(std::wstring_view className, UINT style,
		HBRUSH backgroundBrush, HICON icon, HCURSOR cursor, HICON smIcon) noexcept -> WindowClassPtr
	{
		WNDCLASSEXW w{ };
		auto atom = static_cast<ATOM>(GetClassInfoExW(GetHInstance(), className.data(), &w));

		if (!atom)
		{
			auto windowClass = CreateWindowClassSharedPtr(
				className, style, backgroundBrush,
				icon, cursor, smIcon);

			atom = windowClass->GetAtom();
			registeredClasses[atom] = windowClass;
		}

		return registeredClasses[atom];
	}

	WindowClass::~WindowClass() noexcept
	{
		UnregisterClassW(className.c_str(), GetHInstance());
	}
}

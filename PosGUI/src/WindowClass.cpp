module;
#include <Windows.h>

module PGUI.WindowClass;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

namespace PGUI
{
	// ReSharper disable once CppInconsistentNaming
	extern auto _WindowProc(HWND hWnd, MessageID msg, Argument1 arg1, Argument2 arg2) -> LRESULT;

	template <typename... Args>
	auto static CreateWindowClassSharedPtr(Args&&... args) -> std::shared_ptr<WindowClass>
	{
		struct EnableMakeShared : WindowClass
		{
			explicit(false) EnableMakeShared(Args&&... args) :
				WindowClass{ std::forward<Args>(args)... }
			{ }
		};
		return std::make_shared<EnableMakeShared>(std::forward<Args>(args)...);
	}

	auto WindowClass::Create(const std::wstring& className, UINT style,
	                         HBRUSH backgroundBrush, HICON icon, HCURSOR cursor, HICON smIcon) -> WindowClassPtr
	{
		WNDCLASSEXW w{ };
		auto atom = static_cast<ATOM>(GetClassInfoExW(GetHInstance(), className.c_str(), &w));

		std::scoped_lock lock{ mutex };

		if (atom != INVALID_ATOM && registeredClasses.contains(atom))
		{
			if (auto ptr = registeredClasses.at(atom).lock())
			{
				const auto areParametersSame =
						w.style != style ||
						w.hbrBackground != backgroundBrush ||
						w.hIcon != icon ||
						w.hIconSm != smIcon ||
						(cursor && w.hCursor != cursor);

				if (!areParametersSame)
				{
					Logger::Warning(
						Error{ ErrorCode::InvalidArgument }
						.AddDetail(L"Class name", className)
						.SetCustomMessage(L"Window class already registered with different parameters"));
				}

				return ptr;
			}
		}

		const auto windowClass = CreateWindowClassSharedPtr(
			className, style, backgroundBrush,
			icon, cursor, smIcon);

		atom = windowClass->GetAtom();
		registeredClasses[atom] = windowClass;

		return windowClass;
	}

	WindowClass::~WindowClass() noexcept
	{
		UnregisterClassW(className.c_str(), GetHInstance());
	}

	WindowClass::WindowClass(const std::wstring& className, const UINT style,
	                         const HBRUSH backgroundBrush, const HICON icon, const HCURSOR cursor, const HICON smIcon) :
		className{ className }
	{
		WNDCLASSEXW wc;
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = style;
		wc.lpfnWndProc = _WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(nullptr);
		wc.hInstance = GetHInstance();
		wc.hbrBackground = backgroundBrush;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className.c_str();
		wc.hIcon = icon;
		wc.hCursor = cursor
				? cursor
				: static_cast<HCURSOR>(LoadImageW(nullptr, IDC_ARROW, IMAGE_CURSOR, NULL, NULL,
				                                  LR_SHARED | LR_DEFAULTSIZE));
		wc.hIconSm = smIcon;

		classAtom = RegisterClassExW(&wc);

		if (!classAtom)
		{
			throw Exception{
				Error{ GetLastError() }
				.AddDetail(L"Class name", className)
				.AddDetail(L"Style", std::format(L"{:b}", style))
			};
		}
	}
}

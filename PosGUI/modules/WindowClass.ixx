module;
#include <Windows.h>

export module PGUI.WindowClass;

import PGUI.Mutex;
import PGUI.Utils;

import std;

export namespace PGUI
{
	class WindowClass;

	using WindowClassPtr = std::shared_ptr<WindowClass>;
	
	using WindowClassWeakPtr = std::weak_ptr<WindowClass>;

	class WindowClass : public std::enable_shared_from_this<WindowClass>
	{
		public:
		static auto Create(
			wzstring_view className,
			UINT style = CS_HREDRAW | CS_VREDRAW, HBRUSH backgroundBrush = nullptr,
			HICON icon = nullptr, HCURSOR cursor = nullptr, HICON smIcon = nullptr) -> WindowClassPtr;

		WindowClass(const WindowClass&) = delete;
		auto operator=(const WindowClass&) -> WindowClass& = delete;
		WindowClass(WindowClass&&) noexcept = delete;
		auto operator=(WindowClass&&) noexcept -> WindowClass& = delete;

		~WindowClass() noexcept;

		[[nodiscard]] auto ClassName() const noexcept -> wzstring_view { return className; }
		[[nodiscard]] auto GetAtom() const noexcept { return classAtom; }

		protected:
		WindowClass(wzstring_view className,
		            UINT style, HBRUSH backgroundBrush,
		            HICON icon, HCURSOR cursor, HICON smIcon);

		private:
		std::wstring className;
		ATOM classAtom{ INVALID_ATOM };

		static inline Mutex::CSMutex mutex;
		static inline std::unordered_map<ATOM, WindowClassWeakPtr> registeredClasses;
	};
}

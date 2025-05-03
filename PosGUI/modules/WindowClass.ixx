module;
#include <Windows.h>
#include <string>
#include <memory>
#include <unordered_map>

export module PGUI.WindowClass;


export namespace PGUI
{
	class WindowClass;

	using WindowClassPtr = std::shared_ptr<WindowClass>;
	
	class WindowClass
	{
		public:
		static auto Create(std::wstring_view _className,
			UINT style = CS_HREDRAW | CS_VREDRAW, HBRUSH backgroundBrush = nullptr,
			HICON icon = nullptr, HCURSOR cursor = nullptr, HICON smIcon = nullptr) noexcept -> WindowClassPtr;

		WindowClass(const WindowClass&) = delete;
		auto operator=(const WindowClass&)->WindowClass & = delete;
		WindowClass(WindowClass&&) noexcept = delete;
		auto operator=(WindowClass&&) noexcept -> WindowClass&& = delete;

		~WindowClass() noexcept;

		[[nodiscard]] auto ClassName() const noexcept -> std::wstring_view { return className; }
		[[nodiscard]] auto GetAtom() const noexcept { return classAtom; }

		protected:
		WindowClass(std::wstring_view _className,
			UINT style, HBRUSH backgroundBrush,
			HICON icon, HCURSOR cursor, HICON smIcon);

		private:
		std::wstring className;
		ATOM classAtom;

		static inline std::unordered_map<ATOM, WindowClassPtr> registeredClasses;
	};
}

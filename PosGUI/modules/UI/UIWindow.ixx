module;
#include <Windows.h>

export module PGUI.UI.UIWindow;

import std;

import PGUI.Shape2D;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;
import PGUI.UI.UIComponent;

export namespace PGUI::UI
{
	class UIWindow : public DirectXCompositionWindow
	{
		public:
		UIWindow() noexcept;
		explicit UIWindow(const WindowClassPtr& wndClass) noexcept;

		template <UIComponentType Type, typename ...Args>
		[[nodiscard]] auto AddComponent(Args&&... args) noexcept
		{
			auto component = std::make_shared<Type>(std::forward<Args>(args)...);

			if (!component)
			{
				throw std::runtime_error{ "component is null" };
			}

			components.push_back(component);
			return std::dynamic_pointer_cast<Type>(components.back());
		}

		private:
		std::vector<UIComponentPtr<>> components;
	};
}

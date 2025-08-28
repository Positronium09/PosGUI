export module PGUI.UI.UIBase:Interface;

import std;

export namespace PGUI::UI
{
	class UIWindow;
	class UIComponent;

	template <typename T>
	concept UIComponentType = std::derived_from<T, UIComponent>;

	template<UIComponentType T = UIComponent>
	using UIComponentPtr = std::shared_ptr<T>;
	template<UIComponentType T = UIComponent>
	using RawUIComponentPtr = T*;
	template<UIComponentType T = UIComponent>
	using CRawUIComponentPtr = const T*;

	template <typename T>
	concept UIWindowType = std::derived_from<T, UIWindow>;

	template <UIWindowType T = UIWindow>
	using UIWindowPtr = std::shared_ptr<T>;
	template <UIWindowType T = UIWindow>
	using RawUIWindowPtr = T*;
	template <UIWindowType T = UIWindow>
	using CRawUIWindowPtr = const T*;
}

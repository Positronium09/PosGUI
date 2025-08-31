export module PGUI.UI.UIBase:Interface;

import std;

export namespace PGUI::UI
{
	class UIContainer;
	class UIComponent;
	class UIWindow;

	template <typename T>
	concept UIContainerType = std::derived_from<T, UIContainer>;
	template<UIContainerType T = UIContainer>
	using UIContainerPtr = std::shared_ptr<T>;
	template<UIContainerType T = UIContainer>
	using RawUIContainerPtr = T*;
	template<UIContainerType T = UIContainer>
	using CRawUIContainerPtr = const T*;

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


	using ZIndex = int;
	namespace ZIndices
	{
		constexpr auto Background = -1000;
		constexpr auto Normal = 0;
		constexpr auto Elevated = 100;
		constexpr auto Floating = 500;
		constexpr auto Modal = 1000;
		constexpr auto Tooltip = 2000;
		constexpr auto Notification = 5000;
		constexpr auto Debug = 10000;
	}
}

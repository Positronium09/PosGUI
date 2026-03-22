export module PGUI.UI.UICore:Interface;

import std;

export namespace PGUI::UI
{
	class UIElement;

	template <typename T>
	concept UIElementType = std::derived_from<T, UIElement>;

	template <UIElementType T = UIElement>
	using UIElementPtr = std::unique_ptr<T>;

	template <UIElementType T = UIElement>
	using RawUIElementPtr = T*;

	class UIHost;

	template <typename T>
	concept UIHostType = std::derived_from<T, UIHost>;

	template <UIHostType T = UIHost>
	using UIHostPtr = std::unique_ptr<T>;

	template <UIHostType T = UIHost>
	using RawUIHostPtr = T*;

	class UIContainer;

	template <typename T>
	concept UIContainerType = std::derived_from<T, UIContainer>;

	template <UIContainerType T = UIContainer>
	using UIContainerPtr = std::unique_ptr<T>;

	template <UIContainerType T = UIContainer>
	using RawUIContainerPtr = T*;
}
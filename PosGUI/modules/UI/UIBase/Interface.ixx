export module PGUI.UI.UIBase:Interface;

import std;

export namespace PGUI::UI
{
	class UIElement;
	class UIContainer;

	template <typename T>
	concept UIElementType = std::derived_from<T, UIElement>;

	using UIElementPtr = std::shared_ptr<UIElement>;
	using RawUIElementPtr = UIElement*;
	using WeakUIElementPtr = std::weak_ptr<UIElement>;
	// ReSharper disable once CppInconsistentNaming

	using RawCUIElementPtr = const UIElement*;

	using UIContainerPtr = std::shared_ptr<UIContainer>;
	using RawUIContainerPtr = UIContainer*;
	using WeakUIContainerPtr = std::weak_ptr<UIContainer>;
	// ReSharper disable once CppInconsistentNaming

	using RawCUIContainerPtr = const UIContainer*;

	using ID = std::uint64_t;
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

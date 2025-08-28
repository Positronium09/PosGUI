module;
#include <Windows.h>

export module PGUI.UI.UIBase:UIWindow;

import std;

import :Interface;
import PGUI.Event;
import PGUI.Shape2D;
import PGUI.Window;
import PGUI.UI.Graphics;
import PGUI.ErrorHandling;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

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
				throw Exception{ ErrorCode::NullPointer, L"Failed to create UI component" };
			}

			return std::dynamic_pointer_cast<Type>(AddComponent(component));
		}
		auto AddComponent(const UIComponentPtr<>& component) noexcept -> UIComponentPtr<>;

		auto RemoveComponent(const UIComponentPtr<>& component) noexcept -> Result<UIComponentPtr<>>;
		auto RemoveComponentAtIndex(std::size_t index) noexcept -> Result<UIComponentPtr<>>;
		[[nodiscard]] auto GetComponentIndex(const UIComponentPtr<>& component) noexcept -> Result<std::size_t>;
		template <UIComponentType T = UIComponent>
		[[nodiscard]] auto GetComponentFromIndex(const std::size_t index) const noexcept -> Result<UIComponentPtr<>>
		{
			if (index >= components.size())
			{
				return Unexpected{
					Error{ ErrorCode::OutOfRange }
					.AddDetail(L"Index", std::to_wstring(index))
				};
			}

			if (auto component = std::dynamic_pointer_cast<T>(components[index]); component)
			{
				return component;
			}

			return Unexpected{
				Error{ ErrorCode::InvalidCast }
				.SetCustomMessage(L"Failed to cast UI component to the specified type")
				.AddDetail(L"Index", std::to_wstring(index))
			};
		}

		[[nodiscard]] auto GetComponentCount() const noexcept { return components.size(); }
		[[nodiscard]] const auto& GetAllComponents() const noexcept { return components; }

		private:
		struct ChildCallbackIds
		{
			CallbackId zIndexChanged = 0;
		};

		RawUIComponentPtr<> hovered;
		RawUIComponentPtr<> pressed;
		RawUIComponentPtr<> focused;

		bool needsZSorting = false;
		auto ChildZOrderChanged(int) noexcept -> void;
		auto EnsureZOrder() noexcept -> void;

		auto Draw(Graphics graphics) -> void override;

		auto OnMouseMove(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnLButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnLButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnRButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;
		auto OnRButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult;

		std::multimap<RawUIComponentPtr<>, ChildCallbackIds> childCallbacks;
		std::vector<UIComponentPtr<>> components;
	};
}

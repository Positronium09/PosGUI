module;
#include <Windows.h>

module PGUI.UI.UIBase:UIWindow;

import std;

import :UIComponent;
import PGUI.Window;
import PGUI.Event;
import PGUI.WindowClass;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIWindow::UIWindow() noexcept :
		UIWindow{ WindowClass::Create(L"PGUI_UIWindow") }
	{ }

	UIWindow::UIWindow(const WindowClassPtr& wndClass) noexcept :
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_MOUSEMOVE, &UIWindow::OnMouseMove);
		RegisterHandler(WM_LBUTTONDOWN, &UIWindow::OnLButtonDown);
		RegisterHandler(WM_LBUTTONUP, &UIWindow::OnLButtonUp);
		RegisterHandler(WM_RBUTTONDOWN, &UIWindow::OnRButtonDown);
		RegisterHandler(WM_RBUTTONUP, &UIWindow::OnRButtonUp);
	}

	auto UIWindow::AddComponent(const UIComponentPtr<>& component) noexcept -> UIComponentPtr<>
	{
		if (!component)
		{
			return nullptr;
		}

		const auto parent = component->parent;
		if (parent == this)
		{
			return nullptr;
		}
		if (parent)
		{
			if (const auto removedResult = parent->RemoveComponent(component);
				!removedResult.value())
			{
				Logger::Error(removedResult.error(), L"Cannot remove the component from the parent");
				return nullptr;
			}
		}

		const auto zIndexCallbackId = component->zIndexChanged.AddCallback(
			std::bind_front(&UIWindow::ChildZOrderChanged, this)
		);
		childCallbacks.emplace(
			component.get(),
			ChildCallbackIds{
				zIndexCallbackId
			}
		);

		needsZSorting = true;
		components.push_back(component);
		return components.back();
	}

	auto UIWindow::RemoveComponent(const UIComponentPtr<>& component) noexcept -> Result<UIComponentPtr<>>
	{
		if (!component)
		{
			return Unexpected{
				Error{ ErrorCode::NullPointer }.SetCustomMessage(L"Component pointer is null")
			};
		}
		const auto result = GetComponentIndex(component);
		if (result.has_value())
		{
			return RemoveComponentAtIndex(result.value());
		}

		return Unexpected{ result.error() };
	}

	auto UIWindow::RemoveComponentAtIndex(const std::size_t index) noexcept -> Result<UIComponentPtr<>>
	{
		if (index >= components.size())
		{
			return Unexpected{
				Error{ ErrorCode::OutOfRange }.SetCustomMessage(L"Index is out of range")
			};
		}

		auto component = components[index];
		components.erase(components.begin() + index);
		component->parent = nullptr;
		for (const auto [begin, end] = childCallbacks.equal_range(component.get());
		     const auto& [
			     zIndexChanged
		     ] : std::ranges::subrange{ begin, end } | std::views::values)
		{
			component->zIndexChanged.RemoveCallback(zIndexChanged);
		}
		return component;
	}

	auto UIWindow::GetComponentIndex(const UIComponentPtr<>& component) noexcept -> Result<std::size_t>
	{
		if (!component)
		{
			return Unexpected{
				Error{ ErrorCode::NullPointer }.SetCustomMessage(L"Component pointer is null")
			};
		}

		if (const auto it = std::ranges::find_if(
			components,
			[&component](const auto& ptr)
			{
				return ptr.get() == component.get();
			}); it != components.end())
		{
			return static_cast<std::size_t>(std::distance(components.begin(), it));
		}

		return Unexpected{
			Error{ ErrorCode::NotFound }.SetCustomMessage(L"Component not found in window")
		};
	}

	auto UIWindow::ChildZOrderChanged(int) noexcept -> void
	{
		needsZSorting = true;
	}

	auto UIWindow::EnsureZOrder() noexcept -> void
	{
		if (!needsZSorting)
		{
			return;
		}

		std::ranges::stable_sort(
			components,
			[](const auto& a, const auto& b)
			{
				return a->GetZIndex() < b->GetZIndex();
			});

		needsZSorting = false;
	}

	auto UIWindow::Draw(const Graphics graphics) -> void
	{
		DirectXCompositionWindow::Draw(graphics);
	}

	auto UIWindow::OnMouseMove(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		EnsureZOrder();

		for (const auto& component : components)
		{ }

		return 0;
	}

	auto UIWindow::OnLButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnLButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnRButtonDown(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}

	auto UIWindow::OnRButtonUp(UINT, WPARAM, LPARAM) noexcept -> MessageHandlerResult
	{
		return 0;
	}
}

module PGUI.UI.UICore:UIContainer;

import :Interface;
import :UIElement;
import :UIEvent;

import PGUI.UI.Graphics;
import PGUI.UI.D2D.D2DEnums;

import std;

namespace PGUI::UI
{
	UIContainer::UIContainer(const RectF& rect) noexcept :
		UIElement{ rect }
	{
		ChildAddedEvent().AddCallback(std::bind_front(&UIContainer::OnChildAddedEvent, this));
		ChildRemovedEvent().AddCallback(std::bind_front(&UIContainer::OnChildRemovedEvent, this));
	}

	auto UIContainer::RemoveChildElement(RawUIElementPtr<> element) -> Result<UIElementPtr<>>
	{
		const auto it = std::ranges::find_if(
			children,
			[element](const auto& child)
			{
				return child.get() == element;
			});

		if (it == children.end())
		{
			return Unexpected{ Error{ ErrorCode::NotFound } };
		}

		auto elementPtr = std::move(*it);
		children.erase(it);
		ChildRemovedEvent().Invoke(element);

		return elementPtr;
	}

	auto UIContainer::RemoveChildElement(const std::size_t index) -> Result<UIElementPtr<>>
	{
		if (index >= children.size())
		{
			return Unexpected{ Error{ ErrorCode::OutOfRange } };
		}

		return RemoveChildElement(children.at(index).get());
	}

	auto UIContainer::GetElementIndex(RawUIElementPtr<> element) const noexcept -> Result<std::size_t>
	{
		const auto it = std::ranges::find_if(
			children,
			[element](const auto& child)
			{
				return child.get() == element;
			});

		if (it == children.end())
		{
			return Unexpected{ Error{ ErrorCode::NotFound } };
		}

		return std::distance(children.begin(), it);
	}

	auto UIContainer::GetElementLinearIndex(const RawUIElementPtr<> element) const noexcept -> Result<std::size_t>
	{
		std::size_t offset = 0;

		for (const auto& child : children)
		{
			if (element == child.get())
			{
				return offset;
			}
			if (const auto container = dynamic_cast<UIContainer*>(child.get());
				container != nullptr)
			{
				if (const auto result = container->GetElementLinearIndex(element);
					result.has_value())
				{
					return offset + result.value();
				}
				offset += container->GetLinearElementCount();
			}
			offset++;
		}

		return Unexpected{ Error{ ErrorCode::NotFound } };
	}

	auto UIContainer::GetElementAtIndex(const std::size_t index) const noexcept -> Result<RawUIElementPtr<>>
	{
		if (index >= children.size())
		{
			return Unexpected{ Error{ ErrorCode::OutOfRange } };
		}

		return children.at(index).get();
	}

	auto UIContainer::GetLinearElementCount() const noexcept -> std::size_t
	{
		auto count = GetChildElementCount();

		const auto isContainer = [](const auto& element)
		{
			return dynamic_cast<UIContainer*>(element.get()) != nullptr;
		};
		const auto getContainer = [](const auto& element)
		{
			return dynamic_cast<UIContainer*>(element.get());
		};

		for (const auto& child : children
		                         | std::views::filter(isContainer)
		                         | std::views::transform(getContainer))
		{
			count += child->GetLinearElementCount();
		}

		return count;
	}

	auto UIContainer::GetElementAtLinearIndex(std::size_t index) const noexcept -> Result<RawUIElementPtr<>>
	{
		if (index >= GetLinearElementCount())
		{
			return Unexpected{ Error{ ErrorCode::OutOfRange } };
		}

		for (const auto& child : GetChildElements())
		{
			if (const auto container = dynamic_cast<UIContainer*>(child.get());
				container != nullptr)
			{
				const auto subCount = container->GetLinearElementCount();
				if (index < subCount)
				{
					return container->GetElementAtLinearIndex(index);
				}
				index -= subCount;

				continue;
			}

			if (index == 0)
			{
				return child.get();
			}

			index--;
		}

		return Unexpected{ Error{ ErrorCode::NotFound } };
	}

	auto UIContainer::GetElementAtPosition(const PointF point) const noexcept -> Result<RawUIElementPtr<>>
	{
		if (!GetRect().Contains(point))
		{
			return Unexpected{
				Error{ ErrorCode::InvalidArgument }
				.SetCustomMessage(L"Point is outside the container's bounds")
				.AddDetail(L"point", std::format(L"{}", point))
			};
		}

		const auto hitTestPoint = [point](const auto& element)
		{
			return element->GetRect().Contains(point);
		};

		for (const auto& child : children
		                         | std::views::reverse
		                         | std::views::filter(hitTestPoint))
		{
			if (const auto container = dynamic_cast<UIContainer*>(child.get());
				container != nullptr)
			{
				if (const auto result = container->GetElementAtPosition(point);
					result.has_value())
				{
					return result;
				}
			}
			else
			{
				return child.get();
			}
		}

		return Unexpected{
			Error{ ErrorCode::NotFound }
			.AddDetail(L"point", std::format(L"{}", point))
		};
	}

	auto UIContainer::IsChildElementVisible(const RawUIElementPtr<> element) const noexcept -> bool
	{
		return element->GetRect().Intersects(GetRect());
	}

	auto UIContainer::Render(const Graphics& graphics) noexcept -> void
	{
		EnsureZOrder();

		if (clipRendering)
		{
			graphics.PushAxisAlignedClip(GetRect(), D2D::AntiAliasingMode::PerPrimitive);
		}

		for (const auto& child : children
		                         | std::views::transform([](const auto& childElement) { return childElement.get(); })
		                         | std::views::filter(std::bind_front(&UIContainer::IsChildElementVisible, this)))
		{
			child->Render(graphics);
		}

		if (clipRendering)
		{
			graphics.PopAxisAlignedClip();
		}
	}

	auto UIContainer::HitTest(const PointF point) noexcept -> bool
	{
		EnsureZOrder();

		if (!UIElement::HitTest(point))
		{
			return false;
		}

		for (const auto& child : children | std::views::reverse)
		{
			if (child->HitTest(point))
			{
				return true;
			}
		}

		return false;
	}

	auto UIContainer::CreateDeviceResources() noexcept -> void
	{
		for (const auto& child : children)
		{
			child->CreateDeviceResources();
		}
	}

	auto UIContainer::DiscardDeviceResources() noexcept -> void
	{
		for (const auto& child : children)
		{
			child->DiscardDeviceResources();
		}
	}

	auto UIContainer::OnChildAddedEvent(RawUIElementPtr<> element) noexcept -> void
	{
		isZOrderDirty = true;

		const auto zIndexCallbackId = element->ZIndexEvent().AddObserver(
			std::bind_front(&UIContainer::OnChildZIndexChangedEvent, this, element)
		);
		const auto redrawRequestCallbackId = element->RedrawRequestedEvent().AddCallback(
			std::bind_front(&UIContainer::OnChildRedrawRequestedEvent, this)
		);

		element->host = GetHost();

		childAssociatedData.emplace(
			element,
			ChildAssociatedData
			{
				.zIndexCallbackId = zIndexCallbackId,
				.redrawRequestCallbackId = redrawRequestCallbackId
			});
	}

	auto UIContainer::OnChildRemovedEvent(const RawUIElementPtr<> element) noexcept -> void
	{
		if (childAssociatedData.contains(element)) [[likely]]
		{
			const auto& [zIndexChangedId, redrawRequestCallbackId] = childAssociatedData.at(element);
			element->ZIndexEvent().RemoveObserver(zIndexChangedId);
			element->RedrawRequestedEvent().RemoveCallback(redrawRequestCallbackId);
		}

		if (element->GetHost() == GetHost()) [[likely]]
		{
			element->host = nullptr;
		}

		childAssociatedData.erase(element);
	}

	auto UIContainer::OnChildZIndexChangedEvent(RawUIElementPtr<>, ZIndex) noexcept -> void
	{
		isZOrderDirty = true;
	}

	auto UIContainer::OnChildRedrawRequestedEvent(const RawUIElementPtr<> element) noexcept -> void
	{
		if (IsChildElementVisible(element))
		{
			RequestRedraw();
		}
	}

	auto UIContainer::EnsureZOrder() noexcept -> void
	{
		if (!isZOrderDirty)
		{
			return;
		}

		const auto zIndexComparator = [](const auto& lhs, const auto& rhs)
		{
			return lhs->GetZIndex() < rhs->GetZIndex();
		};

		std::ranges::stable_sort(children, zIndexComparator);

		isZOrderDirty = false;
	}
}

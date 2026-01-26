module PGUI.UI.UIBase:UIContainer;

import std;

import :UIContainer;
import :UIElement;
import PGUI.UI.D2D.D2DEnums;
import PGUI.ErrorHandling;
import PGUI.UI.Graphics;

namespace PGUI::UI
{
	auto UIContainer::AddElement(const UIElementPtr& ptr) noexcept -> void
	{
		ptr->SetParent(this);
		elements.push_back(ptr);

		if (layoutPanel)
		{
			layoutPanel->AddItem(*ptr);
		}

		zOrderDirty = true;
	}

	auto UIContainer::GetElementAtIndex(const std::size_t index) const noexcept -> Result<UIElementPtr>
	{
		if (index >= elements.size())
		{
			return Unexpected{
				Error{
					ErrorCode::OutOfRange
				}.AddDetail(L"Index", std::to_wstring(index))
			};
		}

		return elements.at(index);
	}

	auto UIContainer::GetElementAtIndex(const std::size_t index) noexcept -> Result<UIElementPtr>
	{
		if (index >= elements.size())
		{
			return Unexpected{
				Error{
					ErrorCode::OutOfRange
				}.AddDetail(L"Index", std::to_wstring(index))
			};
		}

		return elements.at(index);
	}

	auto UIContainer::RemoveElement(const std::size_t index) noexcept -> Result<UIElementPtr>
	{
		if (index >= elements.size())
		{
			return Unexpected{
				Error{
					ErrorCode::OutOfRange
				}.AddDetail(L"Index", std::to_wstring(index))
			};
		}

		const auto element = elements.at(index);
		elements.erase(std::next(elements.begin(), index));
		element->SetParent(nullptr);

		return element;
	}

	auto UIContainer::RemoveElement(const RawUIElementPtr ptr) noexcept -> Result<UIElementPtr>
	{
		const auto indexResult = GetElementIndex(ptr);
		if (indexResult.has_value())
		{
			return RemoveElement(*indexResult);
		}
		return Unexpected{ indexResult.error() };
	}

	auto UIContainer::GetElementIndex(RawCUIElementPtr ptr) const noexcept -> Result<std::size_t>
	{
		const auto found = std::ranges::find_if(
			elements,
			[&ptr](const auto& elem)
			{
				return elem.get() == ptr;
			});

		if (found == elements.end())
		{
			return Unexpected{
				Error{
					ErrorCode::NotFound
				}.AddDetail(L"Element", std::to_wstring(std::bit_cast<std::uintptr_t>(ptr)))
			};
		}
		return std::distance(elements.begin(), found);
	}

	auto UIContainer::GetElementAtPosition(const PointF point) const noexcept -> Result<RawCUIElementPtr>
	{
		if (!rect.Contains(point))
		{
			return Unexpected{
				Error{
					ErrorCode::InvalidArgument
				}.AddDetail(L"Point", std::format(L"{}", point))
			};
		}

		for (const auto& element : elements | std::views::reverse)
		{
			if (!*element->IsEnabled())
			{
				continue;
			}
			if (element->HitTest(point))
			{
				return element.get();
			}
		}

		return Unexpected{
			Error{
				ErrorCode::NotFound
			}.AddDetail(L"Point", std::format(L"{}", point))
		};
	}

	auto UIContainer::GetElementAtPosition(const PointF point) noexcept -> Result<RawUIElementPtr>
	{
		if (!rect.Contains(point))
		{
			return Unexpected{
				Error{
					ErrorCode::InvalidArgument
				}.AddDetail(L"Point", std::format(L"{}", point))
			};
		}

		for (const auto& element : elements | std::views::reverse)
		{
			if (!*element->IsEnabled())
			{
				continue;
			}
			if (element->HitTest(point))
			{
				if (const auto container = std::dynamic_pointer_cast<UIContainer>(element))
				{
					return container->GetElementAtPosition(point);
				}
				return element.get();
			}
		}

		return Unexpected{
			Error{
				ErrorCode::NotFound
			}.AddDetail(L"Point", std::format(L"{}", point))
		};
	}

	auto UIContainer::GetLinearElementCount() const noexcept -> std::size_t
	{
		std::size_t count = 0;

		for (const auto& containerElement : GetElements())
		{
			if (const auto asContainer = dynamic_cast<RawUIContainerPtr>(containerElement.get());
				asContainer != nullptr)
			{
				count += asContainer->GetLinearElementCount();
				continue;
			}
			count++;
		}

		return count;
	}

	auto UIContainer::GetElementAtLinearIndex(std::size_t index) const noexcept -> Result<RawUIElementPtr>
	{
		if (index >= GetLinearElementCount())
		{
			return Unexpected{ Error{ ErrorCode::OutOfRange } };
		}

		for (const auto& containerElement : GetElements())
		{
			if (const auto asContainer = std::dynamic_pointer_cast<UIContainer>(containerElement);
				asContainer != nullptr)
			{
				const auto subCount = asContainer->GetLinearElementCount();
				if (index < subCount)
				{
					return asContainer->GetElementAtLinearIndex(index);
				}
				index -= subCount;
				continue;
			}

			if (index == 0)
			{
				return containerElement.get();
			}

			index--;
		}

		return Unexpected{ Error{ ErrorCode::NotFound } };
	}

	auto UIContainer::GetElementLinearIndex(const RawCUIElementPtr ptr) const noexcept -> Result<std::size_t>
	{
		std::size_t offset = 0;
		for (const auto& [index, containerElement] : GetElements() | std::views::enumerate)
		{
			if (containerElement.get() == ptr)
			{
				return offset;
			}
			if (const auto asContainer = dynamic_cast<RawUIContainerPtr>(containerElement.get());
				asContainer != nullptr)
			{
				if (const auto subIndexResult = asContainer->GetElementLinearIndex(ptr);
					subIndexResult.has_value())
				{
					return offset + subIndexResult.value();
				}
				offset += asContainer->GetElementCount();
			}
			offset++;
		}

		return Unexpected{ Error{ ErrorCode::NotFound } };
	}

	auto UIContainer::HandleEvent(UIEvent& event) -> void
	{
		if (event.type == EventType::RectChanged && layoutPanel)
		{
			layoutPanel->MoveAndResize(GetRect());
		}
		else if (event.type == EventType::SizeChanged && layoutPanel)
		{
			layoutPanel->Resize(GetSize());
		}
	}

	auto UIContainer::EnsureZOrder() noexcept -> void
	{
		if (!zOrderDirty)
		{
			return;
		}

		std::ranges::stable_sort(
			elements,
			[](const auto& a, const auto& b)
			{
				return *a->GetZIndex() < *b->GetZIndex();
			});

		zOrderDirty = false;
	}

	auto UIContainer::HitTest(const PointF point) noexcept -> bool
	{
		EnsureZOrder();

		if (!rect.Contains(point))
		{
			return false;
		}

		for (const auto& element : elements | std::views::reverse)
		{
			if (!*element->IsEnabled())
			{
				continue;
			}

			if (element->HitTest(point))
			{
				return true;
			}
		}

		return false;
	}

	auto UIContainer::Render(const Graphics& graphics) -> void
	{
		EnsureZOrder();

		if (cropRendering)
		{
			graphics.PushAxisAlignedClip(GetRect(), D2D::AntiAliasingMode::PerPrimitive);
		}

		for (const auto& element : elements)
		{
			if (!*element->IsEnabled())
			{
				continue;
			}

			if (const auto rc = GetRect();
				!rc.Intersects(element->GetRect()))
			{
				continue;
			}

			element->Render(graphics);
		}

		if (cropRendering)
		{
			graphics.PopAxisAlignedClip();
		}
	}

	auto UIContainer::CreateDeviceResources() -> void
	{
		for (const auto& element : elements)
		{
			element->CreateDeviceResources();
		}
	}

	auto UIContainer::DiscardDeviceResources() -> void
	{
		for (const auto& element : elements)
		{
			element->DiscardDeviceResources();
		}
	}
}

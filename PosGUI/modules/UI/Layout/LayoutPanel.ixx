module;
#include <Windows.h>

export module PGUI.UI.Layout.LayoutPanel;

import std;

import PGUI.Window;
import PGUI.Shape2D;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.UIComponent;

export namespace PGUI::UI::Layout
{
	class LayoutPanel;

	using LayoutItem = std::variant<RawWindowPtr<>, RawPtr<LayoutPanel>>;

	class LayoutPanel
	{
		public:
		explicit LayoutPanel(const RectF bounds) noexcept :
			bounds{ bounds }
		{
		}
		virtual ~LayoutPanel() noexcept = default;

		virtual auto RearrangeItems() noexcept -> void = 0;

		virtual auto AddItem(const LayoutItem& item) noexcept -> void
		{
			managedItems.push_back(item);
			OnItemAdded(item);
		}
		auto AddItem(const WindowPtr<>& wnd) -> void
		{
			AddItem(wnd.get());
		}
		auto AddItem(const RawWindowPtr<>& wnd)
		{
			AddItem(LayoutItem{ wnd });
		}
		auto AddItem(LayoutPanel& panel)
		{
			AddItem(&panel);
		}
		virtual auto RemoveItem(const std::size_t index) -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}

			managedItems.erase(managedItems.begin() + index);
			OnItemRemoved(index);

			return Error{ ErrorCode::Success };
		}
		auto RemoveItem(const HWND hwnd) noexcept
		{
			if (const auto it = std::ranges::find_if(managedItems,
				[&hwnd](const LayoutItem& item)
			{
				if (std::holds_alternative<RawWindowPtr<>>(item))
				{
					return std::get<RawWindowPtr<>>(item)->Hwnd() == hwnd;
				}
				return false;
			});
			it != managedItems.end())
			{
				managedItems.erase(it);
				OnItemRemoved(std::distance(managedItems.begin(), it));
				return Error{ ErrorCode::Success };
			}
			return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given HWND not found in managed items");
		}
		auto RemoveItem(const RawWindowPtr<> wnd) noexcept
		{
			return RemoveItem(wnd->Hwnd());
		}
		[[nodiscard]] const auto& GetItems() const noexcept
		{
			return managedItems;
		}
		[[nodiscard]] auto GetItem(const std::size_t& index) const noexcept -> Result<LayoutItem>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{
					Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range")
				};
			}
			return managedItems.at(index);
		}
		[[nodiscard]] auto GetItem(const HWND hwnd) const noexcept -> Result<LayoutItem>
		{
			if (const auto it = std::ranges::find_if(managedItems,
				[&hwnd](const LayoutItem& item)
			{
				if (std::holds_alternative<RawWindowPtr<>>(item))
				{
					return std::get<RawWindowPtr<>>(item)->Hwnd() == hwnd;
				}
				return false;
			});
			it != managedItems.end())
			{
				return *it;
			}
			return Unexpected{
				Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given HWND not found in managed items")
			};
		}
		[[nodiscard]] auto GetItem(const RawWindowPtr<> wnd) const noexcept
		{
			return GetItem(wnd->Hwnd());
		}
		[[nodiscard]] auto GetItem(const LayoutPanel& panel) const noexcept -> Result<LayoutItem>
		{
			if (const auto it = std::ranges::find_if(managedItems,
				[&panel](const LayoutItem& item)
			{
				if (std::holds_alternative<RawPtr<LayoutPanel>>(item))
				{
					return std::get<RawPtr<LayoutPanel>>(item) == &panel;
				}
				return false;
			});
			it != managedItems.end())
			{
				return *it;
			}

			return Unexpected{
				Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given panel not found in managed items")
			};
		}

		[[nodiscard]] auto GetItemCount() const noexcept
		{
			return managedItems.size();
		}

		auto GetTotalItemSize() const noexcept
		{
			SizeF totalSize;
			for (const auto& item : GetItems())
			{
				const auto size = MeasureItem(item);
				totalSize += size;
			}

			return totalSize;
		}
		auto GetItemSizeUpToIndex(const std::size_t index) const noexcept
		{
			if (index >= GetItems().size())
			{
				return SizeF{ 0, 0 };
			}

			SizeF totalSize;
			for (const auto& item : GetItems() | std::views::take(index + 1))
			{
				const auto size = MeasureItem(item);
				totalSize += size;
			}
			return totalSize;
		}
		auto GetItemSizeBetweenIndices(
			const std::size_t startIndex, const std::size_t endIndex) const noexcept
		{
			const auto end = std::clamp(endIndex, 0ULL, GetItems().size() - 1);

			if (startIndex >= GetItems().size() || startIndex > end)
			{
				return SizeF{ 0, 0 };
			}

			SizeF totalSize;

			for (const auto& item : GetItems() |
				std::views::drop(startIndex) | std::views::take(end - startIndex + 1))
			{
				const auto size = MeasureItem(item);
				totalSize += size;
			}
			return totalSize;
		}

		auto SetBounds(const RectF newBounds) noexcept
		{
			bounds = newBounds;
			RearrangeItems();
		}
		[[nodiscard]] auto GetBounds() const noexcept
		{
			return bounds;
		}
		[[nodiscard]] auto GetBoundsSize() const noexcept
		{
			return bounds.Size();
		}
		static auto MeasureItem(const LayoutItem& item) noexcept -> SizeF
		{
			if (std::holds_alternative<RawWindowPtr<>>(item))
			{
				return std::get<RawWindowPtr<>>(item)->GetClientSize();
			}
			if (std::holds_alternative<RawPtr<LayoutPanel>>(item))
			{
				return std::get<RawPtr<LayoutPanel>>(item)->GetBoundsSize();
			}
			std::unreachable();
		}
		auto MeasureItem(const std::size_t index) const noexcept -> SizeF
		{
			if (index < managedItems.size())
			{
				return MeasureItem(managedItems.at(index));
			}

			return SizeF{ 0, 0 };
		}

		auto Move(const PointF point) noexcept
		{
			auto size = bounds.Size();
			bounds = { point, size };
			RearrangeItems();
		}
		auto Resize(const SizeF size) noexcept
		{
			auto topLeft = bounds.TopLeft();
			bounds = { topLeft, size };
			RearrangeItems();
		}

		protected:
		auto GetItemIndex(const LayoutItem& item) const noexcept -> Result<std::size_t>
		{
			if (const auto it = std::ranges::find_if(
				GetItems(),
				[&item](const auto& other)
			{
				if (item.index() != other.index())
				{
					return false;
				}
				if (std::holds_alternative<RawWindowPtr<>>(item))
				{
					return std::get<RawWindowPtr<>>(item)->Hwnd() == std::get<RawWindowPtr<>>(other)->Hwnd();
				}
				if (std::holds_alternative<RawPtr<LayoutPanel>>(item))
				{
					return std::get<RawPtr<LayoutPanel>>(item) == std::get<RawPtr<LayoutPanel>>(other);
				}
				std::unreachable();
			}); it != GetItems().end())
			{
				return std::distance(GetItems().begin(), it);
			}

			return Unexpected{
				Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given item not found in managed items")
			};
		}
		auto ArrangeItem(const LayoutItem& item, const RectF assignedBounds) const noexcept -> void
		{
			auto converted = assignedBounds;
			converted.Shift(bounds.TopLeft());
			if (std::holds_alternative<RawWindowPtr<>>(item))
			{
				std::get<RawWindowPtr<>>(item)->MoveAndResize(converted);
			}
			else if (std::holds_alternative<RawPtr<LayoutPanel>>(item))
			{
				std::get<RawPtr<LayoutPanel>>(item)->SetBounds(converted);
			}
		}
		auto MoveItem(const LayoutItem& item, const PointF point) const noexcept -> void
		{
			auto converted = point;
			converted.Shift(bounds.TopLeft());
			if (std::holds_alternative<RawWindowPtr<>>(item))
			{
				std::get<RawWindowPtr<>>(item)->Move(converted);
			}
			else if (std::holds_alternative<RawPtr<LayoutPanel>>(item))
			{
				const auto currentBounds = std::get<RawPtr<LayoutPanel>>(item)->GetBounds();
				const RectF assignedBounds{ converted, currentBounds.Size() };
				std::get<RawPtr<LayoutPanel>>(item)->SetBounds(assignedBounds);
			}
		}
		static auto ResizeItem(const LayoutItem& item, const SizeF size) noexcept -> void
		{
			if (std::holds_alternative<RawWindowPtr<>>(item))
			{
				std::get<RawWindowPtr<>>(item)->Resize(size);
			}
			else if (std::holds_alternative<RawPtr<LayoutPanel>>(item))
			{
				const auto currentBounds = std::get<RawPtr<LayoutPanel>>(item)->GetBounds();
				const RectF assignedBounds{ currentBounds.TopLeft(), size };
				std::get<RawPtr<LayoutPanel>>(item)->SetBounds(assignedBounds);
			}
		}
		auto ArrangeItem(const std::size_t index, const RectF assignedBounds) const noexcept -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}
			ArrangeItem(managedItems.at(index), assignedBounds);
			return Error{ ErrorCode::Success };
		}
		auto MoveItem(const std::size_t index, const PointF point) const noexcept -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}
			MoveItem(managedItems.at(index), point);
			return Error{ ErrorCode::Success };
		}
		auto ResizeItem(const std::size_t index, const SizeF size) const noexcept -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}
			ResizeItem(managedItems.at(index), size);
			return Error{ ErrorCode::Success };
		}

		virtual auto OnItemAdded(const LayoutItem&) -> void
		{
			RearrangeItems();
		}
		virtual auto OnItemRemoved(const std::size_t) -> void
		{
			RearrangeItems();
		}

		private:
		std::vector<LayoutItem> managedItems;
		RectF bounds;
	};
}

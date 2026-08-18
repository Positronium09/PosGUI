module;
#include <Windows.h>

export module PGUI.UI.Layout.LayoutPanel;

import std;

import PGUI.Window;
import PGUI.Shape;
import PGUI.Utils;
import PGUI.ErrorHandling;

namespace PGUI::UI::Layout::Detail
{
	template <typename T>
	concept LayoutItemLike = requires (T & t)
	{
		t.MoveAndResize(RectF{ });
		t.MoveAndResize(PointF{ }, SizeF{ });
		t.Move(PointF{ });
		t.Resize(SizeF{ });
		{ t.GetRect() } -> std::same_as<RectF>;
		{ t.GetSize() } -> std::same_as<SizeF>;
		{ t.GetPosition() } -> std::same_as<PointF>;
	};;
}

export namespace PGUI::UI::Layout
{
	class LayoutItem
	{
		struct VTable
		{
			void (*moveAndResize)(void*, RectF) noexcept;
			void (*moveAndResizeSplit)(void*, PointF, SizeF) noexcept;
			void (*move)(void*, PointF) noexcept;
			void (*resize)(void*, SizeF) noexcept;
			RectF (*getRect)(const void*) noexcept;
			SizeF (*getSize)(const void*) noexcept;
			PointF (*getPosition)(const void*) noexcept;
		};

		template <typename T>
		static constexpr VTable vtableFor{
			[](void* obj, const RectF rect) noexcept { static_cast<T*>(obj)->MoveAndResize(rect); },
			[](void* obj, const PointF point, const SizeF size) noexcept { static_cast<T*>(obj)->MoveAndResize(point, size); },
			[](void* obj, const PointF point) noexcept { static_cast<T*>(obj)->Move(point); },
			[](void* obj, const SizeF size) noexcept { static_cast<T*>(obj)->Resize(size); },
			[](const void* obj) noexcept -> RectF { return static_cast<const T*>(obj)->GetRect(); },
			[](const void* obj) noexcept -> SizeF { return static_cast<const T*>(obj)->GetSize(); },
			[](const void* obj) noexcept -> PointF { return static_cast<const T*>(obj)->GetPosition(); }
		};

		public:
		template <Detail::LayoutItemLike T>
		explicit LayoutItem(T& item) noexcept :
			obj{ std::addressof(item) },
			vtable{ &vtableFor<T> }
		{
		}

		auto MoveAndResize(const RectF rect) const noexcept -> void
		{
			vtable->moveAndResize(obj, rect);
		}
		auto MoveAndResize(const PointF point, const SizeF size) const noexcept -> void
		{
			vtable->moveAndResizeSplit(obj, point, size);
		}
		auto Move(const PointF point) const noexcept -> void
		{
			vtable->move(obj, point);
		}
		auto Resize(const SizeF size) const noexcept -> void
		{
			vtable->resize(obj, size);
		}
		[[nodiscard]] auto GetRect() const noexcept -> RectF
		{
			return vtable->getRect(obj);
		}
		[[nodiscard]] auto GetSize() const noexcept -> SizeF
		{
			return vtable->getSize(obj);
		}
		[[nodiscard]] auto GetPosition() const noexcept -> PointF
		{
			return vtable->getPosition(obj);
		}

		[[nodiscard]] auto operator==(const LayoutItem& other) const noexcept -> bool
		{
			return obj == other.obj;
		}

		private:
		void* obj;
		const VTable* vtable;
	};

	[[nodiscard]] auto MakeLayoutItem(Detail::LayoutItemLike auto& item) noexcept -> LayoutItem
	{
		return LayoutItem{ item };
	}

	class LayoutPanel
	{
		public:
		explicit LayoutPanel(const RectF bounds) noexcept :
			rect{ bounds }
		{
		}
		virtual ~LayoutPanel() noexcept = default;

		virtual auto RearrangeItems() noexcept -> void = 0;

		template <typename T> requires Detail::LayoutItemLike<T>
		auto AddItem(T& item) noexcept -> void
		{
			managedItems.push_back(MakeLayoutItem(item));
			OnItemAdded(managedItems.back());
		}

		[[nodiscard]] auto RemoveItem(const std::size_t index) -> Result<void>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{ Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range") };
			}

			managedItems.erase(managedItems.begin() + index);
			OnItemRemoved(index);

			return EmptyResult;
		}
		template <typename Self>
		[[nodiscard]] auto&& GetItems(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.managedItems);
		}
		[[nodiscard]] auto GetItem(const std::size_t index) const noexcept -> Result<std::reference_wrapper<const LayoutItem>>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{
					Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range")
				};
			}

			return managedItems.at(index);
		}
		[[nodiscard]] auto GetItem(const std::size_t index) noexcept -> Result<std::reference_wrapper<LayoutItem>>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{
					Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range")
				};
			}

			return managedItems.at(index);
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
			if (index >= GetItemCount())
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
			if (GetItemCount() == 0)
			{
				return SizeF{ 0, 0 };
			}
			const auto end = std::clamp(endIndex, 0ULL, GetItemCount() - 1);

			if (startIndex >= GetItemCount() || startIndex > end)
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

		static auto MeasureItem(const LayoutItem& item) noexcept -> SizeF
		{
			return item.GetSize();
		}
		auto MeasureItem(const std::size_t index) const noexcept -> SizeF
		{
			if (index < managedItems.size())
			{
				return MeasureItem(managedItems.at(index));
			}

			return SizeF{ 0, 0 };
		}
		auto MoveAndResize(const RectF newRect) noexcept
		{
			rect = newRect;
			RearrangeItems();
		}
		auto MoveAndResize(const PointF point, const SizeF size) noexcept
		{
			rect = { point, size };
			RearrangeItems();
		}
		auto Move(const PointF point) noexcept
		{
			rect.Move(point);
			RearrangeItems();
		}
		auto Resize(const SizeF size) noexcept
		{
			rect.Resize(size);
			RearrangeItems();
		}
		[[nodiscard]] auto GetRect() const noexcept -> RectF
		{
			return rect;
		}
		[[nodiscard]] auto GetSize() const noexcept -> SizeF
		{
			return rect.Size();
		}
		[[nodiscard]] auto GetPosition() const noexcept -> PointF
		{
			return rect.TopLeft();
		}

		protected:
		auto GetItemIndex(const LayoutItem& item) const noexcept -> Result<std::size_t>
		{
			if (const auto it = std::ranges::find_if(
				GetItems(),
				[&item](const auto& other)
			{
				return item == other;
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
			converted.Shift(rect.TopLeft());
			item.MoveAndResize(converted);
		}
		auto MoveItem(const LayoutItem& item, const PointF point) const noexcept -> void
		{
			auto converted = point;
			converted.Shift(rect.TopLeft());
			item.Move(converted);
		}
		static auto ResizeItem(const LayoutItem& item, const SizeF size) noexcept -> void
		{
			item.Resize(size);
		}
		[[nodiscard]] auto ArrangeItem(const std::size_t index, const RectF assignedBounds) const noexcept -> Result<void>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{ Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range") };
			}
			ArrangeItem(managedItems.at(index), assignedBounds);
			return EmptyResult;
		}
		[[nodiscard]] auto MoveItem(const std::size_t index, const PointF point) const noexcept -> Result<void>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{ Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range") };
			}
			MoveItem(managedItems.at(index), point);
			return EmptyResult;
		}
		[[nodiscard]] auto ResizeItem(const std::size_t index, const SizeF size) const noexcept -> Result<void>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{ Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range") };
			}
			ResizeItem(managedItems.at(index), size);
			return EmptyResult;
		}

		virtual auto OnItemAdded(const LayoutItem&) -> void
		{
			RearrangeItems();
		}
		virtual auto OnItemRemoved(const std::size_t) -> void
		{
			RearrangeItems();
		}

		auto operator==(const LayoutPanel& other) const noexcept -> bool
		{
			return this == &other;
		}

		private:
		std::vector<LayoutItem> managedItems;
		RectF rect;
	};
}

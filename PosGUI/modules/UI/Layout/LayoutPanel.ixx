module;
#include <Windows.h>

#undef GetObject

export module PGUI.UI.Layout.LayoutPanel;

import std;

import PGUI.Window;
import PGUI.Shape2D;
import PGUI.Utils;
import PGUI.ErrorHandling;
import TypeErasure;

namespace PGUI::UI::Layout::Detail
{
	struct LayoutItemFeature
	{
		template <typename T>
		struct Validator
		{
			static constexpr auto value =
				requires (T& t)
			{
				t.MoveAndResize(RectF{ });
				t.MoveAndResize(PointF{ }, SizeF{ });
				t.Move(PointF{ });
				t.Resize(SizeF{ });
				{ t.GetRect() } -> std::same_as<RectF>;
				{ t.GetSize() } -> std::same_as<SizeF>;
				{ t.GetPosition() } -> std::same_as<PointF>;
			};
		};

		template <typename V>
		struct VTable : virtual V
		{
			virtual auto MoveAndResize(RectF) noexcept -> void = 0;
			virtual auto MoveAndResize(PointF, SizeF) noexcept -> void = 0;
			virtual auto Move(PointF) noexcept -> void = 0;
			virtual auto Resize(SizeF) noexcept -> void = 0;
			virtual auto GetRect() const noexcept -> RectF = 0;
			virtual auto GetSize() const noexcept -> SizeF = 0;
			virtual auto GetPosition() const noexcept -> PointF = 0;
		};

		template <typename M>
		struct Model : M
		{
			using M::M;
			auto MoveAndResize(const RectF rect) noexcept -> void override
			{
				this->GetObject().MoveAndResize(rect);
			}
			auto MoveAndResize(const PointF point, const SizeF size) noexcept -> void override
			{
				this->GetObject().MoveAndResize(point, size);
			}
			auto Move(const PointF point) noexcept -> void override
			{
				this->GetObject().Move(point);
			}
			auto Resize(const SizeF size) noexcept -> void override
			{
				this->GetObject().Resize(size);
			}
			auto GetRect() const noexcept -> RectF override
			{
				return this->GetObject().GetRect();
			}
			auto GetSize() const noexcept -> SizeF override
			{
				return this->GetObject().GetSize();
			}
			auto GetPosition() const noexcept -> PointF override
			{
				return this->GetObject().GetPosition();
			}
		};

		template <typename I>
		struct Interface : I
		{
			auto MoveAndResize(const RectF rect) noexcept -> void
			{
				dynamic_cast<VTable<TypeErasure::VTableBase>*>(this->GetVTable())->MoveAndResize(rect);
			}
			auto MoveAndResize(const PointF point, const SizeF size) noexcept -> void
			{
				dynamic_cast<VTable<TypeErasure::VTableBase>*>(this->GetVTable())->MoveAndResize(point, size);
			}
			auto Move(const PointF point) noexcept -> void
			{
				dynamic_cast<VTable<TypeErasure::VTableBase>*>(this->GetVTable())->Move(point);
			}
			auto Resize(const SizeF size) noexcept -> void
			{
				dynamic_cast<VTable<TypeErasure::VTableBase>*>(this->GetVTable())->Resize(size);
			}
			auto GetRect() const noexcept -> RectF
			{
				return dynamic_cast<const VTable<TypeErasure::VTableBase>*>(this->GetVTable())->GetRect();
			}
			auto GetSize() const noexcept -> SizeF
			{
				return dynamic_cast<const VTable<TypeErasure::VTableBase>*>(this->GetVTable())->GetSize();
			}
			auto GetPosition() const noexcept -> PointF
			{
				return dynamic_cast<const VTable<TypeErasure::VTableBase>*>(this->GetVTable())->GetPosition();
			}
			auto operator==(const Interface& other) const noexcept
			{
				return this->GetVTable() == other.GetVTable();
			}
		};
	};
	static_assert(TypeErasure::FeatureType<LayoutItemFeature>);
}

export namespace PGUI::UI::Layout
{
	using LayoutItemFeatures = TypeErasure::FeatureComposer<Detail::LayoutItemFeature, TypeErasure::Features::EqualityComparable>;
	//using LayoutItemFeatures = TypeErasure::FeatureComposer<Detail::LayoutItemFeature>;
	using LayoutItem = TypeErasure::Any<LayoutItemFeatures>;

	template <typename T>
	auto MakeLayoutItem(T& item) noexcept
	{
		return TypeErasure::MakeAnyRef<LayoutItemFeatures>(item);
	}

	class LayoutPanel;

	class LayoutPanel
	{
		public:
		explicit LayoutPanel(const RectF bounds) noexcept :
			rect{ bounds }
		{
		}
		virtual ~LayoutPanel() noexcept = default;

		virtual auto RearrangeItems() noexcept -> void = 0;

		template <typename T> requires Detail::LayoutItemFeature::Validator<T>::value
		auto AddItem(T& item) noexcept -> void
		{
			managedItems.push_back(TypeErasure::MakeAnyRef<LayoutItemFeatures>(item));
			OnItemAdded(managedItems.back());
		}

		auto RemoveItem(const std::size_t index) -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}

			managedItems.erase(managedItems.begin() + index);
			OnItemRemoved(index);

			return Error{ ErrorCode::Success };
		}
		[[nodiscard]] const auto& GetItems() const noexcept
		{
			return managedItems;
		}
		[[nodiscard]] auto& GetItems() noexcept
		{
			return managedItems;
		}
		[[nodiscard]] auto GetItem(const std::size_t& index) const noexcept -> Result<std::reference_wrapper<const LayoutItem>>
		{
			if (index >= managedItems.size())
			{
				return Unexpected{
					Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range")
				};
			}

			return managedItems.at(index);
		}
		[[nodiscard]] auto GetItem(const std::size_t& index) noexcept -> Result<std::reference_wrapper<LayoutItem>>
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
		auto ArrangeItem(LayoutItem& item, const RectF assignedBounds) const noexcept -> void
		{
			auto converted = assignedBounds;
			converted.Shift(rect.TopLeft());
			item.MoveAndResize(converted);
		}
		auto MoveItem(LayoutItem& item, const PointF point) const noexcept -> void
		{
			auto converted = point;
			converted.Shift(rect.TopLeft());
			item.Move(converted);
		}
		static auto ResizeItem(LayoutItem& item, const SizeF size) noexcept -> void
		{
			item.Resize(size);
		}
		auto ArrangeItem(const std::size_t index, const RectF assignedBounds) noexcept -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}
			ArrangeItem(managedItems.at(index), assignedBounds);
			return Error{ ErrorCode::Success };
		}
		auto MoveItem(const std::size_t index, const PointF point) noexcept -> Error
		{
			if (index >= managedItems.size())
			{
				return Error{ ErrorCode::InvalidArgument }.SuggestFix(L"Given index is out of range");
			}
			MoveItem(managedItems.at(index), point);
			return Error{ ErrorCode::Success };
		}
		auto ResizeItem(const std::size_t index, const SizeF size) noexcept -> Error
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

		auto operator==(const LayoutPanel& other) const noexcept -> bool
		{
			return this == &other;
		}

		private:
		std::vector<LayoutItem> managedItems;
		RectF rect;
	};
}

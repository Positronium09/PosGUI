module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionViewBox;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionViewBox : public CompositionObjectT<WUC::CompositionViewBox>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] static auto Create(const RectF& rect) noexcept -> Result<CompositionViewBox>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &rect]
			{
				const auto viewBox = compositor.CreateViewBox();
				viewBox.Offset(rect.TopLeft());
				viewBox.Size(rect.Size());

				return CompositionViewBox{ viewBox };
			});
		}

		[[nodiscard]] auto GetHorizontalAlignmentRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().HorizontalAlignmentRatio();
			});
		}
		[[nodiscard]] auto SetHorizontalAlignmentRatio(
			const float horizontalAlignmentRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, horizontalAlignmentRatio]
			{
				Get().HorizontalAlignmentRatio(horizontalAlignmentRatio);
			});
		}

		[[nodiscard]] auto GetOffset() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().Offset() };
			});
		}
		[[nodiscard]] auto SetOffset(const Point2F& offset) const noexcept -> Result<void>
		{
			return CallProjected([this, &offset]
			{
				Get().Offset(offset);
			});
		}

		[[nodiscard]] auto GetSize() const noexcept -> Result<SizeF>
		{
			return CallProjected([this]
			{
				return SizeF{ Get().Size() };
			});
		}
		[[nodiscard]] auto SetSize(const SizeF& size) const noexcept -> Result<void>
		{
			return CallProjected([this, &size]
			{
				Get().Size(size);
			});
		}

		[[nodiscard]] auto GetStretch() const noexcept -> Result<Stretch>
		{
			return CallProjected([this]
			{
				return Get().Stretch();
			});
		}
		[[nodiscard]] auto SetStretch(const Stretch stretch) const noexcept -> Result<void>
		{
			return CallProjected([this, stretch]
			{
				Get().Stretch(stretch);
			});
		}

		[[nodiscard]] auto GetVerticalAlignmentRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().VerticalAlignmentRatio();
			});
		}
		[[nodiscard]] auto SetVerticalAlignmentRatio(
			const float verticalAlignmentRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, verticalAlignmentRatio]
			{
				Get().VerticalAlignmentRatio(verticalAlignmentRatio);
			});
		}

		[[nodiscard]] auto GetRectangle() const noexcept -> Result<RectF>
		{
			auto offset = GetOffset();
			if (!offset.has_value())
			{
				return Unexpected{ offset.error().AddDetail(L"part", L"Offset") };
			}
			auto size = GetSize();
			if (!size.has_value())
			{
				return Unexpected{ size.error().AddDetail(L"part", L"Size") };
			}

			return RectF{ offset.value(), size.value() };
		}
		[[nodiscard]] auto SetRectangle(const RectF& rect) const noexcept -> Result<void>
		{
			if (auto result = SetOffset(rect.TopLeft());
				!result.has_value())
			{
				return Unexpected{ result.error().AddDetail(L"part", L"Offset") };
			}
			if (auto result = SetSize(rect.Size());
				!result.has_value())
			{
				return Unexpected{ result.error().AddDetail(L"part", L"Size") };
			}

			return EmptyResult;
		}
	};
}

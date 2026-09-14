module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Brush.CompositionSurfaceBrush;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Brush.CompositionBrush;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class CompositionSurfaceBrush : public CompositionBrushT<WUC::CompositionSurfaceBrush>
	{
		public:
		using CompositionBrushT::CompositionBrushT;

		[[nodiscard]] static auto Create(const WrapperDerivedFrom<WUC::ICompositionSurface> auto& surface) noexcept -> Result<CompositionSurfaceBrush>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, &surface]
			{
				return CompositionSurfaceBrush{ compositor.CreateSurfaceBrush(surface.Get()) };
			});
		}

		[[nodiscard]] auto GetAnchorPoint() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().AnchorPoint() };
			});
		}
		[[nodiscard]] auto SetAnchorPoint(const Point2F& anchorPoint) const noexcept -> Result<void>
		{
			return CallProjected([this, &anchorPoint]
			{
				this->Get().AnchorPoint(anchorPoint);
			});
		}

		[[nodiscard]] auto GetBitmapInterpolationMode() const noexcept -> Result<BitmapInterpolationMode>
		{
			return CallProjected([this]
			{
				return this->Get().BitmapInterpolationMode();
			});
		}
		[[nodiscard]] auto SetBitmapInterpolationMode(const BitmapInterpolationMode bitmapInterpolationMode) const noexcept -> Result<void>
		{
			return CallProjected([this, bitmapInterpolationMode]
			{
				this->Get().BitmapInterpolationMode(bitmapInterpolationMode);
			});
		}

		[[nodiscard]] auto GetCenterPoint() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().CenterPoint() };
			});
		}
		[[nodiscard]] auto SetCenterPoint(const Point2F& centerPoint) const noexcept -> Result<void>
		{
			return CallProjected([this, &centerPoint]
			{
				this->Get().CenterPoint(centerPoint);
			});
		}

		[[nodiscard]] auto GetHorizontalAlignmentRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().HorizontalAlignmentRatio();
			});
		}
		[[nodiscard]] auto SetHorizontalAlignmentRatio(const float horizontalAlignmentRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, horizontalAlignmentRatio]
			{
				this->Get().HorizontalAlignmentRatio(horizontalAlignmentRatio);
			});
		}

		[[nodiscard]] auto GetOffset() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().Offset() };
			});
		}
		[[nodiscard]] auto SetOffset(const Point2F& offset) const noexcept -> Result<void>
		{
			return CallProjected([this, &offset]
			{
				this->Get().Offset(offset);
			});
		}

		[[nodiscard]] auto GetRotationAngle() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().RotationAngle();
			});
		}
		[[nodiscard]] auto SetRotationAngle(const float rotationAngle) const noexcept -> Result<void>
		{
			return CallProjected([this, rotationAngle]
			{
				this->Get().RotationAngle(rotationAngle);
			});
		}

		[[nodiscard]] auto GetRotationAngleInDegrees() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().RotationAngleInDegrees();
			});
		}
		[[nodiscard]] auto SetRotationAngleInDegrees(const float rotationAngleInDegrees) const noexcept -> Result<void>
		{
			return CallProjected([this, rotationAngleInDegrees]
			{
				this->Get().RotationAngleInDegrees(rotationAngleInDegrees);
			});
		}

		[[nodiscard]] auto GetScale() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().Scale() };
			});
		}
		[[nodiscard]] auto SetScale(const Point2F& scale) const noexcept -> Result<void>
		{
			return CallProjected([this, &scale]
			{
				this->Get().Scale(scale);
			});
		}

		[[nodiscard]] auto EnableSnapToPixels(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				this->Get().SnapToPixels(enabled);
			});
		}
		[[nodiscard]] auto DisableSnapToPixels() const noexcept -> Result<void> { return EnableSnapToPixels(false); }
		[[nodiscard]] auto IsSnapToPixelsEnabled() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return this->Get().SnapToPixels();
			});
		}

		[[nodiscard]] auto GetStretch() const noexcept -> Result<Stretch>
		{
			return CallProjected([this]
			{
				return this->Get().Stretch();
			});
		}
		[[nodiscard]] auto SetStretch(const Stretch stretch) const noexcept -> Result<void>
		{
			return CallProjected([this, stretch]
			{
				this->Get().Stretch(stretch);
			});
		}

		//TODO Surface ICompositionSurface

		[[nodiscard]] auto GetTransformMatrix() const noexcept -> Result<Matrix3x2>
		{
			return CallProjected([this]
			{
				return Matrix3x2{ this->Get().TransformMatrix() };
			});
		}
		[[nodiscard]] auto SetTransformMatrix(const Matrix3x2& transformMatrix) const noexcept -> Result<void>
		{
			return CallProjected([this, &transformMatrix]
			{
				this->Get().TransformMatrix(transformMatrix);
			});
		}

		[[nodiscard]] auto GetVerticalAlignmentRatio() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().VerticalAlignmentRatio();
			});
		}
		[[nodiscard]] auto SetVerticalAlignmentRatio(const float verticalAlignmentRatio) const noexcept -> Result<void>
		{
			return CallProjected([this, verticalAlignmentRatio]
			{
				this->Get().VerticalAlignmentRatio(verticalAlignmentRatio);
			});
		}
	};
}

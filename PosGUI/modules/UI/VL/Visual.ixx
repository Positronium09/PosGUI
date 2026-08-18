module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Visual;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.Shape;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;
namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::Visual> Projected = WUC::Visual>
	class VisualT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetAnchorPoint() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ this->Get().AnchorPoint() };
			});
		}
		auto SetAnchorPoint(const Point2F& point) const noexcept -> Result<void>
		{
			return CallProjected([this, &point]
			{
				this->Get().AnchorPoint(point);
			});
		}

		[[nodiscard]] auto GetBackfaceVisibility() const noexcept -> Result<BackfaceVisibility>
		{
			return CallProjected([this]
			{
				return this->Get().BackfaceVisibility();
			});
		}
		auto SetBackfaceVisibility(const BackfaceVisibility visibility) const noexcept -> Result<void>
		{
			return CallProjected([this, visibility]
			{
				this->Get().BackfaceVisibility(visibility);
			});
		}

		[[nodiscard]] auto GetBorderMode() const noexcept -> Result<BorderMode>
		{
			return CallProjected([this]
			{
				return this->Get().BorderMode();
			});
		}
		auto SetBorderMode(const BorderMode mode) const noexcept -> Result<void>
		{
			return CallProjected([this, mode]
			{
				this->Get().BorderMode(mode);
			});
		}

		[[nodiscard]] auto GetCenterPoint() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ this->Get().CenterPoint() };
			});
		}
		auto SetCenterPoint(const Point3F& point) const noexcept -> Result<void>
		{
			return CallProjected([this, &point]
			{
				this->Get().CenterPoint(point);
			});
		}

		//TODO Clip CompositionClip

		[[nodiscard]] auto GetCompositeMode() const noexcept -> Result<CompositeMode>
		{
			return CallProjected([this]
			{
				return this->Get().CompositeMode();
			});
		}
		auto SetCompositeMode(const CompositeMode mode) const noexcept -> Result<void>
		{
			return CallProjected([this, mode]
			{
				this->Get().CompositeMode(mode);
			});
		}

		auto EnableHitTestVisible(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				this->Get().IsHitTestVisible(enabled);
			});
		}
		auto DisableHitTestVisible() const noexcept -> Result<void> { return EnableHitTestVisible(false); }
		[[nodiscard]] auto IsHitTestVisible() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return this->Get().IsHitTestVisible();
			});
		}

		auto EnablePixelSnapping(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				this->Get().IsPixelSnappingEnabled(enabled);
			});
		}
		auto DisablePixelSnapping() const noexcept -> Result<void> { return EnablePixelSnapping(false); }
		[[nodiscard]] auto IsPixelSnappingEnabled() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return this->Get().IsPixelSnappingEnabled();
			});
		}

		auto EnableVisibility(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				this->Get().IsVisible(enabled);
			});
		}
		auto DisableVisibility() const noexcept -> Result<void> { return EnableVisibility(false); }
		[[nodiscard]] auto IsVisible() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return this->Get().IsVisible();
			});
		}

		[[nodiscard]] auto GetOffset() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ this->Get().Offset() };
			});
		}
		auto SetOffset(const Point3F& offset) const noexcept -> Result<void>
		{
			return CallProjected([this, &offset]
			{
				this->Get().Offset(offset);
			});
		}

		[[nodiscard]] auto GetOpacity() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().Opacity();
			});
		}
		auto SetOpacity(const float opacity) const noexcept -> Result<void>
		{
			return CallProjected([this, opacity]
			{
				this->Get().Opacity(opacity);
			});
		}

		[[nodiscard]] auto GetOrientation() const noexcept -> Result<Quaternion>
		{
			return CallProjected([this]
			{
				return this->Get().Orientation();
			});
		}
		auto SetOrientation(const Quaternion& quaternion) const noexcept -> Result<void>
		{
			return CallProjected([this, &quaternion]
			{
				this->Get().Orientation(quaternion);
			});
		}

		[[nodiscard]] auto GetRelativeOffsetAdjustment() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ this->Get().RelativeOffsetAdjustment() };
			});
		}
		auto SetRelativeOffsetAdjustment(const Point3F& adjustment) const noexcept -> Result<void>
		{
			return CallProjected([this, &adjustment]
			{
				this->Get().RelativeOffsetAdjustment(adjustment);
			});
		}

		[[nodiscard]] auto GetRelativeSizeAdjustment() const noexcept -> Result<SizeF>
		{
			return CallProjected([this]
			{
				return SizeF{ this->Get().RelativeSizeAdjustment() };
			});
		}
		auto SetRelativeSizeAdjustment(const SizeF& adjustment) const noexcept -> Result<void>
		{
			return CallProjected([this, &adjustment]
			{
				this->Get().RelativeSizeAdjustment(adjustment);
			});
		}

		[[nodiscard]] auto GetRotationAngle() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().RotationAngle();
			});
		}
		auto SetRotationAngle(const float angle) const noexcept -> Result<void>
		{
			return CallProjected([this, angle]
			{
				this->Get().RotationAngle(angle);
			});
		}

		[[nodiscard]] auto GetRotationAngleInDegrees() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().RotationAngleInDegrees();
			});
		}
		auto SetRotationAngleInDegrees(const float angle) const noexcept -> Result<void>
		{
			return CallProjected([this, angle]
			{
				this->Get().RotationAngleInDegrees(angle);
			});
		}

		[[nodiscard]] auto GetRotationAxis() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ this->Get().RotationAxis() };
			});
		}
		auto SetRotationAxis(const Point3F& axis) const noexcept -> Result<void>
		{
			return CallProjected([this, &axis]
			{
				this->Get().RotationAxis(axis);
			});
		}

		[[nodiscard]] auto GetScale() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ this->Get().Scale() };
			});
		}
		auto SetScale(const Point3F& scale) const noexcept -> Result<void>
		{
			return CallProjected([this, &scale]
			{
				this->Get().Scale(scale);
			});
		}

		[[nodiscard]] auto GetSize() const noexcept -> Result<SizeF>
		{
			return CallProjected([this]
			{
				return SizeF{ this->Get().Size() };
			});
		}
		auto SetSize(const SizeF& size) const noexcept -> Result<void>
		{
			return CallProjected([this, &size]
			{
				this->Get().Size(size);
			});
		}

		[[nodiscard]] auto GetTransformMatrix() const noexcept -> Result<Matrix4x4>
		{
			return CallProjected([this]
			{
				return Matrix4x4{ this->Get().TransformMatrix() };
			});
		}
		auto SetTransformMatrix(const Matrix4x4& matrix) const noexcept -> Result<void>
		{
			return CallProjected([this, &matrix]
			{
				this->Get().TransformMatrix(matrix);
			});
		}
	};
	using Visual = VisualT<>;

	template <HandleDerivedFrom<WUC::Visual> Projected>
	[[nodiscard]] auto GetParentForTransformOf(const VisualT<Projected>& visual) noexcept -> Result<Visual>
	{
		return CallProjected([&visual]
		{
			return Visual{ visual.Get().ParentForTransform() };
		});
	}
	template <HandleDerivedFrom<WUC::Visual> Projected>
	auto SetParentForTransformOf(const VisualT<Projected>& visual, const Visual& parentForTransform) noexcept -> Result<void>
	{
		return CallProjected([&visual, &parentForTransform]
		{
			visual.Get().ParentForTransform(parentForTransform.Get());
		});
	}
}

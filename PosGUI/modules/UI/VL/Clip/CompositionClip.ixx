module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Clip.CompositionClip;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionClip> Projected = WUC::CompositionClip>
	class CompositionClipT : public CompositionObjectT<Projected>
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
		[[nodiscard]] auto SetAnchorPoint(const Point2F& anchorPoint) const noexcept -> Result<void>
		{
			return CallProjected([this, &anchorPoint]
			{
				this->Get().AnchorPoint(anchorPoint);
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
	};
	using CompositionClip = CompositionClipT<>;
}

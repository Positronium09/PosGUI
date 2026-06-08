module;
#include <dcomp.h>

export module PGUI.UI.DComp.Transform2D;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.DComp.Effect;
import PGUI.UI.DComp.Animation;

export namespace PGUI::UI::DComp
{
	template <typename Interface = IDCompositionTransform>
	class Transform : public Transform3D<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionTransform>);

		public:
		Transform() noexcept = default;
		explicit(false) Transform(const ComPtr<Interface>& ptr) noexcept :
			Transform3D<Interface>{ ptr }
		{ }

		explicit(false) operator Transform<>() const noexcept
		{
			return Transform<>{ this->Get() };
		}
	};

	class TranslateTransform : public Transform<IDCompositionTranslateTransform>
	{
		public:
		TranslateTransform() noexcept = default;
		explicit(false) TranslateTransform(const ComPtr<IDCompositionTranslateTransform>& ptr) noexcept :
			Transform{ ptr }
		{ }

		[[nodiscard]] auto SetOffsetX(const float offsetX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffsetX(offsetX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetOffsetX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffsetX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetOffsetY(const float offsetY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffsetY(offsetY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetOffsetY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffsetY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class ScaleTransform : public Transform<IDCompositionScaleTransform>
	{
		public:
		ScaleTransform() noexcept = default;
		explicit(false) ScaleTransform(const ComPtr<IDCompositionScaleTransform>& ptr) noexcept :
			Transform{ ptr }
		{ }

		[[nodiscard]] auto SetScaleX(const float scaleX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetScaleX(scaleX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetScaleX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetScaleX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetScaleY(const float scaleY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetScaleY(scaleY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetScaleY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetScaleY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterX(const float centerX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterX(centerX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterY(const float centerY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterY(centerY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class RotateTransform : public Transform<IDCompositionRotateTransform>
	{
		public:
		RotateTransform() noexcept = default;
		explicit(false) RotateTransform(const ComPtr<IDCompositionRotateTransform>& ptr) noexcept :
			Transform{ ptr }
		{ }

		[[nodiscard]] auto SetAngle(const float angle) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngle(angle) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAngle(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngle(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterX(const float centerX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterX(centerX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterY(const float centerY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterY(centerY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class SkewTransform : public Transform<IDCompositionSkewTransform>
	{
		public:
		SkewTransform() noexcept = default;
		explicit(false) SkewTransform(const ComPtr<IDCompositionSkewTransform>& ptr) noexcept :
			Transform{ ptr }
		{ }

		[[nodiscard]] auto SetAngleX(const float angleX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngleX(angleX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAngleX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngleX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAngleY(const float angleY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngleY(angleY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAngleY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngleY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterX(const float centerX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterX(centerX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterY(const float centerY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterY(centerY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class MatrixTransform : public Transform<IDCompositionMatrixTransform>
	{
		public:
		MatrixTransform() noexcept = default;
		explicit(false) MatrixTransform(const ComPtr<IDCompositionMatrixTransform>& ptr) noexcept :
			Transform{ ptr }
		{ }

		[[nodiscard]] auto SetMatrix(const D2D::Matrix3x2& matrix) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetMatrix(matrix) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetMatrixElement(const int row, const int column, const float value) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetMatrixElement(row, column, value) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetMatrixElement(const int row, const int column, const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetMatrixElement(row, column, animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};
}

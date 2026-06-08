module;
#include <d2d1_1.h>
#include <dcomp.h>

export module PGUI.UI.DComp.Transform3D;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.DComp.Effect;
import PGUI.UI.DComp.Animation;

export namespace PGUI::UI::DComp
{
	class TranslateTransform3D : public Transform3D<IDCompositionTranslateTransform3D>
	{
		public:
		TranslateTransform3D() noexcept = default;
		explicit(false) TranslateTransform3D(const ComPtr<IDCompositionTranslateTransform3D>& ptr) noexcept :
			Transform3D{ ptr }
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

		[[nodiscard]] auto SetOffsetZ(const float offsetZ) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffsetZ(offsetZ) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetOffsetZ(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffsetZ(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class ScaleTransform3D : public Transform3D<IDCompositionScaleTransform3D>
	{
		public:
		ScaleTransform3D() noexcept = default;
		explicit(false) ScaleTransform3D(const ComPtr<IDCompositionScaleTransform3D>& ptr) noexcept :
			Transform3D{ ptr }
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

		[[nodiscard]] auto SetScaleZ(const float scaleZ) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetScaleZ(scaleZ) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetScaleZ(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetScaleZ(animation.GetRaw()) };
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

		[[nodiscard]] auto SetCenterZ(const float centerZ) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterZ(centerZ) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterZ(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterZ(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class RotateTransform3D : public Transform3D<IDCompositionRotateTransform3D>
	{
		public:
		RotateTransform3D() noexcept = default;
		explicit(false) RotateTransform3D(const ComPtr<IDCompositionRotateTransform3D>& ptr) noexcept :
			Transform3D{ ptr }
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

		[[nodiscard]] auto SetAxisX(const float axisX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAxisX(axisX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAxisX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAxisX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAxisY(const float axisY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAxisY(axisY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAxisY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAxisY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAxisZ(const float axisZ) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAxisZ(axisZ) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAxisZ(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAxisZ(animation.GetRaw()) };
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

		[[nodiscard]] auto SetCenterZ(const float centerZ) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterZ(centerZ) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCenterZ(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCenterZ(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	//TODO Write 4x4 matrix struct for D2D and DComp
	class MatrixTransform3D : public Transform3D<IDCompositionMatrixTransform3D>
	{
		public:
		MatrixTransform3D() noexcept = default;
		explicit(false) MatrixTransform3D(const ComPtr<IDCompositionMatrixTransform3D>& ptr) noexcept :
			Transform3D{ ptr }
		{ }

		[[nodiscard]] auto SetMatrix(const D2D1_MATRIX_4X4_F& matrix) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetMatrix(std::bit_cast<D3DMATRIX>(matrix)) };
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

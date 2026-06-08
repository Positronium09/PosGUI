module;
#include <d2d1_1.h>
#include <dcomp.h>

export module PGUI.UI.DComp.FilterEffects;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.Color;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.DComp.DCompStructs;
import PGUI.UI.DComp.Effect;
import PGUI.UI.DComp.Animation;

export namespace PGUI::UI::DComp
{
	class GaussianBlurEffect : public FilterEffect<IDCompositionGaussianBlurEffect>
	{
		public:
		GaussianBlurEffect() noexcept = default;
		explicit(false) GaussianBlurEffect(const ComPtr<IDCompositionGaussianBlurEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetStandardDeviation(const float amount) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetStandardDeviation(amount) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetStandardDeviation(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetStandardDeviation(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBorderMode(const D2D::BorderMode mode) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBorderMode(ToUnderlying<D2D1_BORDER_MODE>(mode)) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class BrightnessEffect : public FilterEffect<IDCompositionBrightnessEffect>
	{
		public:
		BrightnessEffect() noexcept = default;
		explicit(false) BrightnessEffect(const ComPtr<IDCompositionBrightnessEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetWhitePoint(const Vector2F& whitePoint) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetWhitePoint(whitePoint) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlackPoint(const Vector2F& blackPoint) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlackPoint(blackPoint) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetWhitePointX(const float whitePointX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetWhitePointX(whitePointX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetWhitePointX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetWhitePointX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetWhitePointY(const float whitePointY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetWhitePointY(whitePointY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetWhitePointY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetWhitePointY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlackPointX(const float blackPointX) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlackPointX(blackPointX) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlackPointX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlackPointX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlackPointY(const float blackPointY) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlackPointY(blackPointY) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlackPointY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlackPointY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class ColorMatrixEffect : public FilterEffect<IDCompositionColorMatrixEffect>
	{
		public:
		ColorMatrixEffect() noexcept = default;
		explicit(false) ColorMatrixEffect(const ComPtr<IDCompositionColorMatrixEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetMatrix(const D2D1_MATRIX_5X4_F& matrix) const noexcept -> Result<void>
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

		[[nodiscard]] auto SetAlphaMode(const D2D::ColorMatrixAlphaMode mode) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaMode(ToUnderlying<D2D1_COLORMATRIX_ALPHA_MODE>(mode)) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetClampOutput(const bool clamp) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetClampOutput(clamp) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class SaturationEffect : public FilterEffect<IDCompositionSaturationEffect>
	{
		public:
		SaturationEffect() noexcept = default;
		explicit(false) SaturationEffect(const ComPtr<IDCompositionSaturationEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetSaturation(const float ratio) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetSaturation(ratio) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetSaturation(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetSaturation(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class HueRotationEffect : public FilterEffect<IDCompositionHueRotationEffect>
	{
		public:
		HueRotationEffect() noexcept = default;
		explicit(false) HueRotationEffect(const ComPtr<IDCompositionHueRotationEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetAngle(const float amountDegrees) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAngle(amountDegrees) };
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
	};

	class ShadowEffect : public FilterEffect<IDCompositionShadowEffect>
	{
		public:
		ShadowEffect() noexcept = default;
		explicit(false) ShadowEffect(const ComPtr<IDCompositionShadowEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetStandardDeviation(const float amount) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetStandardDeviation(amount) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetStandardDeviation(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetStandardDeviation(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetColor(const Vector4F& color) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetColor(color) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetColor(const RGBA& color) const noexcept -> Result<void>
		{
			return SetColor(Vector4F{ color });
		}

		[[nodiscard]] auto SetRed(const float red) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRed(red) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRed(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRed(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreen(const float green) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreen(green) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreen(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreen(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlue(const float blue) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlue(blue) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlue(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlue(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlpha(const float alpha) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlpha(alpha) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlpha(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlpha(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class LinearTransferEffect : public FilterEffect<IDCompositionLinearTransferEffect>
	{
		public:
		LinearTransferEffect() noexcept = default;
		explicit(false) LinearTransferEffect(const ComPtr<IDCompositionLinearTransferEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetRedYIntercept(const float redYIntercept) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedYIntercept(redYIntercept) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedYIntercept(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedYIntercept(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedSlope(const float redSlope) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedSlope(redSlope) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedSlope(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedSlope(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedDisable(const bool redDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedDisable(redDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenYIntercept(const float greenYIntercept) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenYIntercept(greenYIntercept) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenYIntercept(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenYIntercept(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenSlope(const float greenSlope) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenSlope(greenSlope) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenSlope(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenSlope(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenDisable(const bool greenDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenDisable(greenDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueYIntercept(const float blueYIntercept) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueYIntercept(blueYIntercept) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueYIntercept(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueYIntercept(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueSlope(const float blueSlope) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueSlope(blueSlope) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueSlope(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueSlope(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueDisable(const bool blueDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueDisable(blueDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaYIntercept(const float alphaYIntercept) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaYIntercept(alphaYIntercept) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaYIntercept(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaYIntercept(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaSlope(const float alphaSlope) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaSlope(alphaSlope) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaSlope(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaSlope(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaDisable(const bool alphaDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaDisable(alphaDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetClampOutput(const bool clampOutput) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetClampOutput(clampOutput) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class TableTransferEffect : public FilterEffect<IDCompositionTableTransferEffect>
	{
		public:
		TableTransferEffect() noexcept = default;
		explicit(false) TableTransferEffect(const ComPtr<IDCompositionTableTransferEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetRedTable(const std::span<const float> table) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedTable(table.data(), static_cast<UINT>(table.size())) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenTable(const std::span<const float> table) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenTable(table.data(), static_cast<UINT>(table.size())) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueTable(const std::span<const float> table) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueTable(table.data(), static_cast<UINT>(table.size())) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaTable(const std::span<const float> table) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaTable(table.data(), static_cast<UINT>(table.size())) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedDisable(const bool redDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedDisable(redDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenDisable(const bool greenDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenDisable(greenDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueDisable(const bool blueDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueDisable(blueDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaDisable(const bool alphaDisable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaDisable(alphaDisable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedTableValue(const UINT index, const float value) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedTableValue(index, value) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRedTableValue(const UINT index, const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRedTableValue(index, animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenTableValue(const UINT index, const float value) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenTableValue(index, value) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetGreenTableValue(const UINT index, const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetGreenTableValue(index, animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueTableValue(const UINT index, const float value) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueTableValue(index, value) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBlueTableValue(const UINT index, const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBlueTableValue(index, animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaTableValue(const UINT index, const float value) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaTableValue(index, value) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAlphaTableValue(const UINT index, const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAlphaTableValue(index, animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetClampOutput(const bool clampOutput) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetClampOutput(clampOutput) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class CompositeEffect : public FilterEffect<IDCompositionCompositeEffect>
	{
		public:
		CompositeEffect() noexcept = default;
		explicit(false) CompositeEffect(const ComPtr<IDCompositionCompositeEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetMode(const D2D::CompositeMode mode) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetMode(ToUnderlying<D2D1_COMPOSITE_MODE>(mode)) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class BlendEffect : public FilterEffect<IDCompositionBlendEffect>
	{
		public:
		BlendEffect() noexcept = default;
		explicit(false) BlendEffect(const ComPtr<IDCompositionBlendEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetMode(const D2D::BlendMode mode) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetMode(ToUnderlying<D2D1_BLEND_MODE>(mode)) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class ArithmeticCompositeEffect : public FilterEffect<IDCompositionArithmeticCompositeEffect>
	{
		public:
		ArithmeticCompositeEffect() noexcept = default;
		explicit(false) ArithmeticCompositeEffect(const ComPtr<IDCompositionArithmeticCompositeEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetCoefficients(const Vector4F& coefficients) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficients(coefficients) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetClampOutput(const bool clampOutput) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetClampOutput(clampOutput) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient1(const float coefficient1) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient1(coefficient1) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient1(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient1(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient2(const float coefficient2) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient2(coefficient2) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient2(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient2(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient3(const float coefficient3) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient3(coefficient3) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient3(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient3(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient4(const float coefficient4) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient4(coefficient4) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCoefficient4(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetCoefficient4(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class AffineTransform2DEffect : public FilterEffect<IDCompositionAffineTransform2DEffect>
	{
		public:
		AffineTransform2DEffect() noexcept = default;
		explicit(false) AffineTransform2DEffect(const ComPtr<IDCompositionAffineTransform2DEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetInterpolationMode(const D2D::AffineTransformInterpolationMode interpolationMode) const noexcept -> Result<void>
		{
			if (const auto error = Error{
					Get()->SetInterpolationMode(
						ToUnderlying<D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE>(interpolationMode))
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBorderMode(const D2D::BorderMode borderMode) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBorderMode(ToUnderlying<D2D1_BORDER_MODE>(borderMode)) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTransformMatrix(const D2D::Matrix3x2& transformMatrix) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTransformMatrix(transformMatrix) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTransformMatrixElement(const int row, const int column, const float value) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTransformMatrixElement(row, column, value) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTransformMatrixElement(const int row, const int column, const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTransformMatrixElement(row, column, animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetSharpness(const float sharpness) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetSharpness(sharpness) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetSharpness(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetSharpness(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};

	class TurbulenceEffect : public FilterEffect<IDCompositionTurbulenceEffect>
	{
		public:
		TurbulenceEffect() noexcept = default;
		explicit(false) TurbulenceEffect(const ComPtr<IDCompositionTurbulenceEffect>& ptr) noexcept :
			FilterEffect{ ptr }
		{ }

		[[nodiscard]] auto SetOffset(const Vector2F& offset) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetOffset(offset) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBaseFrequency(const Vector2F& frequency) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBaseFrequency(frequency) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetSize(const Vector2F& size) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetSize(size) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetNumOctaves(const UINT numOctaves) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetNumOctaves(numOctaves) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetSeed(const UINT seed) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetSeed(seed) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetNoise(const D2D::TurbulenceNoise noise) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetNoise(ToUnderlying<D2D1_TURBULENCE_NOISE>(noise)) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetStitchable(const bool stitchable) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetStitchable(stitchable) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};
}

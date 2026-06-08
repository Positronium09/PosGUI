module;
#include <dcompanimation.h>

export module PGUI.UI.DComp.Animation;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::UI::DComp
{
	class Animation : public ComPtrHolder<IDCompositionAnimation>
	{
		public:
		Animation() noexcept = default;
		explicit(false) Animation(const ComPtr<IDCompositionAnimation>& ptr) noexcept :
			ComPtrHolder{ ptr }
		{ }

		[[nodiscard]] auto Reset() const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->Reset() };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetAbsoluteBeginTime(const LARGE_INTEGER beginTime) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetAbsoluteBeginTime(beginTime) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto AddCubic(const double beginOffset,
			const float constantCoefficient,
			const float linearCoefficient,
			const float quadraticCoefficient,
			const float cubicCoefficient) const noexcept -> Result<void>
		{
			if (const auto error = Error{
					Get()->AddCubic(beginOffset, constantCoefficient,
						linearCoefficient, quadraticCoefficient, cubicCoefficient)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto AddSinusoidal(const double beginOffset,
			const float bias,
			const float amplitude,
			const float frequency,
			const float phase) const noexcept -> Result<void>
		{
			if (const auto error = Error{
					Get()->AddSinusoidal(beginOffset, bias, amplitude, frequency, phase)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto AddRepeat(const double beginOffset, const double durationToRepeat) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->AddRepeat(beginOffset, durationToRepeat) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto End(const double endOffset, const float endValue) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->End(endOffset, endValue) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};
}

module;
#include <dcomp.h>

export module PGUI.UI.DComp.Clip;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.Shape2D;
import PGUI.UI.DComp.Animation;

export namespace PGUI::UI::DComp
{
	template <typename Interface = IDCompositionClip>
	class Clip : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionClip>);

		public:
		Clip() noexcept = default;
		explicit(false) Clip(const ComPtr<Interface>& ptr) noexcept :
			ComPtrHolder<Interface>{ ptr }
		{ }

		explicit(false) operator Clip<>() const noexcept
		{
			return Clip<>{ this->Get() };
		}
	};

	class RectangleClip : public Clip<IDCompositionRectangleClip>
	{
		public:
		RectangleClip() noexcept = default;
		explicit(false) RectangleClip(const ComPtr<IDCompositionRectangleClip>& ptr) noexcept :
			Clip{ ptr }
		{ }

		[[nodiscard]] auto SetLeft(const float left) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetLeft(left) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetLeft(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetLeft(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTop(const float top) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTop(top) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTop(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTop(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRight(const float right) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRight(right) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetRight(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRight(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottom(const float bottom) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottom(bottom) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottom(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottom(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopLeftRadiusX(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopLeftRadiusX(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopLeftRadiusX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopLeftRadiusX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopLeftRadiusY(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopLeftRadiusY(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopLeftRadiusY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopLeftRadiusY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopRightRadiusX(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopRightRadiusX(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopRightRadiusX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopRightRadiusX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopRightRadiusY(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopRightRadiusY(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTopRightRadiusY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetTopRightRadiusY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomRightRadiusX(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomRightRadiusX(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomRightRadiusX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomRightRadiusX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomRightRadiusY(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomRightRadiusY(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomRightRadiusY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomRightRadiusY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomLeftRadiusX(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomLeftRadiusX(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomLeftRadiusX(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomLeftRadiusX(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomLeftRadiusY(const float radius) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomLeftRadiusY(radius) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetBottomLeftRadiusY(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetBottomLeftRadiusY(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetCornerRadius(const float xRadius, const float yRadius) const noexcept -> Result<void>
		{
			if (auto result = SetTopLeftRadiusX(xRadius);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetTopLeftRadiusY(yRadius);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetTopRightRadiusX(xRadius);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetTopRightRadiusY(yRadius);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetBottomRightRadiusX(xRadius);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetBottomRightRadiusY(yRadius);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetBottomLeftRadiusX(xRadius);
				!result.has_value())
			{
				return result;
			}

			return SetBottomLeftRadiusY(yRadius);
		}

		[[nodiscard]] auto SetRoundedRect(const RoundedRect& roundedRect) const noexcept -> Result<void>
		{
			if (auto result = SetLeft(roundedRect.left);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetTop(roundedRect.top);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetRight(roundedRect.right);
				!result.has_value())
			{
				return result;
			}
			if (auto result = SetBottom(roundedRect.bottom);
				!result.has_value())
			{
				return result;
			}

			return SetCornerRadius(roundedRect.xRadius, roundedRect.yRadius);
		}
	};
}

module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTransition;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::UI::Animation
{
	class AnimationTransition : public ComPtrHolder<IUIAnimationTransition2>
	{
		public:
		AnimationTransition() noexcept = default;

		explicit(false) AnimationTransition(const ComPtr<IUIAnimationTransition2>& ptr) noexcept;

		[[nodiscard]] auto GetDimension() const noexcept -> Result<UINT>;

		[[nodiscard]] auto GetDuration() const noexcept -> Result<double>;

		[[nodiscard]] auto IsDurationKnown() const noexcept -> bool;

		auto SetInitialValue(double value) const noexcept -> Error;

		auto SetInitialValue(std::span<const double> values) const noexcept -> Error;

		auto SetInitialVelocity(double velocity) const noexcept -> Error;

		auto SetInitialVelocity(std::span<const double> velocities) const noexcept -> Error;
	};
}

module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationTransition;

import std;

import PGUI.ComPtr;

export namespace PGUI::UI::Animation
{
	class AnimationTransition : public ComPtrHolder<IUIAnimationTransition2>
	{
		public:
		AnimationTransition() noexcept = default;

		explicit(false) AnimationTransition(const ComPtr<IUIAnimationTransition2>& ptr) noexcept;

		[[nodiscard]] auto GetDimension() const -> UINT;

		[[nodiscard]] auto GetDuration() const -> double;

		[[nodiscard]] auto IsDurationKnown() const noexcept -> bool;

		auto SetInitialValue(double value) const -> void;

		auto SetInitialValue(std::span<const double> values) const -> void;

		auto SetInitialVelocity(double velocity) const -> void;

		auto SetInitialVelocity(std::span<const double> velocities) const -> void;
	};
}

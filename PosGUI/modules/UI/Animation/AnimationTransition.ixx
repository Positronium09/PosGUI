module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationTransition;

import std;

import PGUI.ComPtr;

export namespace  PGUI::UI::Animation
{
	class AnimationTransition : public ComPtrHolder<IUIAnimationTransition2>
	{
		public:
		AnimationTransition() noexcept = default;
		AnimationTransition(ComPtr<IUIAnimationTransition2> ptr) noexcept;

		[[nodiscard]] auto GetDimension() const -> UINT;
		[[nodiscard]] auto GetDuration() const -> double;
		[[nodiscard]] auto IsDurationKnown() const noexcept -> bool;

		void SetInitialValue(double value) const;
		void SetInitialValue(std::span<const double> values) const;
		void SetInitialVelocity(double velocity) const;
		void SetInitialVelocity(std::span<const double> velocities) const;
	};
}

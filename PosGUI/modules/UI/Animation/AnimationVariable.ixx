module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationVariable;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	class AnimationVariable : public ComPtrHolder<IUIAnimationVariable2>
	{
		public:
		AnimationVariable() noexcept = default;

		explicit(false) AnimationVariable(const ComPtr<IUIAnimationVariable2>& ptr) noexcept;

		[[nodiscard]] auto GetStoryboard() const noexcept -> Result<Storyboard>;

		[[nodiscard]] auto GetDimension() const noexcept -> Result<UINT>;

		[[nodiscard]] auto GetPreviousIntegerValue() const noexcept -> Result<INT32>;

		[[nodiscard]] auto GetPreviousIntegerVectorValue() const noexcept -> Result<std::vector<INT32>>;

		[[nodiscard]] auto GetIntegerValue() const noexcept -> Result<INT32>;

		[[nodiscard]] auto GetIntegerVectorValue() const noexcept -> Result<std::vector<INT32>>;

		[[nodiscard]] auto GetFinalIntegerValue() const noexcept -> Result<INT32>;

		[[nodiscard]] auto GetFinalIntegerVectorValue() const noexcept -> Result<std::vector<INT32>>;

		[[nodiscard]] auto GetPreviousValue() const noexcept -> Result<double>;

		[[nodiscard]] auto GetPreviousVectorValue() const noexcept -> Result<std::vector<double>>;

		[[nodiscard]] auto GetValue() const noexcept -> Result<double>;

		[[nodiscard]] auto GetVectorValue() const noexcept -> Result<std::vector<double>>;

		[[nodiscard]] auto GetFinalValue() const noexcept -> Result<double>;

		[[nodiscard]] auto GetFinalVectorValue() const noexcept -> Result<std::vector<double>>;

		auto SetLowerBound(double bound) const noexcept -> Result<void>;

		auto SetLowerBound(std::span<const double> bounds) const noexcept -> Result<void>;

		auto SetUpperBound(double bound) const noexcept -> Result<void>;

		auto SetUpperBound(std::span<const double> bounds) const noexcept -> Result<void>;

		auto SetRoundingMode(AnimationRoundingMode mode) const noexcept -> Result<void>;

		auto SetTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Result<void>;

		[[nodiscard]] auto GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>;

		auto SetVariableChangeHandler(
			AnimationVariableChangeEventHandler& handler,
			bool registerForNext = false) const noexcept -> Result<void>;

		auto ClearVariableChangeHandler(bool registerForNext = false) const noexcept -> Result<void>;

		template <std::floating_point T>
		explicit operator T() const { return static_cast<T>(GetValue().value()); }

		template <std::integral T>
		explicit operator T() const { return static_cast<T>(GetIntegerValue().value()); }

		//TODO SetVariableCurveChangeHandler
	};
}

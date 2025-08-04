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

		auto SetLowerBound(double bound) const noexcept -> Error;

		auto SetLowerBound(std::span<double> bounds) const noexcept -> Error;

		auto SetUpperBound(double bound) const noexcept -> Error;

		auto SetUpperBound(std::span<double> bounds) const noexcept -> Error;

		auto SetRoundingMode(AnimationRoundingMode mode) const noexcept -> Error;

		auto SetTag(const ComPtr<IUnknown>& obj, UINT32 id) const noexcept -> Error;

		[[nodiscard]] auto GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>;

		auto SetVariableChangeHandler(
			AnimationVariableChangeEventHandler& handler,
			bool registerForNext = false) const noexcept -> Error;

		auto ClearVariableChangeHandler(bool registerForNext = false) const noexcept -> Error;

		template <std::floating_point T>
		explicit(false) operator T() const noexcept { return static_cast<T>(GetValue().value()); }

		template <std::integral T>
		explicit(false) operator T() const noexcept { return static_cast<T>(GetIntegerValue().value()); }

		//TODO SetVariableCurveChangeHandler
	};
}

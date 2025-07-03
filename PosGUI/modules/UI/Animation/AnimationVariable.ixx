module;
#include <UIAnimation.h>

export module PGUI.UI.Animation:AnimationVariable;

import std;

import PGUI.ComPtr;
import :AnimationInterface;
import :AnimationEnums;

export namespace PGUI::UI::Animation
{
	class AnimationVariable : public ComPtrHolder<IUIAnimationVariable2>
	{
		public:
		AnimationVariable() noexcept = default;

		explicit(false) AnimationVariable(const ComPtr<IUIAnimationVariable2>& ptr) noexcept;

		[[nodiscard]] auto GetStoryboard() const -> Storyboard;

		[[nodiscard]] auto GetDimension() const -> UINT;

		[[nodiscard]] auto GetPreviousIntegerValue() const -> INT32;

		[[nodiscard]] auto GetPreviousIntegerVectorValue() const -> std::vector<INT32>;

		[[nodiscard]] auto GetIntegerValue() const -> INT32;

		[[nodiscard]] auto GetIntegerVectorValue() const -> std::vector<INT32>;

		[[nodiscard]] auto GetFinalIntegerValue() const -> INT32;

		[[nodiscard]] auto GetFinalIntegerVectorValue() const -> std::vector<INT32>;

		[[nodiscard]] auto GetPreviousValue() const -> double;

		[[nodiscard]] auto GetPreviousVectorValue() const -> std::vector<double>;

		[[nodiscard]] auto GetValue() const -> double;

		[[nodiscard]] auto GetVectorValue() const -> std::vector<double>;

		[[nodiscard]] auto GetFinalValue() const -> double;

		[[nodiscard]] auto GetFinalVectorValue() const -> std::vector<double>;

		auto SetLowerBound(double bound) const -> void;

		auto SetLowerBound(std::span<double> bounds) const -> void;

		auto SetUpperBound(double bound) const -> void;

		auto SetUpperBound(std::span<double> bounds) const -> void;

		auto SetRoundingMode(AnimationRoundingMode mode) const -> void;

		auto SetTag(const ComPtr<IUnknown>& obj, UINT32 id) const -> void;

		[[nodiscard]] auto GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>;

		auto SetVariableChangeHandler(
			AnimationVariableChangeEventHandler& handler,
			bool registerForNext = false) const -> void;

		auto ClearVariableChangeHandler(bool registerForNext = false) const -> void;

		template <std::floating_point T>
		explicit(false) operator T() const { return static_cast<T>(GetValue()); }

		template <std::integral T>
		explicit(false) operator T() const { return static_cast<T>(GetIntegerValue()); }

		//TODO SetVariableCurveChangeHandler
	};
}

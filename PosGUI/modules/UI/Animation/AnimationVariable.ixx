module;
#include <UIAnimation.h>

export module PGUI.UI.Animation.AnimationVariable;

import std;

import PGUI.ComPtr;
import PGUI.UI.Animation.AnimationEnums;

export namespace  PGUI::UI::Animation
{
	class Storyboard;
	class AnimationVariableChangeEventHandler;

	class AnimationVariable : public ComPtrHolder<IUIAnimationVariable2>
	{
		public:
		AnimationVariable() noexcept = default;
		AnimationVariable(ComPtr<IUIAnimationVariable2> ptr) noexcept;

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

		void SetLowerBound(double bound) const;
		void SetLowerBound(std::span<double> bounds) const;
		void SetUpperBound(double bound) const;
		void SetUpperBound(std::span<double> bounds) const;

		void SetRoundingMode(AnimationRoundingMode mode) const;

		void SetTag(ComPtr<IUnknown> obj, UINT32 id) const;
		[[nodiscard]] auto GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>;

		void SetVariableChangeHandler(AnimationVariableChangeEventHandler& handler, bool registerForNext = false) const;
		void ClearVariableChangeHandler(bool registerForNext = false) const;

		template <std::floating_point T>
		explicit(false) operator T() const { return static_cast<T>(GetValue()); }
		template <std::integral T>
		explicit(false) operator T() const { return static_cast<T>(GetIntegerValue()); }

		//TODO SetVariableCurveChangeHandler
	};
}

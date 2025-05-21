module;
#include <wrl.h>
#include <UIAnimation.h>

module PGUI.UI.Animation.AnimationVariable;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.Animation.Storyboard;

namespace  PGUI::UI::Animation
{
	AnimationVariable::AnimationVariable(ComPtr<IUIAnimationVariable2> ptr) noexcept : 
		ComPtrHolder{ ptr }
	{
	}

	auto AnimationVariable::GetStoryboard() const -> Storyboard
	{
		ComPtr<IUIAnimationStoryboard2> storyboard;
		auto hr = Get()->GetCurrentStoryboard(&storyboard); ThrowFailed(hr);

		return storyboard;
	}
	auto AnimationVariable::GetDimension() const -> UINT
	{
		UINT dimension;
		auto hr = Get()->GetDimension(&dimension); ThrowFailed(hr);

		return dimension;
	}

	auto AnimationVariable::GetPreviousIntegerValue() const -> INT32
	{
		INT32 previousValue;
		auto hr = Get()->GetPreviousIntegerValue(&previousValue); ThrowFailed(hr);

		return previousValue;
	}
	auto AnimationVariable::GetPreviousIntegerVectorValue() const -> std::vector<INT32>
	{
		UINT dimension = GetDimension();
		std::vector<INT32> previousValues(dimension);

		auto hr = Get()->GetPreviousIntegerVectorValue(previousValues.data(), dimension); ThrowFailed(hr);

		return previousValues;
	}
	auto AnimationVariable::GetIntegerValue() const -> INT32
	{
		INT32 value;
		auto hr = Get()->GetIntegerValue(&value); ThrowFailed(hr);

		return value;
	}
	auto AnimationVariable::GetIntegerVectorValue() const -> std::vector<INT32>
	{
		UINT dimension = GetDimension();
		std::vector<INT32> values(dimension);

		auto hr = Get()->GetIntegerVectorValue(values.data(), dimension); ThrowFailed(hr);

		return values;
	}
	auto AnimationVariable::GetFinalIntegerValue() const -> INT32
	{
		INT32 finalValue;
		auto hr = Get()->GetFinalIntegerValue(&finalValue); ThrowFailed(hr);

		return finalValue;
	}
	auto AnimationVariable::GetFinalIntegerVectorValue() const -> std::vector<INT32>
	{
		UINT dimension = GetDimension();
		std::vector<INT32> finalValues(dimension);

		auto hr = Get()->GetFinalIntegerVectorValue(finalValues.data(), dimension); ThrowFailed(hr);

		return finalValues;
	}

	auto AnimationVariable::GetPreviousValue() const -> double
	{
		double previousValue;
		auto hr = Get()->GetPreviousValue(&previousValue); ThrowFailed(hr);

		return previousValue;
	}
	auto AnimationVariable::GetPreviousVectorValue() const -> std::vector<double>
	{
		UINT dimension = GetDimension();
		std::vector<double> previousValues(dimension);

		auto hr = Get()->GetPreviousVectorValue(previousValues.data(), dimension); ThrowFailed(hr);

		return previousValues;
	}
	auto AnimationVariable::GetValue() const -> double
	{
		double value;
		auto hr = Get()->GetValue(&value); ThrowFailed(hr);

		return value;
	}
	auto AnimationVariable::GetVectorValue() const -> std::vector<double>
	{
		UINT dimension = GetDimension();
		std::vector<double> values(dimension);

		auto hr = Get()->GetVectorValue(values.data(), dimension); ThrowFailed(hr);

		return values;
	}
	auto AnimationVariable::GetFinalValue() const -> double
	{
		double finalValue;
		auto hr = Get()->GetFinalValue(&finalValue); ThrowFailed(hr);

		return finalValue;
	}
	auto AnimationVariable::GetFinalVectorValue() const -> std::vector<double>
	{
		UINT dimension = GetDimension();
		std::vector<double> finalValues(dimension);

		auto hr = Get()->GetFinalVectorValue(finalValues.data(), dimension); ThrowFailed(hr);

		return finalValues;
	}

	void AnimationVariable::SetLowerBound(double bound) const
	{
		auto hr = Get()->SetLowerBound(bound); ThrowFailed(hr);
	}
	void AnimationVariable::SetLowerBound(std::span<double> bounds) const
	{
		auto hr = Get()->SetLowerBoundVector(bounds.data(), 
			static_cast<UINT>(bounds.size())); ThrowFailed(hr);
	}
	void AnimationVariable::SetUpperBound(double bound) const
	{
		auto hr = Get()->SetUpperBound(bound); ThrowFailed(hr);
	}
	void AnimationVariable::SetUpperBound(std::span<double> bounds) const
	{
		auto hr = Get()->SetUpperBoundVector(bounds.data(),
			static_cast<UINT>(bounds.size())); ThrowFailed(hr);
	}

	void AnimationVariable::SetRoundingMode(AnimationRoundingMode mode) const
	{
		auto hr = Get()->SetRoundingMode(
			static_cast<UI_ANIMATION_ROUNDING_MODE>(mode)); ThrowFailed(hr);
	}

	void AnimationVariable::SetTag(ComPtr<IUnknown> obj, UINT32 id) const
	{
		auto hr = Get()->SetTag(obj.Get(), id); ThrowFailed(hr);
	}
	auto AnimationVariable::GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		auto hr = Get()->GetTag(&obj, &tag); ThrowFailed(hr);

		return { obj, tag };
	}
}

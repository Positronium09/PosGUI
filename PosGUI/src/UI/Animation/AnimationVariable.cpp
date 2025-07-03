module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationVariable;
import :AnimationVariable;

import PGUI.ComPtr;
import PGUI.Exceptions;
import :Storyboard;
import :AnimationVariableChangeEventHandler;

namespace PGUI::UI::Animation
{
	AnimationVariable::AnimationVariable(const ComPtr<IUIAnimationVariable2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{ }

	auto AnimationVariable::GetStoryboard() const -> Storyboard
	{
		ComPtr<IUIAnimationStoryboard2> storyboard;
		const auto hr = Get()->GetCurrentStoryboard(&storyboard);
		ThrowFailed(hr);

		return storyboard;
	}

	auto AnimationVariable::GetDimension() const -> UINT
	{
		UINT dimension;
		const auto hr = Get()->GetDimension(&dimension);
		ThrowFailed(hr);

		return dimension;
	}

	auto AnimationVariable::GetPreviousIntegerValue() const -> INT32
	{
		INT32 previousValue;
		const auto hr = Get()->GetPreviousIntegerValue(&previousValue);
		ThrowFailed(hr);

		return previousValue;
	}

	auto AnimationVariable::GetPreviousIntegerVectorValue() const -> std::vector<INT32>
	{
		const auto dimension = GetDimension();
		std::vector<INT32> previousValues(dimension);

		const auto hr = Get()->GetPreviousIntegerVectorValue(previousValues.data(), dimension);
		ThrowFailed(hr);

		return previousValues;
	}

	auto AnimationVariable::GetIntegerValue() const -> INT32
	{
		INT32 value;
		const auto hr = Get()->GetIntegerValue(&value);
		ThrowFailed(hr);

		return value;
	}

	auto AnimationVariable::GetIntegerVectorValue() const -> std::vector<INT32>
	{
		const auto dimension = GetDimension();
		std::vector<INT32> values(dimension);

		const auto hr = Get()->GetIntegerVectorValue(values.data(), dimension);
		ThrowFailed(hr);

		return values;
	}

	auto AnimationVariable::GetFinalIntegerValue() const -> INT32
	{
		INT32 finalValue;
		const auto hr = Get()->GetFinalIntegerValue(&finalValue);
		ThrowFailed(hr);

		return finalValue;
	}

	auto AnimationVariable::GetFinalIntegerVectorValue() const -> std::vector<INT32>
	{
		const auto dimension = GetDimension();
		std::vector<INT32> finalValues(dimension);

		const auto hr = Get()->GetFinalIntegerVectorValue(finalValues.data(), dimension);
		ThrowFailed(hr);

		return finalValues;
	}

	auto AnimationVariable::GetPreviousValue() const -> double
	{
		double previousValue;
		const auto hr = Get()->GetPreviousValue(&previousValue);
		ThrowFailed(hr);

		return previousValue;
	}

	auto AnimationVariable::GetPreviousVectorValue() const -> std::vector<double>
	{
		const auto dimension = GetDimension();
		std::vector<double> previousValues(dimension);

		const auto hr = Get()->GetPreviousVectorValue(previousValues.data(), dimension);
		ThrowFailed(hr);

		return previousValues;
	}

	auto AnimationVariable::GetValue() const -> double
	{
		double value;
		const auto hr = Get()->GetValue(&value);
		ThrowFailed(hr);

		return value;
	}

	auto AnimationVariable::GetVectorValue() const -> std::vector<double>
	{
		const auto dimension = GetDimension();
		std::vector<double> values(dimension);

		const auto hr = Get()->GetVectorValue(values.data(), dimension);
		ThrowFailed(hr);

		return values;
	}

	auto AnimationVariable::GetFinalValue() const -> double
	{
		double finalValue;
		const auto hr = Get()->GetFinalValue(&finalValue);
		ThrowFailed(hr);

		return finalValue;
	}

	auto AnimationVariable::GetFinalVectorValue() const -> std::vector<double>
	{
		const auto dimension = GetDimension();
		std::vector<double> finalValues(dimension);

		const auto hr = Get()->GetFinalVectorValue(finalValues.data(), dimension);
		ThrowFailed(hr);

		return finalValues;
	}

	auto AnimationVariable::SetLowerBound(const double bound) const -> void
	{
		const auto hr = Get()->SetLowerBound(bound);
		ThrowFailed(hr);
	}

	auto AnimationVariable::SetLowerBound(const std::span<double> bounds) const -> void
	{
		const auto hr = Get()->SetLowerBoundVector(
			bounds.data(),
			static_cast<UINT>(bounds.size()));
		ThrowFailed(hr);
	}

	auto AnimationVariable::SetUpperBound(const double bound) const -> void
	{
		const auto hr = Get()->SetUpperBound(bound);
		ThrowFailed(hr);
	}

	auto AnimationVariable::SetUpperBound(const std::span<double> bounds) const -> void
	{
		const auto hr = Get()->SetUpperBoundVector(
			bounds.data(),
			static_cast<UINT>(bounds.size()));
		ThrowFailed(hr);
	}

	auto AnimationVariable::SetRoundingMode(AnimationRoundingMode mode) const -> void
	{
		const auto hr = Get()->SetRoundingMode(
			static_cast<UI_ANIMATION_ROUNDING_MODE>(mode));
		ThrowFailed(hr);
	}

	auto AnimationVariable::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const -> void
	{
		const auto hr = Get()->SetTag(obj.Get(), id);
		ThrowFailed(hr);
	}

	auto AnimationVariable::GetTag() const -> std::pair<ComPtr<IUnknown>, UINT32>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		const auto hr = Get()->GetTag(&obj, &tag);
		ThrowFailed(hr);

		return { obj, tag };
	}

	auto AnimationVariable::SetVariableChangeHandler(
		AnimationVariableChangeEventHandler& handler,
		const bool registerForNext) const -> void
	{
		const auto& ptr = Get();
		auto hr = ptr->SetVariableChangeHandler(
			&handler.GetRouter(),
			registerForNext
		);
		ThrowFailed(hr);
		hr = ptr->SetVariableIntegerChangeHandler(
			&handler.GetIntegerRouter(),
			registerForNext
		);
		ThrowFailed(hr);
	}

	auto AnimationVariable::ClearVariableChangeHandler(const bool registerForNext) const -> void
	{
		const auto& ptr = Get();
		auto hr = ptr->SetVariableChangeHandler(nullptr, registerForNext);
		ThrowFailed(hr);
		hr = ptr->SetVariableIntegerChangeHandler(nullptr, registerForNext);
		ThrowFailed(hr);
	}
}

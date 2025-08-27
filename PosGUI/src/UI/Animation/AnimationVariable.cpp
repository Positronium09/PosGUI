module;
#include <UIAnimation.h>
#include <wrl.h>

module PGUI.UI.Animation:AnimationVariable;
import :AnimationVariable;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import :Storyboard;
import :AnimationVariableChangeEventHandler;

namespace PGUI::UI::Animation
{
	AnimationVariable::AnimationVariable(const ComPtr<IUIAnimationVariable2>& ptr) noexcept :
		ComPtrHolder{ ptr }
	{ }

	auto AnimationVariable::GetStoryboard() const noexcept -> Result<Storyboard>
	{
		ComPtr<IUIAnimationStoryboard2> storyboard;

		if (const auto hr = Get()->GetCurrentStoryboard(&storyboard);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get current storyboard for animation variable");
			return Unexpected{ error };
		}
	
		return storyboard;
	}

	auto AnimationVariable::GetDimension() const noexcept -> Result<UINT>
	{
		UINT dimension;
		if (const auto hr = Get()->GetDimension(&dimension);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get dimension of animation variable");
			return Unexpected{ error };
		}

		return dimension;
	}

	auto AnimationVariable::GetPreviousIntegerValue() const noexcept -> Result<INT32>
	{
		INT32 previousValue;
		if (const auto hr = Get()->GetPreviousIntegerValue(&previousValue);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get previous integer value of animation variable");
			return Unexpected{ error };
		}

		return previousValue;
	}

	auto AnimationVariable::GetPreviousIntegerVectorValue() const noexcept -> Result<std::vector<INT32>>
	{
		const auto dimension = GetDimension();
		return dimension.and_then([this](const UINT dim) -> Result<std::vector<INT32>>
		{
			std::vector<INT32> previousValues(dim);
			if (const auto hr = Get()->GetPreviousIntegerVectorValue(previousValues.data(), dim);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to get previous integer vector value of animation variable");
				return Unexpected{ error };
			}

			return previousValues;
		});
	}

	auto AnimationVariable::GetIntegerValue() const noexcept -> Result<INT32>
	{
		INT32 value;
		if (const auto hr = Get()->GetIntegerValue(&value);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get integer value of animation variable");
			return Unexpected{ error };
		}

		return value;
	}

	auto AnimationVariable::GetIntegerVectorValue() const noexcept -> Result<std::vector<INT32>>
	{
		const auto dimension = GetDimension();
		return dimension.and_then([this](const UINT dim) -> Result<std::vector<INT32>>
		{
			std::vector<INT32> values(dim);
			if (const auto hr = Get()->GetIntegerVectorValue(values.data(), dim);
				FAILED(hr))
			{
				Error error{ hr };
				error.AddDetail(L"Dimension", std::to_wstring(dim));
				Logger::Error(error, L"Failed to get integer vector value of animation variable");
				return Unexpected{ error };
			}
			return values;
		});
	}

	auto AnimationVariable::GetFinalIntegerValue() const noexcept -> Result<INT32>
	{
		INT32 finalValue;
		if (const auto hr = Get()->GetFinalIntegerValue(&finalValue);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get final integer value of animation variable");
			return Unexpected{ error };
		}

		return finalValue;
	}

	auto AnimationVariable::GetFinalIntegerVectorValue() const noexcept -> Result<std::vector<INT32>>
	{
		const auto dimension = GetDimension();
		return dimension.and_then([this](const UINT dim) -> Result<std::vector<INT32>>
		{
			std::vector<INT32> finalValues(dim);
			if (const auto hr = Get()->GetFinalIntegerVectorValue(finalValues.data(), dim);
				FAILED(hr))
			{
				Error error{ hr };
				error.AddDetail(L"Dimension", std::to_wstring(dim));
				Logger::Error(error, L"Failed to get final integer vector value of animation variable");
				return Unexpected{ error };
			}
			return finalValues;
		});
	}

	auto AnimationVariable::GetPreviousValue() const noexcept -> Result<double>
	{
		double previousValue;
		if (const auto hr = Get()->GetPreviousValue(&previousValue);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get previous value of animation variable");
			return Unexpected{ error };
		}

		return previousValue;
	}

	auto AnimationVariable::GetPreviousVectorValue() const noexcept -> Result<std::vector<double>>
	{
		const auto dimension = GetDimension();
		return dimension.and_then([this](const UINT dim) -> Result<std::vector<double>>
		{
			std::vector<double> previousValues(dim);
			if (const auto hr = Get()->GetPreviousVectorValue(previousValues.data(), dim);
				FAILED(hr))
			{
				Error error{ hr };
				error.AddDetail(L"Dimension", std::to_wstring(dim));
				Logger::Error(error, L"Failed to get previous vector value of animation variable");
				return Unexpected{ error };
			}
			return previousValues;
		});
	}

	auto AnimationVariable::GetValue() const noexcept -> Result<double>
	{
		double value;
		if (const auto hr = Get()->GetValue(&value);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get value of animation variable");
			return Unexpected{ error };
		}

		return value;
	}

	auto AnimationVariable::GetVectorValue() const noexcept -> Result<std::vector<double>>
	{
		const auto dimension = GetDimension();
		return dimension.and_then([this](const UINT dim) -> Result<std::vector<double>>
		{
			std::vector<double> values(dim);
			if (const auto hr = Get()->GetVectorValue(values.data(), dim);
				FAILED(hr))
			{
				Error error{ hr };
				error.AddDetail(L"Dimension", std::to_wstring(dim));
				Logger::Error(error, L"Failed to get vector value of animation variable");
				return Unexpected{ error };
			}
			return values;
		});
	}

	auto AnimationVariable::GetFinalValue() const noexcept -> Result<double>
	{
		double finalValue;
		if (const auto hr = Get()->GetFinalValue(&finalValue);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get final value of animation variable");
			return Unexpected{ error };
		}

		return finalValue;
	}

	auto AnimationVariable::GetFinalVectorValue() const noexcept -> Result<std::vector<double>>
	{
		const auto dimension = GetDimension();
		return dimension.and_then([this](const UINT dim) -> Result<std::vector<double>>
		{
			std::vector<double> finalValues(dim);
			if (const auto hr = Get()->GetFinalVectorValue(finalValues.data(), dim);
				FAILED(hr))
			{
				Error error{ hr };
				error.AddDetail(L"Dimension", std::to_wstring(dim));
				Logger::Error(error, L"Failed to get final vector value of animation variable");
				return Unexpected{ error };
			}
			return finalValues;
		});
	}

	auto AnimationVariable::SetLowerBound(const double bound) const noexcept -> Error
	{
		Error error{
			Get()->SetLowerBound(bound)
		};
		LogIfFailed(error, L"Failed to set lower bound");
		return error;
	}

	auto AnimationVariable::SetLowerBound(const std::span<double> bounds) const noexcept -> Error
	{
		Error error{
			Get()->SetLowerBoundVector(
				bounds.data(),
				static_cast<UINT>(bounds.size()))
		};
		LogIfFailed(error, L"Failed to set lower bound vector");
		return error;
	}

	auto AnimationVariable::SetUpperBound(const double bound) const noexcept -> Error
	{
		Error error{
			Get()->SetUpperBound(bound)
		};
		LogIfFailed(error, L"Failed to set upper bound");
		return error;
	}

	auto AnimationVariable::SetUpperBound(const std::span<double> bounds) const noexcept -> Error
	{
		Error error{
			Get()->SetUpperBoundVector(
				bounds.data(),
				static_cast<UINT>(bounds.size()))
		};
		LogIfFailed(error, L"Failed to set upper bound vector");
		return error;
	}

	auto AnimationVariable::SetRoundingMode(AnimationRoundingMode mode) const noexcept -> Error
	{
		Error error{
			Get()->SetRoundingMode(
				static_cast<UI_ANIMATION_ROUNDING_MODE>(mode))
		};
		LogIfFailed(error, L"Failed to set rounding mode");
		return error;
	}

	auto AnimationVariable::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Error
	{
		Error error{
			Get()->SetTag(obj.Get(), id)
		};
		LogIfFailed(error, L"Failed to set tag");
		return error;
	}

	auto AnimationVariable::GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		if (const auto hr = Get()->GetTag(&obj, &tag);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to get tag");
			return Unexpected{ error };
		}

		return std::make_pair(obj, tag);
	}

	auto AnimationVariable::SetVariableChangeHandler(
		AnimationVariableChangeEventHandler& handler,
		const bool registerForNext) const noexcept -> Error
	{
		const auto& ptr = Get();

		if (const auto hr = ptr->SetVariableChangeHandler(
			&handler.GetRouter(),
			registerForNext);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to set variable change handler");
			return error;
		}

		if (const auto hr = ptr->SetVariableIntegerChangeHandler(
			&handler.GetIntegerRouter(),
			registerForNext);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to set variable integer change handler");
			return error;
		}

		return Error{ GeneralErrorCodes::Success };
	}

	auto AnimationVariable::ClearVariableChangeHandler(const bool registerForNext) const noexcept -> Error
	{
		const auto& ptr = Get();

		if (const auto hr = ptr->SetVariableChangeHandler(nullptr, registerForNext);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to clear variable change handler");
			return error;
		}

		if (const auto hr = ptr->SetVariableIntegerChangeHandler(nullptr, registerForNext);
			FAILED(hr))
		{
			Error error{ hr };
			Logger::Error(error, L"Failed to clear variable integer change handler");
			return error;
		}

		return Error{ GeneralErrorCodes::Success };
	}
}

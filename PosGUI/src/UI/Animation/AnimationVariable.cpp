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
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}
	
		return storyboard;
	}

	auto AnimationVariable::GetDimension() const noexcept -> Result<UINT>
	{
		UINT dimension;
		if (const auto hr = Get()->GetDimension(&dimension);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return dimension;
	}

	auto AnimationVariable::GetPreviousIntegerValue() const noexcept -> Result<INT32>
	{
		INT32 previousValue;
		if (const auto hr = Get()->GetPreviousIntegerValue(&previousValue);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Animation)
				};
			}

			return previousValues;
		}).or_else([this](const Error& err) -> Result<std::vector<INT32>>
		{
			return Unexpected{ err };
		});
	}

	auto AnimationVariable::GetIntegerValue() const noexcept -> Result<INT32>
	{
		INT32 value;
		if (const auto hr = Get()->GetIntegerValue(&value);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Animation)
				};
			}
			return values;
		}).or_else([this](const Error& err) -> Result<std::vector<INT32>>
		{
			return Unexpected{ err };
		});
	}

	auto AnimationVariable::GetFinalIntegerValue() const noexcept -> Result<INT32>
	{
		INT32 finalValue;
		if (const auto hr = Get()->GetFinalIntegerValue(&finalValue);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Animation)
				};
			}
			return finalValues;
		}).or_else([this](const Error& err) -> Result<std::vector<INT32>>
		{
			return Unexpected{ err };
		});
	}

	auto AnimationVariable::GetPreviousValue() const noexcept -> Result<double>
	{
		double previousValue;
		if (const auto hr = Get()->GetPreviousValue(&previousValue);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Animation)
				};
			}
			return previousValues;
		}).or_else([this](const Error& err) -> Result<std::vector<double>>
		{
			return Unexpected{ err };
		});
	}

	auto AnimationVariable::GetValue() const noexcept -> Result<double>
	{
		double value;
		if (const auto hr = Get()->GetValue(&value);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Animation)
				};
			}
			return values;
		}).or_else([this](const Error& err) -> Result<std::vector<double>>
		{
			return Unexpected{ err };
		});
	}

	auto AnimationVariable::GetFinalValue() const noexcept -> Result<double>
	{
		double finalValue;
		if (const auto hr = Get()->GetFinalValue(&finalValue);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
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
				return Unexpected{
					Error{ hr }
					.AddTag(ErrorTags::Animation)
				};
			}
			return finalValues;
		}).or_else([this](const Error& err) -> Result<std::vector<double>>
		{
			return Unexpected{ err };
		});
	}

	auto AnimationVariable::SetLowerBound(const double bound) const noexcept -> Error
	{
		Error error{
			Get()->SetLowerBound(bound)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Failed to set lower bound");
		}

		return error;
	}

	auto AnimationVariable::SetLowerBound(const std::span<double> bounds) const noexcept -> Error
	{
		Error error{
			Get()->SetLowerBoundVector(
				bounds.data(),
				static_cast<UINT>(bounds.size()))
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Failed to set lower bound");
		}

		return error;
	}

	auto AnimationVariable::SetUpperBound(const double bound) const noexcept -> Error
	{
		Error error{
			Get()->SetUpperBound(bound)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Failed to set upper bound");
		}

		return error;
	}

	auto AnimationVariable::SetUpperBound(const std::span<double> bounds) const noexcept -> Error
	{
		Error error{
			Get()->SetUpperBoundVector(
				bounds.data(),
				static_cast<UINT>(bounds.size()))
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Failed to set upper bound");
		}

		return error;
	}

	auto AnimationVariable::SetRoundingMode(AnimationRoundingMode mode) const noexcept -> Error
	{
		Error error{
			Get()->SetRoundingMode(
				static_cast<UI_ANIMATION_ROUNDING_MODE>(mode))
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Failed to set rounding mode");
		}

		return error;
	}

	auto AnimationVariable::SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Error
	{
		Error error{
			Get()->SetTag(obj.Get(), id)
		};
		error.AddTag(ErrorTags::Animation);

		if (error.IsFailure())
		{
			Logger::Error(L"Failed to set tag");
		}

		return error;
	}

	auto AnimationVariable::GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>
	{
		ComPtr<IUnknown> obj;
		UINT32 tag;
		if (const auto hr = Get()->GetTag(&obj, &tag);
			FAILED(hr))
		{
			return Unexpected{
				Error{ hr }
				.AddTag(ErrorTags::Animation)
			};
		}

		return std::make_pair(obj, tag);
	}

	auto AnimationVariable::SetVariableChangeHandler(
		AnimationVariableChangeEventHandler& handler,
		const bool registerForNext) const noexcept -> Error
	{
		const auto& ptr = Get();

		auto hr = ptr->SetVariableChangeHandler(
			&handler.GetRouter(),
			registerForNext
		);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to set variable change handler");
			return Error{ hr }
				.AddTag(ErrorTags::Animation);
		}

		hr = ptr->SetVariableIntegerChangeHandler(
			&handler.GetIntegerRouter(),
			registerForNext
		);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to set variable integer change handler");
			return Error{ hr }
				.AddTag(ErrorTags::Animation);
		}

		return Error{ S_OK }.AddTag(ErrorTags::Animation);
	}

	auto AnimationVariable::ClearVariableChangeHandler(const bool registerForNext) const noexcept -> Error
	{
		const auto& ptr = Get();

		auto hr = ptr->SetVariableChangeHandler(nullptr, registerForNext);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to clear variable change handler");
			return Error{ hr }
				.AddTag(ErrorTags::Animation);
		}

		hr = ptr->SetVariableIntegerChangeHandler(nullptr, registerForNext);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to clear variable integer change handler");
			return Error{ hr }
				.AddTag(ErrorTags::Animation);
		}

		return Error{ S_OK }.AddTag(ErrorTags::Animation);
	}
}

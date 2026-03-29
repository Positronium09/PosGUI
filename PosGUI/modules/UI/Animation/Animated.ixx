export module PGUI.UI.Animation:Animated;

import :AnimationInterface;
import :AnimationEnums;
import :AnimationTimeTypes;
import :Storyboard;
import :AnimationTransition;
import :AnimationTransitionLibrary;
import :AnimationTimer;
import :AnimationVariable;
import :AnimationVariableChangeEventHandler;
import :AnimationManager;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;
import PGUI.UI.Color;
import PGUI.Shape2D;

export namespace PGUI::UI::Animation
{
	template <typename Converter, std::size_t N>
	concept AnimatedConverter = N > 1 && requires
	{
		{ Converter::ConvertToValue(std::declval<std::span<const double, N>>()) } -> NotSameAs<void>;
		{ Converter::ConvertFromValue(Converter::ConvertToValue(std::declval<std::span<const double, N>>())) } -> std::same_as<std::array<double, N>>;
	};

	namespace Converters
	{
		template <typename T, std::size_t N> requires (std::floating_point<T> || std::integral<T>) && (N > 1ULL)
			struct DefaultConverter
		{
			static auto ConvertToValue(const std::span<const double, N>& values) -> std::array<T, N>
			{
				std::array<T, N> result{ };

				for (std::size_t i = 0; i < N; ++i)
				{
					result[i] = static_cast<T>(values[i]);
				}

				return result;
			}

			static auto ConvertFromValue(const std::array<T, N>& value) -> std::array<double, N>
			{
				std::array<double, N> result{ };

				for (std::size_t i = 0; i < N; ++i)
				{
					result[i] = static_cast<double>(value[i]);
				}

				return result;
			}
		};

		struct RGBAConverter
		{
			static auto ConvertToValue(const std::span<const double, 4>& values) -> RGBA
			{
				return RGBA{
					static_cast<float>(values[0]),
					static_cast<float>(values[1]),
					static_cast<float>(values[2]),
					static_cast<float>(values[3])
				};
			}
			static auto ConvertFromValue(const RGBA& color) -> std::array<double, 4>
			{
				return std::array{
					static_cast<double>(color.r),
					static_cast<double>(color.g),
					static_cast<double>(color.b),
					static_cast<double>(color.a)
				};
			}
		};

		struct sRGBConverter
		{
			static auto ConvertToValue(const std::span<const double, 3>& values) -> sRGB
			{
				return sRGB{
					static_cast<float>(values[0]),
					static_cast<float>(values[1]),
					static_cast<float>(values[2])
				};
			}
			static auto ConvertFromValue(const sRGB& color) -> std::array<double, 3>
			{
				return std::array{
					static_cast<double>(color.r),
					static_cast<double>(color.g),
					static_cast<double>(color.b)
				};
			}
		};

		struct HSLConverter
		{
			static auto ConvertToValue(const std::span<const double, 3>& values) -> HSL
			{
				return HSL{
					static_cast<float>(values[0]),
					static_cast<float>(values[1]),
					static_cast<float>(values[2])
				};
			}
			static auto ConvertFromValue(const HSL& color) -> std::array<double, 3>
			{
				return std::array{
					static_cast<double>(color.h),
					static_cast<double>(color.s),
					static_cast<double>(color.l)
				};
			}
		};

		struct HSVConverter
		{
			static auto ConvertToValue(const std::span<const double, 3>& values) -> HSV
			{
				return HSV{
					static_cast<float>(values[0]),
					static_cast<float>(values[1]),
					static_cast<float>(values[2])
				};
			}
			static auto ConvertFromValue(const HSV& color) -> std::array<double, 3>
			{
				return std::array{
					static_cast<double>(color.h),
					static_cast<double>(color.s),
					static_cast<double>(color.v)
				};
			}
		};

		struct PointFConverter
		{
			static auto ConvertToValue(const std::span<const double, 2>& values) -> PointF
			{
				return PointF{
					static_cast<float>(values[0]),
					static_cast<float>(values[1])
				};
			}
			static auto ConvertFromValue(const PointF& point) -> std::array<double, 2>
			{
				return std::array{
					static_cast<double>(point.x),
					static_cast<double>(point.y)
				};
			}
		};

		struct SizeFConverter
		{
			static auto ConvertToValue(const std::span<const double, 2>& values) -> SizeF
			{
				return SizeF{
					static_cast<float>(values[0]),
					static_cast<float>(values[1])
				};
			}
			static auto ConvertFromValue(const SizeF& size) -> std::array<double, 2>
			{
				return std::array{
					static_cast<double>(size.cx),
					static_cast<double>(size.cy)
				};
			}
		};

		struct RectFConverter
		{
			static auto ConvertToValue(const std::span<const double, 4>& values) -> RectF
			{
				return RectF{
					static_cast<float>(values[0]),
					static_cast<float>(values[1]),
					static_cast<float>(values[2]),
					static_cast<float>(values[3])
				};
			}
			static auto ConvertFromValue(const RectF& rect) -> std::array<double, 4>
			{
				return std::array{
					static_cast<double>(rect.left),
					static_cast<double>(rect.top),
					static_cast<double>(rect.right),
					static_cast<double>(rect.bottom)
				};
			}
		};
	}

	struct AnimateResult
	{
		Storyboard storyboard;
		AnimationSchedulingResult schedulingResult;
	};

	template <typename, std::size_t, typename = void>
	class Animated;

	template <typename T> requires std::floating_point<T> || std::integral<T>
	class Animated<T, 1>
	{
		public:
		explicit Animated(const T initialValue, 
			const std::optional<AnimationManager>& manager = std::nullopt) noexcept
			: currentManager{ manager }
		{
			const auto& animationManager = GetAnimationManager();

			if (const auto result = animationManager.CreateAnimationVariable(static_cast<double>(initialValue)); 
				result.has_value())
			{
				variable = result.value();
				if (auto error = variable.SetVariableChangeHandler(animationVariableChangeEvent, true);
					error.IsFailure())
				{
					Logger::Error(error, L"Failed to set variable change handler");
				}
				if constexpr (std::floating_point<T>)
				{
					animationVariableChangeEvent.VariableChanged().AddCallback(
						[this](const Storyboard&, const AnimationVariable&,
							std::span<const double> newValues, std::span<const double> oldValues)
						{
							valueChangedEvent.Invoke(static_cast<T>(newValues[0]), static_cast<T>(oldValues[0]));
						});
				}
				else if constexpr (std::integral<T>)
				{
					animationVariableChangeEvent.VariableIntegerChanged().AddCallback(
						[this](const Storyboard&, const AnimationVariable&,
							std::span<const INT32> newValues, std::span<const INT32> oldValues)
						{
							valueChangedEvent.Invoke(static_cast<T>(newValues[0]), static_cast<T>(oldValues[0]));
						});
				}
			}
			else
			{
				Logger::Error(L"Failed to create animation variable: {}", result.error());
			}
		}

		auto TransitionTo(const T& targetValue, TransitionParameters transitionParameters) const noexcept -> Error
		{
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, static_cast<double>(targetValue));
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			const auto& transition = transitionResult.value();
			const auto& animationManager = GetAnimationManager();
			const auto& timer = AnimationTimer::GetGlobalInstance();
			auto nowResult = timer.GetTime();
			if (!nowResult.has_value())
			{
				return nowResult.error();
			}

			return animationManager.ScheduleTransition(variable, transition, nowResult.value());
		}

		auto AnimateTo(const T& targetValue, TransitionParameters transitionParameters) const noexcept -> Result<AnimateResult>
		{
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, static_cast<double>(targetValue));
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return Unexpected{ transitionResult.error() };
			}

			const auto& transition = transitionResult.value();
			const auto& animationManager = GetAnimationManager();

			const auto storyboardResult = animationManager.CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			if (auto error = storyboard.AddTransition(variable, transition);
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			const auto& timer = AnimationTimer::GetGlobalInstance();
			auto nowResult = timer.GetTime();
			if (!nowResult.has_value())
			{
				return Unexpected{ nowResult.error() };
			}

			auto schedulingResult = storyboard.Schedule(nowResult.value());
			if (!schedulingResult.has_value())
			{
				return Unexpected{ schedulingResult.error() };
			}

			return AnimateResult{ .storyboard = storyboard, .schedulingResult = schedulingResult.value() };
		}

		auto AnimateTo(const T& targetValue, TransitionParameters transitionParameters,
			const Storyboard& storyboard) const noexcept -> Error
		{
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, static_cast<double>(targetValue));
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			return storyboard.AddTransition(variable, transitionResult.value());
		}

		auto AnimateToAtKeyframe(const T& targetValue, TransitionParameters transitionParameters,
			const Storyboard& storyboard, const KeyFrame keyFrame) const noexcept -> Error
		{
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, static_cast<double>(targetValue));
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			return storyboard.AddTransitionAtKeyframe(variable, transitionResult.value(), keyFrame);
		}

		auto AnimateToBetweenKeyframes(const T& targetValue, TransitionParameters transitionParameters,
			const Storyboard& storyboard, const KeyFrame startKeyFrame, const KeyFrame endKeyFrame) const noexcept -> Error
		{
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, static_cast<double>(targetValue));
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			return storyboard.AddTransitionBetweenKeyframes(variable, transitionResult.value(), startKeyFrame, endKeyFrame);
		}

		[[nodiscard]] auto GetFloatingValue() const noexcept -> Result<T>
			requires std::floating_point<T>
		{
			return variable.GetValue().transform([](const double& val)
			{
				return static_cast<T>(val);
			});
		}

		[[nodiscard]] auto GetIntegerValue() const noexcept -> Result<T>
		{
			return variable.GetIntegerValue().transform([](const INT32& val)
			{
				return static_cast<T>(val);
			});
		}

		[[nodiscard]] auto GetValue() const noexcept -> Result<T>
		{
			if constexpr (std::floating_point<T>)
			{
				return GetFloatingValue();
			}
			else if constexpr (std::integral<T>)
			{
				return GetIntegerValue();
			}
			else
			{
				static_assert(false, "Unsupported Type");
				std::unreachable();
			}
		}

		[[nodiscard]] auto GetPreviousFloatingValue() const noexcept -> Result<T>
			requires std::floating_point<T>
		{
			return variable.GetPreviousValue().transform([](const double& val)
			{
				return static_cast<T>(val);
			});
		}

		[[nodiscard]] auto GetPreviousIntegerValue() const noexcept -> Result<T>
		{
			return variable.GetPreviousIntegerValue().transform([](const INT32& val)
			{
				return static_cast<T>(val);
			});
		}

		[[nodiscard]] auto GetPreviousValue() const noexcept -> Result<T>
		{
			if constexpr (std::floating_point<T>)
			{
				return GetPreviousFloatingValue();
			}
			else if constexpr (std::integral<T>)
			{
				return GetPreviousIntegerValue();
			}
			else
			{
				static_assert(false, "Unsupported Type");
				std::unreachable();
			}
		}

		auto SetLowerBound(const T& bound) const noexcept -> Error
		{
			return variable.SetLowerBound(static_cast<double>(bound));
		}

		auto SetUpperBound(const T& bound) const noexcept -> Error
		{
			return variable.SetUpperBound(static_cast<double>(bound));
		}

		auto SetBounds(const T& lowerBound, const T& upperBound) const noexcept -> Error
		{
			if (auto error = SetLowerBound(lowerBound);
				error.IsFailure())
			{
				return error;
			}
			return SetUpperBound(upperBound);
		}

		auto SetRoundingMode(const AnimationRoundingMode mode) const noexcept -> Error
		{
			return variable.SetRoundingMode(mode);
		}

		auto SetTag(const ComPtr<IUnknown>& obj, const UINT32 id) const noexcept -> Error
		{
			return variable.SetTag(obj, id);
		}

		[[nodiscard]] auto GetTag() const noexcept -> Result<std::pair<ComPtr<IUnknown>, UINT32>>
		{
			return variable.GetTag();
		}

		[[nodiscard]] auto Get() const noexcept -> Result<T>
		{
			return GetValue();
		}

		[[nodiscard]] explicit operator T() const
		{
			const auto result = GetValue();
			if (!result.has_value())
			{
				throw Exception{ result.error() };
			}

			return result.value();
		}

		[[nodiscard]] const auto& GetAnimationManager() const noexcept
		{
			if (currentManager.has_value())
			{
				return currentManager.value();
			}

			return AnimationManager::GetGlobalInstance();
		}

		[[nodiscard]] auto IsInitialized() const noexcept
		{
			return variable.IsInitialized();
		}

		template <typename Self>
		[[nodiscard]] auto&& ValueChangedEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.valueChangedEvent);
		}

		private:
		AnimationVariable variable;
		AnimationVariableChangeEvent animationVariableChangeEvent;
		EventSRWM<T, T> valueChangedEvent;
		std::optional<AnimationManager> currentManager;
	};

	template <typename T, std::size_t N, AnimatedConverter<N> Converter>
	requires (N > 1ULL)
	class Animated<T, N, Converter>
	{
		public:
		explicit Animated(const T& initialValue, const std::optional<AnimationManager>& manager = std::nullopt) noexcept
			: currentManager{ manager }
		{
			const auto& animationManager = GetAnimationManager();
			const auto initialArray = Converter::ConvertFromValue(initialValue);
			if (const auto result = animationManager.CreateAnimationVariable(initialArray);
				result.has_value())
			{
				variable = result.value();
				if (auto error = variable.SetVariableChangeHandler(animationVariableChangeEvent, true);
					error.IsFailure())
				{
					Logger::Error(error, L"Failed to set variable change handler");
					return;
				}
				animationVariableChangeEvent.VariableChanged().AddCallback(
					[this](const Storyboard&, const AnimationVariable&,
						std::span<const double> newValues, std::span<const double> oldValues)
					{
						valueChangedEvent.Invoke(
							Converter::ConvertToValue(std::span<const double, N>{newValues}), 
							Converter::ConvertToValue(std::span<const double, N>{oldValues}));
				});
			}
			else
			{
				Logger::Error(L"Failed to create animation variable: {}", result.error());
			}
		}

		auto TransitionTo(const T& targetValue, TransitionParameters transitionParameters) const noexcept -> Error
		{
			const auto targetArray = Converter::ConvertFromValue(targetValue);
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, targetArray);
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			const auto& transition = transitionResult.value();
			const auto& animationManager = GetAnimationManager();
			const auto& timer = AnimationTimer::GetGlobalInstance();

			auto nowResult = timer.GetTime();
			if (!nowResult.has_value())
			{
				return nowResult.error();
			}

			return animationManager.ScheduleTransition(variable, transition, nowResult.value());
		}

		[[nodiscard]] auto AnimateTo(const T& targetValue, TransitionParameters transitionParameters) const noexcept -> Result<AnimateResult>
		{
			const auto targetArray = Converter::ConvertFromValue(targetValue);
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, targetArray);
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return Unexpected{ transitionResult.error() };
			}

			const auto& transition = transitionResult.value();
			const auto& animationManager = GetAnimationManager();
			const auto storyboardResult = animationManager.CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return Unexpected{ storyboardResult.error() };
			}

			const auto& storyboard = storyboardResult.value();
			if (auto error = storyboard.AddTransition(variable, transition);
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			const auto& timer = AnimationTimer::GetGlobalInstance();

			auto nowResult = timer.GetTime();
			if (!nowResult.has_value())
			{
				return Unexpected{ nowResult.error() };
			}

			auto schedulingResult = storyboard.Schedule(nowResult.value());
			if (!schedulingResult.has_value())
			{
				return Unexpected{ schedulingResult.error() };
			}

			return AnimateResult{ .storyboard = storyboard, .schedulingResult = schedulingResult.value() };
		}

		auto AnimateTo(const T& targetValue, TransitionParameters transitionParameters,
			const Storyboard& storyboard) const noexcept -> Error
		{
			const auto targetArray = Converter::ConvertFromValue(targetValue);
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, targetArray);
				error.IsFailure())
			{
				return error;
			}
			
			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			return storyboard.AddTransition(variable, transitionResult.value());
		}

		auto AnimateToAtKeyframe(const T& targetValue, TransitionParameters transitionParameters,
			const Storyboard& storyboard, const KeyFrame keyFrame) const noexcept -> Error
		{
			const auto targetArray = Converter::ConvertFromValue(targetValue);
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, targetArray);
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			return storyboard.AddTransitionAtKeyframe(variable, transitionResult.value(), keyFrame);
		}

		auto AnimateToBetweenKeyframes(const T& targetValue, TransitionParameters transitionParameters,
			const Storyboard& storyboard, const KeyFrame startKeyFrame, const KeyFrame endKeyFrame) const noexcept -> Error
		{
			const auto targetArray = Converter::ConvertFromValue(targetValue);
			if (auto error = SetFinalValueToTransitionParameters(transitionParameters, targetArray);
				error.IsFailure())
			{
				return error;
			}

			auto transitionResult = AnimationTransitionLibrary::TransitionFromParameters(transitionParameters);
			if (!transitionResult.has_value())
			{
				return transitionResult.error();
			}

			return storyboard.AddTransitionBetweenKeyframes(variable, transitionResult.value(), startKeyFrame, endKeyFrame);
		}

		[[nodiscard]] auto GetValue() const noexcept -> Result<T>
		{
			auto valueResult = variable.GetVectorValue();
			if (!valueResult.has_value())
			{
				return Unexpected{ valueResult.error() };
			}
			if (valueResult.value().size() != N)
			{
				return Unexpected{ Error{
						ErrorCode::InvalidArgument
					}.SetCustomMessage(L"Received vector of unexpected size")
				};
			}

			return Converter::ConvertToValue(std::span<const double, N>{ valueResult.value() });
		}

		[[nodiscard]] auto GetPreviousValue() const noexcept -> Result<T>
		{
			auto valueResult = variable.GetPreviousVectorValue();
			if (!valueResult.has_value())
			{
				return Unexpected{ valueResult.error() };
			}
			
			if (valueResult.value().size() != N)
			{
				return Unexpected{ Error{
						ErrorCode::InvalidArgument
					}.SetCustomMessage(L"Received vector of unexpected size")
				};
			}

			return Converter::ConvertToValue(std::span<const double, N>{ valueResult.value() });
		}

		[[nodiscard]] auto Get() const noexcept -> Result<T>
		{
			return GetValue();
		}

		[[nodiscard]] explicit operator T() const
		{
			const auto result = GetValue();
			if (!result.has_value())
			{
				throw Exception{ result.error() };
			}
			return result.value();
		}

		auto SetUpperBound(const T& bound) const noexcept -> Error
		{
			const auto boundArray = Converter::ConvertFromValue(bound);
			return variable.SetUpperBound(boundArray);
		}
		
		auto SetLowerBound(const T& bound) const noexcept -> Error
		{
			const auto boundArray = Converter::ConvertFromValue(bound);
			return variable.SetLowerBound(boundArray);
		}

		auto SetBounds(const T& lowerBound, const T& upperBound) const noexcept -> Error
		{
			if (auto error = SetLowerBound(lowerBound);
				error.IsFailure())
			{
				return error;
			}
			return SetUpperBound(upperBound);
		}

		[[nodiscard]] const auto& GetAnimationManager() const noexcept
		{
			if (currentManager.has_value())
			{
				return currentManager.value();
			}

			return AnimationManager::GetGlobalInstance();
		}

		[[nodiscard]] auto IsInitialized() const noexcept
		{
			return variable.IsInitialized();
		}

		template <typename Self>
		[[nodiscard]] auto&& ValueChangedEvent(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.valueChangedEvent);
		}

		private:
		AnimationVariable variable;
		AnimationVariableChangeEvent animationVariableChangeEvent;
		EventSRWM<const T&, const T&> valueChangedEvent;
		std::optional<AnimationManager> currentManager;
	};

	template <typename T, std::size_t N> requires (N > 1ULL)
	class Animated<T, N> : public Animated<T, N, Converters::DefaultConverter<T, N>>
	{
		public:
		using Animated<T, N, Converters::DefaultConverter<T, N>>::Animated;
	};

	using AnimatedFloat = Animated<float, 1>;
	using AnimatedDouble = Animated<double, 1>;
	using AnimatedInt = Animated<int, 1>;
	using AnimatedUInt = Animated<unsigned int, 1>;
	using AnimatedLong = Animated<long, 1>;
	using AnimatedULong = Animated<unsigned long, 1>;
	using AnimatedLLong = Animated<long long, 1>;
	// ReSharper disable once CppInconsistentNaming

	using AnimatedULLong = Animated<unsigned long long, 1>;
	using AnimatedRGBA = Animated<RGBA, 4, Converters::RGBAConverter>;
	// ReSharper disable once IdentifierTypo

	using AnimatedsRGB = Animated<sRGB, 3, Converters::sRGBConverter>;
	using AnimatedHSL = Animated<HSL, 3, Converters::HSLConverter>;
	using AnimatedHSV = Animated<HSV, 3, Converters::HSVConverter>;
	using AnimatedPointF = Animated<PointF, 2, Converters::PointFConverter>;
	using AnimatedSizeF = Animated<SizeF, 2, Converters::SizeFConverter>;
	using AnimatedRectF = Animated<RectF, 4, Converters::RectFConverter>;
}

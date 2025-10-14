export module PGUI.UI.Animation:Animated;

import :AnimationEnums;
import :Storyboard;
import :AnimationTransition;
import :AnimationTransitionLibrary;
import :AnimationTimer;
import :AnimationVariable;
import :AnimationManager;

import std;

import PGUI.ErrorHandling;
import PGUI.UI.Color;
import PGUI.Shape2D;

/*
	class Animated
	{
		public:
		explicit Animated(const float f)
		{
			var = AnimationManager::GetGlobalInstance().CreateAnimationVariable(f).value();
		}

		auto AnimateTo(float f) const -> void
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				throw std::runtime_error("Failed to create storyboard");
			}
			const auto& storyboard = storyboardResult.value();
			auto res = storyboard.AddTransition(var, AnimationTransitionLibrary::LinearTransition(1.0F, f).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			(void)storyboard.Schedule(timer.GetTime().value());
		}

		auto Get() const noexcept
		{
			return static_cast<float>(*var.GetValue());
		}

		private:
		AnimationVariable var;
	};
 */

export namespace PGUI::UI::Animation
{
	template <typename, std::size_t = 1>
	class Animated;

	template <typename T> requires std::is_arithmetic_v<T>
	class Animated<T, 1>
	{
		public:
		explicit Animated(const T& initialValue, 
			const std::optional<AnimationManager>& animationManager = std::nullopt) :
			customAnimationManager{ animationManager }
		{
			const auto& manager = GetAnimationManager();

			const auto varResult = manager.CreateAnimationVariable(initialValue);
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const T& newValue, const double duration = 1.0) const noexcept -> Error
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(duration, newValue).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();

			if (const auto scheduleResult = storyboard.Schedule(timer.GetTime().value());
				!scheduleResult.has_value())
			{
				return scheduleResult.error();
			}

			return Error{ ErrorCode::Success };
		}

		auto Get() const noexcept
		{
			if constexpr (std::is_integral_v<T>)
			{
				return var.GetIntegerValue();
			}
			else
			{
				return var.GetValue();
			}
		}

		explicit operator T() const
		{
			if constexpr (std::is_integral_v<T>)
			{
				const auto valResult = var.GetIntegerValue();
				if (!valResult.has_value())
				{
					throw Exception{ valResult.error() };
				}
				return *valResult;
			}
			else
			{
				const auto valResult = var.GetValue();
				if (!valResult.has_value())
				{
					throw Exception{ valResult.error() };
				}
				return *valResult;
			}
		}

		auto GetIntegerValue() const
		{
			const auto valResult = var.GetIntegerValue();
			if (!valResult.has_value())
			{
				throw Exception{ valResult.error() };
			}
			return *valResult;
		}

		auto GetFloatingValue() const
		{
			const auto valResult = var.GetValue();
			if (!valResult.has_value())
			{
				throw Exception{ valResult.error() };
			}
			return *valResult;
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};

	template <typename T, std::size_t Count> requires std::is_arithmetic_v<T> && (Count > 1)
	class Animated<T, Count>
	{
		public:
		explicit Animated(
			const std::span<const T, Count> initialValue,
			const std::optional<AnimationManager>& animationManager = std::nullopt) :
			customAnimationManager{ animationManager }
		{
			const auto& manager = GetAnimationManager();
			const auto varResult = manager.CreateAnimationVariable(
				initialValue | std::ranges::to<const std::span<const double>>());
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const std::span<const T, Count> newValue, const double duration = 1.0) const noexcept -> Error
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(
					duration, 
					newValue | std::ranges::to<const std::span<const double>>()).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			if (const auto scheduleResult = storyboard.Schedule(timer.GetTime().value());
				!scheduleResult.has_value())
			{
				return scheduleResult.error();
			}
			return Error{ ErrorCode::Success };
		}

		auto Get() const noexcept -> Result<std::array<T, Count>>
		{
			if constexpr (std::is_integral_v<T>)
			{
				const auto varResult = var.GetIntegerVectorValue();
				if (!varResult.has_value())
				{
					return Unexpected{ varResult.error() };
				}

				return varResult.value() | std::ranges::to<std::array<T, Count>>();
			}
			else
			{
				const auto varResult = var.GetVectorValue();
				if (!varResult.has_value())
				{
					return Unexpected{ varResult.error() };
				}

				return varResult.value() | std::ranges::to<std::array<T, Count>>();
			}
		}

		explicit operator std::array<T, Count>() const
		{
			if constexpr (std::is_integral_v<T>)
			{
				const auto valResult = var.GetIntegerVectorValue();
				if (!valResult.has_value())
				{
					throw Exception{ valResult.error() };
				}

				return valResult.value() | std::ranges::to<std::array<T, Count>>();
			}
			else
			{
				const auto valResult = var.GetVectorValue();
				if (!valResult.has_value())
				{
					throw Exception{ valResult.error() };
				}

				return valResult.value() | std::ranges::to<std::array<T, Count>>();
			}
		}

		auto GetFloatingValue() const noexcept -> Result<std::array<T, Count>>
		{
			const auto valResult = var.GetVectorValue();
			if (!valResult.has_value())
			{
				return Unexpected{ valResult.error() };
			}

			return valResult.value() | std::ranges::to<std::array<T, Count>>();
		}

		auto GetIntegerValue() const noexcept -> Result<std::array<T, Count>>
		{
			const auto valResult = var.GetIntegerVectorValue();
			if (!valResult.has_value())
			{
				return Unexpected{ valResult.error() };
			}

			return valResult.value() | std::ranges::to<std::array<T, Count>>();
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};

	template <>
	class Animated<RGBA>
	{
		public:
		explicit Animated(
			const RGBA& initialValue,
			const std::optional<AnimationManager>& animationManager = std::nullopt) :
			customAnimationManager{ animationManager }
		{
			const auto& manager = GetAnimationManager();
			const std::array<double, 4> initialArray{
				initialValue.r,
				initialValue.g,
				initialValue.b,
				initialValue.a
			};
			const auto varResult = manager.CreateAnimationVariable(initialArray);
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const RGBA& newValue, const double duration = 1.0) const noexcept -> Error
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			const std::array<double, 4> newArray{
				newValue.r,
				newValue.g,
				newValue.b,
				newValue.a
			};
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(duration, newArray).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			if (const auto scheduleResult = storyboard.Schedule(timer.GetTime().value());
				!scheduleResult.has_value())
			{
				return scheduleResult.error();
			}
			return Error{ ErrorCode::Success };
		}

		auto Get() const noexcept -> Result<RGBA>
		{
			const auto varResult = var.GetVectorValue();
			if (!varResult.has_value())
			{
				return Unexpected{ varResult.error() };
			}
			const auto& vec = varResult.value();
			if (vec.size() != 4)
			{
				return Unexpected{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 2 values got {}", vec.size())) };
			}

			return RGBA{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1)),
				static_cast<float>(vec.at(2)),
				static_cast<float>(vec.at(3))
			};
		}

		explicit operator RGBA() const
		{
			const auto valResult = var.GetVectorValue();
			if (!valResult.has_value())
			{
				throw Exception{ valResult.error() };
			}

			const auto& vec = valResult.value();
			if (vec.size() != 4)
			{
				throw Exception{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 2 values got {}", vec.size())) };
			}
			return RGBA{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1)),
				static_cast<float>(vec.at(2)),
				static_cast<float>(vec.at(3))
			};
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};

	template <>
	class Animated<PointF>
	{
		public:
		explicit Animated(
			const PointF& initialValue,
			const std::optional<AnimationManager>& animationManager = std::nullopt) :
			customAnimationManager{ animationManager }
		{
			const auto& manager = GetAnimationManager();
			const std::array<double, 2> initialArray{
				initialValue.x,
				initialValue.y
			};
			const auto varResult = manager.CreateAnimationVariable(initialArray);
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const PointF& newValue, const double duration = 1.0) const noexcept -> Error
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			const std::array<double, 2> newArray{
				newValue.x,
				newValue.y
			};
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(duration, newArray).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			if (const auto scheduleResult = storyboard.Schedule(timer.GetTime().value());
				!scheduleResult.has_value())
			{
				return scheduleResult.error();
			}
			return Error{ ErrorCode::Success };
		}

		auto Get() const noexcept -> Result<PointF>
		{
			const auto varResult = var.GetVectorValue();
			if (!varResult.has_value())
			{
				return Unexpected{ varResult.error() };
			}
			const auto& vec = varResult.value();
			if (vec.size() != 2)
			{
				return Unexpected{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 2 values got {}", vec.size())) };
			}
			return PointF{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1))
			};
		}

		explicit operator PointF() const
		{
			const auto valResult = var.GetVectorValue();
			if (!valResult.has_value())
			{
				throw Exception{ valResult.error() };
			}
			const auto& vec = valResult.value();
			if (vec.size() != 2)
			{
				throw Exception{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 2 values got {}", vec.size())) };
			}
			return PointF{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1))
			};
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};

	template <>
	class Animated<RectF>
	{
		public:
		explicit Animated(
			const RectF& initialValue,
			const std::optional<AnimationManager>& animationManager = std::nullopt) :
			customAnimationManager{ animationManager }
		{
			const auto& manager = GetAnimationManager();
			const std::array<double, 4> initialArray{
				initialValue.left,
				initialValue.top,
				initialValue.right,
				initialValue.bottom
			};
			const auto varResult = manager.CreateAnimationVariable(initialArray);
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const RectF& newValue, const double duration = 1.0) const noexcept -> Error
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			const std::array<double, 4> newArray{
				newValue.left,
				newValue.top,
				newValue.right,
				newValue.bottom
			};
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(duration, newArray).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			if (const auto scheduleResult = storyboard.Schedule(timer.GetTime().value());
				!scheduleResult.has_value())
			{
				return scheduleResult.error();
			}
			return Error{ ErrorCode::Success };
		}

		auto Get() const noexcept -> Result<RectF>
		{
			const auto varResult = var.GetVectorValue();
			if (!varResult.has_value())
			{
				return Unexpected{ varResult.error() };
			}
			const auto& vec = varResult.value();
			if (vec.size() != 4)
			{
				return Unexpected{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 4 values got {}", vec.size())) };
			}
			return RectF{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1)),
				static_cast<float>(vec.at(2)),
				static_cast<float>(vec.at(3))
			};
		}

		explicit operator RectF() const
		{
			const auto valResult = var.GetVectorValue();
			if (!valResult.has_value())
			{
				throw Exception{ valResult.error() };
			}
			const auto& vec = valResult.value();
			if (vec.size() != 4)
			{
				throw Exception{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 4 values got {}", vec.size())) };
			}
			return RectF{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1)),
				static_cast<float>(vec.at(2)),
				static_cast<float>(vec.at(3))
			};
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};

	template <>
	class Animated<SizeF>
	{
		public:
		explicit Animated(
			const SizeF& initialValue,
			const std::optional<AnimationManager>& animationManager = std::nullopt) :
			customAnimationManager{ animationManager }
		{
			const auto& manager = GetAnimationManager();
			const std::array<double, 2> initialArray{
				initialValue.cx,
				initialValue.cy
			};
			const auto varResult = manager.CreateAnimationVariable(initialArray);
			if (!varResult.has_value())
			{
				throw Exception{ varResult.error() };
			}
			var = varResult.value();
		}

		auto AnimateTo(const SizeF& newValue, const double duration = 1.0) const noexcept -> Error
		{
			const auto storyboardResult = AnimationManager::GetGlobalInstance().CreateStoryboard();
			if (!storyboardResult.has_value())
			{
				return storyboardResult.error();
			}
			const auto& storyboard = storyboardResult.value();
			const std::array<double, 2> newArray{
				newValue.cx,
				newValue.cy
			};
			auto res = storyboard.AddTransition(
				var,
				AnimationTransitionLibrary::LinearTransition(duration, newArray).value());
			const auto& timer = AnimationTimer::GetGlobalInstance();
			if (const auto scheduleResult = storyboard.Schedule(timer.GetTime().value());
				!scheduleResult.has_value())
			{
				return scheduleResult.error();
			}
			return Error{ ErrorCode::Success };
		}

		auto Get() const noexcept -> Result<SizeF>
		{
			const auto varResult = var.GetVectorValue();
			if (!varResult.has_value())
			{
				return Unexpected{ varResult.error() };
			}
			const auto& vec = varResult.value();
			if (vec.size() != 2)
			{
				return Unexpected{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 2 values got {}", vec.size())) };
			}
			return SizeF{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1))
			};
		}

		explicit operator SizeF() const
		{
			const auto valResult = var.GetVectorValue();
			if (!valResult.has_value())
			{
				throw Exception{ valResult.error() };
			}
			const auto& vec = valResult.value();
			if (vec.size() != 2)
			{
				throw Exception{ Error{
					ErrorCode::Failure
				}.SetCustomMessage(std::format(L"Expected 2 values got {}", vec.size())) };
			}
			return SizeF{
				static_cast<float>(vec.at(0)),
				static_cast<float>(vec.at(1))
			};
		}

		auto GetAnimationManager() const noexcept -> const AnimationManager&
		{
			if (customAnimationManager.has_value())
			{
				return customAnimationManager.value();
			}
			return AnimationManager::GetGlobalInstance();
		}

		private:
		AnimationVariable var;
		std::optional<AnimationManager> customAnimationManager;
	};
}

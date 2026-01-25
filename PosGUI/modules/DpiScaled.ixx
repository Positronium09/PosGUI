module;
#include <Windows.h>

export module PGUI.DpiScaled;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;

export namespace PGUI
{
	template <typename T>
	concept DpiMulScale = requires(std::remove_cvref_t<std::remove_pointer_t<T>> value)
	{
		{ value * DEFAULT_SCREEN_DPI } -> std::convertible_to<std::remove_cvref_t<std::remove_pointer_t<T>>>;
	};
	template <typename T>
	concept DpiDivScale = requires(std::remove_cvref_t<std::remove_pointer_t<T>> value)
	{
		{ value / DEFAULT_SCREEN_DPI } -> std::convertible_to<std::remove_cvref_t<std::remove_pointer_t<T>>>;
	};
	template <typename T>
	concept DpiFuncRetScalable = requires(std::remove_cvref_t<std::remove_pointer_t<T>> value)
	{
		value.ScaleByDpiFactor(1.0F);
	};
	template <typename T>
	concept DpiFuncModScalable = requires(std::remove_cvref_t<std::remove_pointer_t<T>> value)
	{
		{ value.ScaleByDpiFactor(1.0F) } -> std::convertible_to<std::remove_cvref_t<std::remove_pointer_t<T>>>;
	};

	template <typename T>
	concept DpiScalable = std::is_arithmetic_v<T> || 
		DpiMulScale<T> || 
		DpiDivScale<T> || 
		DpiFuncRetScalable<T> ||
		DpiFuncModScalable<T>;

#pragma warning(push)
#pragma warning(disable: 4244, \
	justification: "Narrowing conversion may occur and DpiScaled cannot fix such an warning")

	template <DpiScalable T>
	class DpiScaled final
	{
		public:
		explicit DpiScaled(const float dpi = DEFAULT_SCREEN_DPI) noexcept(std::is_nothrow_default_constructible_v<T>) :
			dpi{ dpi }
		{
		}
		explicit DpiScaled(const T& value, const float dpi = DEFAULT_SCREEN_DPI) noexcept(std::is_nothrow_copy_constructible_v<T>) :
			dpi{ dpi }, logicalValue{ value }
		{
		}
		explicit DpiScaled(T&& value, const float dpi = DEFAULT_SCREEN_DPI) noexcept(std::is_nothrow_move_constructible_v<T>) :
			dpi{ dpi }, logicalValue{ std::move(value) }
		{
		}
		DpiScaled(const DpiScaled& other) noexcept(std::is_nothrow_copy_constructible_v<T>) :
			dpi{ other.dpi }, logicalValue{ other.logicalValue }
		{
		}
		DpiScaled(DpiScaled&& other) noexcept(std::is_nothrow_move_constructible_v<T>) :
			dpi{ other.dpi }, logicalValue{ std::move(other.logicalValue) }
		{
		}

		auto operator=(const T& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> DpiScaled&
		{
			logicalValue = other;
			return *this;
		}
		auto operator=(T&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> DpiScaled&
		{
			logicalValue = std::move(other);
			return *this;
		}

		constexpr explicit operator T() const noexcept(std::is_nothrow_copy_constructible_v<T>)
		{
			return logicalValue;
		}
		constexpr explicit operator T&() noexcept
		{
			return logicalValue;
		}

		auto SetDpi(const float newDpi) noexcept -> Error
		{
			if (newDpi <= 0.0F)
			{
				Error error{ ErrorCode::InvalidArgument };
				Logger::Warning(error);

				return error;
			}
			dpi = newDpi;
			return Error{ ErrorCode::Success };
		}
		constexpr auto GetDpi() const noexcept -> float
		{
			return dpi;
		}

		const auto& GetLogicalValue() const noexcept
		{
			return logicalValue;
		}
		auto SetLogicalValue(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>) -> void
		{
			logicalValue = value;
		}
		auto SetLogicalValue(T&& value) noexcept(std::is_nothrow_move_assignable_v<T>) -> void
		{
			logicalValue = std::move(value);
		}
		auto SetPhysicalValue(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>) -> void
		{
			if constexpr (std::is_arithmetic_v<T> || DpiDivScale<T>)
			{
				logicalValue = static_cast<T>(value / GetScaleFactor());
			}
			else if constexpr (DpiMulScale<T>)
			{
				logicalValue = static_cast<T>(value * GetInverseScaleFactor());
			}
			else if constexpr (DpiFuncModScalable<T>)
			{
				logicalValue = value;
				logicalValue.ScaleByDpiFactor(GetInverseScaleFactor());
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				logicalValue = value.ScaleByDpiFactorFactor(GetInverseScaleFactor());
			}
		}
		auto SetPhysicalValue(T&& value) noexcept(std::is_nothrow_move_assignable_v<T>) -> void
		{
			if constexpr (std::is_arithmetic_v<T> || DpiDivScale<T>)
			{
				logicalValue = static_cast<T>(value / GetScaleFactor());
			}
			else if constexpr (DpiMulScale<T>)
			{
				logicalValue = static_cast<T>(value * GetInverseScaleFactor());
			}
			else if constexpr (DpiFuncModScalable<T>)
			{
				logicalValue = std::move(value);
				logicalValue.ScaleByDpiFactor(GetInverseScaleFactor());
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				logicalValue = value.ScaleByDpiFactorFactor(GetInverseScaleFactor());
			}
		}
		auto GetPhysicalValue() const noexcept -> T
		{
			if constexpr (std::is_arithmetic_v<T> || DpiMulScale<T>)
			{
				return static_cast<T>(logicalValue * GetScaleFactor());
			}
			else if constexpr (DpiDivScale<T>)
			{
				return static_cast<T>(logicalValue / GetInverseScaleFactor());
			}
			else if constexpr (DpiFuncModScalable<T>)
			{
				if constexpr (std::is_copy_assignable_v<T>)
				{
					auto copy = logicalValue;
					copy.ScaleByDpiFactor(GetScaleFactor());
					
					return copy;
				}
				else if (std::is_copy_constructible_v<T>)
				{
					return T{ logicalValue }.ScaleByDpiFactor(GetScaleFactor());
				}
				else
				{
					// No copy constructor, return logical value directly
					return logicalValue.ScaleByDpiFactor(GetScaleFactor());
				}
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				return logicalValue.ScaleByDpiFactorFactor(GetScaleFactor());
			}
			std::unreachable();
		}

		constexpr auto GetScaleFactor() const noexcept -> float
		{
			return dpi / DEFAULT_SCREEN_DPI;
		}
		constexpr auto GetInverseScaleFactor() const noexcept -> float
		{
			return DEFAULT_SCREEN_DPI / dpi;
		}

		constexpr auto operator->() const noexcept -> const T*
		{
			return &logicalValue;
		}
		constexpr auto operator->() noexcept -> T*
		{
			return &logicalValue;
		}
		constexpr auto operator*() const noexcept
		{
			return GetLogicalValue();
		}

		auto operator==(const DpiScaled& other) const noexcept -> bool
		{
			static_assert(std::equality_comparable<T>);
			return logicalValue == other.logicalValue && dpi == other.dpi;
		}
		auto operator<=>(const DpiScaled& other) const noexcept -> std::strong_ordering
		{
			static_assert(std::three_way_comparable<T>);
			return logicalValue <=> other.logicalValue;
		}
		auto operator==(const T& other) const noexcept -> bool
		{
			static_assert(std::equality_comparable<T>);
			return logicalValue == other;
		}
		auto operator<=>(const T& other) const noexcept -> std::strong_ordering
		{
			static_assert(std::three_way_comparable<T>);
			return logicalValue <=> other;
		}

		private:
		float dpi{ DEFAULT_SCREEN_DPI };
		T logicalValue{ };
	};

	namespace Scaling
	{
		template <DpiScalable T>
		[[nodiscard]] auto LogicalToPhysical(const T& value, const float dpi = DEFAULT_SCREEN_DPI) noexcept
		{
			if constexpr (std::is_arithmetic_v<T> || DpiMulScale<T>)
			{
				return static_cast<T>(value * (dpi / DEFAULT_SCREEN_DPI));
			}
			else if constexpr (DpiDivScale<T>)
			{
				return static_cast<T>(value / (dpi / DEFAULT_SCREEN_DPI));
			}
			else if constexpr (DpiFuncModScalable<T>)
			{
				auto copy = value;
				copy.ScaleByDpiFactor(dpi / DEFAULT_SCREEN_DPI);
				return copy;
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				return value.ScaleByDpiFactorFactor(dpi / DEFAULT_SCREEN_DPI);
			}
			else
			{
				return value; // No scaling possible, return as is
			}
		}
		template <DpiScalable T>
		[[nodiscard]] auto PhysicalToLogical(const T& value, const float dpi = DEFAULT_SCREEN_DPI) noexcept
		{
			if constexpr (std::is_arithmetic_v<T> || DpiDivScale<T>)
			{
				return static_cast<T>(value / (dpi / DEFAULT_SCREEN_DPI));
			}
			else if constexpr (DpiMulScale<T>)
			{
				return static_cast<T>(value * (dpi / DEFAULT_SCREEN_DPI));
			}
			else if constexpr (DpiFuncModScalable<T>)
			{
				auto copy = value;
				copy.ScaleByDpiFactor(1.0F / (dpi / DEFAULT_SCREEN_DPI));
				return copy;
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				return value.ScaleByDpiFactorFactor(1.0F / (dpi / DEFAULT_SCREEN_DPI));
			}
			else
			{
				return value; // No scaling possible, return as is
			}
		}
	}

	#pragma warning(pop)
}

module;
#include <Windows.h>

export module PGUI.DpiScaled;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;

export namespace PGUI
{
	template <typename T>
	concept DpiMulScale = requires(std::remove_cvref_t<T> value)
	{
		{ value * DEFAULT_SCREEN_DPI } -> std::convertible_to<std::remove_cvref_t<T>>;
	};
	template <typename T>
	concept DpiDivScale = requires(std::remove_cvref_t<T> value)
	{
		{ value / DEFAULT_SCREEN_DPI } -> std::convertible_to<std::remove_cvref_t<T>>;
	};
	template <typename T>
	concept DpiFuncMutScalable = requires(std::remove_cvref_t<T> value)
	{
		{ value.ScaleByDpiFactor(1.0F) } -> std::same_as<void>;
	};
	template <typename T>
	concept DpiFuncRetScalable = requires(std::remove_cvref_t<T> value)
	{
		{ value.ScaleByDpiFactor(1.0F) } -> std::convertible_to<std::remove_cvref_t<T>>;
	};

	template <typename T>
	concept DpiScalable = std::is_arithmetic_v<T> || 
		DpiMulScale<T> || 
		DpiDivScale<T> || 
		DpiFuncMutScalable<T> ||
		DpiFuncRetScalable<T>;

#pragma warning(push)
#pragma warning(disable: 4244, \
	justification: "Narrowing conversion may occur and DpiScaled cannot fix such an warning")


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
				return static_cast<T>(value / (DEFAULT_SCREEN_DPI / dpi));
			}
			else if constexpr (DpiFuncMutScalable<T>)
			{
				auto copy = value;
				copy.ScaleByDpiFactor(dpi / DEFAULT_SCREEN_DPI);
				return copy;
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				return value.ScaleByDpiFactor(dpi / DEFAULT_SCREEN_DPI);
			}
			else
			{
				return value;
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
				return static_cast<T>(value * (DEFAULT_SCREEN_DPI / dpi));
			}
			else if constexpr (DpiFuncMutScalable<T>)
			{
				auto copy = value;
				copy.ScaleByDpiFactor(1.0F / (dpi / DEFAULT_SCREEN_DPI));
				return copy;
			}
			else if constexpr (DpiFuncRetScalable<T>)
			{
				return value.ScaleByDpiFactor(1.0F / (dpi / DEFAULT_SCREEN_DPI));
			}
			else
			{
				return value;
			}
		}
	}

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
		auto operator=(const DpiScaled& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> DpiScaled&
		{
			dpi = other.dpi;
			logicalValue = other.logicalValue;
			return *this;
		}
		DpiScaled(DpiScaled&& other) noexcept(std::is_nothrow_move_constructible_v<T>) :
			dpi{ other.dpi }, logicalValue{ std::move(other.logicalValue) }
		{
		}
		auto operator=(DpiScaled&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> DpiScaled&
		{
			dpi = other.dpi;
			logicalValue = std::move(other.logicalValue);
			return *this;
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
			logicalValue = Scaling::PhysicalToLogical(value, dpi);
		}

		auto GetPhysicalValue() const noexcept -> T
		{
			return Scaling::LogicalToPhysical(logicalValue, dpi);
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
		requires std::equality_comparable<T>
		{
			return logicalValue == other.logicalValue && dpi == other.dpi;
		}
		auto operator<=>(const DpiScaled& other) const noexcept -> auto
		requires std::three_way_comparable<T>
		{
			return logicalValue <=> other.logicalValue;
		}
		auto operator==(const T& other) const noexcept -> bool
			requires std::equality_comparable<T>
		{
			static_assert(std::equality_comparable<T>);
			return logicalValue == other;
		}
		auto operator<=>(const T& other) const noexcept -> auto
			requires std::three_way_comparable<T>
		{
			static_assert(std::three_way_comparable<T>);
			return logicalValue <=> other;
		}

		private:
		float dpi{ DEFAULT_SCREEN_DPI };
		T logicalValue{ };
	};

	#pragma warning(pop)
}

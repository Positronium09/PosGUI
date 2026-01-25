export module PGUI.UI.Animation:AnimationTimeTypes;

import std;

namespace PGUI::UI::Animation
{
	template <typename T>
	struct DurationTypeHelper : std::false_type { };

	template <typename Period>
	struct DurationTypeHelper<std::chrono::duration<double, Period>> : std::true_type { };
}

export namespace PGUI::UI::Animation
{
	template <typename Ratio>
	using Duration = std::chrono::duration<double, Ratio>;

	template <typename T>
	concept DurationType = DurationTypeHelper<T>::value;

	using Nanoseconds = Duration<std::nano>;
	using Microseconds = Duration<std::micro>;
	using Milliseconds = Duration<std::milli>;
	using Seconds = Duration<std::ratio<1>>;
	using Minutes = Duration<std::ratio<60>>;
	using Hours = Duration<std::ratio<3600>>;

	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;

	template <typename Repr, typename Period>
	[[nodiscard]] constexpr auto ToSeconds(const std::chrono::duration<Repr, Period>& duration) noexcept
	{
		return std::chrono::duration_cast<Seconds>(duration).count();
	}

	template <typename Repr, typename Period>
	[[nodiscard]] constexpr auto ToMilliseconds(const std::chrono::duration<Repr, Period>& duration) noexcept
	{
		return std::chrono::duration_cast<Milliseconds>(duration).count();
	}

	[[nodiscard]] constexpr auto FromSeconds(const double seconds) noexcept
	{
		return Seconds{ seconds };
	}

	[[nodiscard]] constexpr auto FromMilliseconds(const double milliseconds) noexcept
	{
		return Milliseconds{ milliseconds };
	}

	[[nodiscard]] auto Now() noexcept -> TimePoint
	{
		return Clock::now();
	}

	[[nodiscard]] constexpr auto FromWAM(const double wamTime) noexcept
	{
		return FromSeconds(wamTime);
	}

	[[nodiscard]] constexpr auto ToWAM(const auto& duration) noexcept
	{
		return ToSeconds(duration);
	}
}

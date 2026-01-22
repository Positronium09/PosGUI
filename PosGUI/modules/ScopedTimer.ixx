export module PGUI.ScopedTimer;

import std;

import PGUI.ErrorHandling;
import PGUI.Utils;

export namespace PGUI
{
	class ScopedTimer
	{
		public:
		static auto SetTimeThreshold(
			const std::chrono::nanoseconds threshold) noexcept -> void
		{
			durationThreshold = threshold;
		}

		explicit ScopedTimer(
			const std::wstring_view name,
			const bool overrideThreshold = false,
			const std::source_location& location = std::source_location::current()) noexcept :
			timerName{ name },
			overrideThreshold{ overrideThreshold },
			sourceLocation{ location },
			startTime{ std::chrono::high_resolution_clock::now() }
		{
		}
		~ScopedTimer() noexcept
		{
			const auto endTime = std::chrono::high_resolution_clock::now();
			const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
			if (duration < durationThreshold && !overrideThreshold)
			{
				return;
			}
			if (duration < std::chrono::milliseconds{ 1 })
			{
				Logger::Info(L"Timer '{}' finished in {}ns", timerName, duration.count());
			}
			else
			{
				const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
				Logger::Info(L"Timer '{}' finished in {}ms", timerName, milliseconds.count());
			}

			Logger::Info(L"Source location: {}:{}({},{})", 
				StringToWString(sourceLocation.file_name()),
				StringToWString(sourceLocation.function_name()),
				sourceLocation.line(),
				sourceLocation.column());
		}

		ScopedTimer(const ScopedTimer&) = delete;
		ScopedTimer(ScopedTimer&&) = delete;
		auto operator=(const ScopedTimer&) -> ScopedTimer  = delete;
		auto operator=(ScopedTimer&&) -> ScopedTimer& = delete;

		private:
		inline static std::chrono::nanoseconds durationThreshold = std::chrono::milliseconds{ 100 };

		std::wstring timerName;
		bool overrideThreshold;
		std::source_location sourceLocation;
		std::chrono::high_resolution_clock::time_point startTime;
	};

	struct NullTimer
	{
		static auto SetTimeThreshold(
			[[maybe_unused]] const std::chrono::nanoseconds threshold) noexcept -> void
		{
		}

		explicit NullTimer(
			[[maybe_unused]] const std::wstring_view name,
			[[maybe_unused]] const bool overrideThreshold = false,
			[[maybe_unused]] const std::source_location& location = std::source_location::current()) noexcept
		{
		}
	};

#ifdef _DEBUG
	using DebugTimer = ScopedTimer;
#else
	using DebugTimer = NullTimer;
#endif
}

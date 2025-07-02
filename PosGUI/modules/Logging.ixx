module;
#include <Windows.h>

export module PGUI.Logging;

import std;

import PGUI.Utils;
import PGUI.Exceptions;

export namespace PGUI
{
	enum class LogLevel
	{
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	class Logger
	{
		public:
		virtual ~Logger() = default;

		static auto Log(const LogLevel level, const std::wstring_view msg,
			const std::stacktrace& trace = std::stacktrace::current()) -> void
		{
			if (level < logLevel)
			{
				return;
			}
			if (logger)
			{
				logger->LogImpl(level, msg, trace);
			}
		}

		static auto SetLogger(Logger* logger_) noexcept -> void { logger = logger_; }
		[[nodiscard]] static auto GetLogger() noexcept -> Logger* { return logger; }
		static auto SetLogLevel(const LogLevel level) noexcept -> void { logLevel = level; }
		[[nodiscard]] static auto GetLogLevel() noexcept -> LogLevel { return logLevel; }

		[[nodiscard]] static auto GetLogLevelString(const LogLevel level) noexcept -> std::wstring
		{
			switch (level)
			{
				case LogLevel::Trace:
					return L"Trace";
				case LogLevel::Debug:
					return L"Debug";
				case LogLevel::Info:
					return L"Info";
				case LogLevel::Warning:
					return L"Warning";
				case LogLevel::Error:
					return L"Error";
				case LogLevel::Fatal:
					return L"Fatal";
				default:
					std::unreachable();
			}
		}

		protected:
		virtual auto LogImpl(
			LogLevel logLevel, std::wstring_view msg,
			const std::stacktrace& trace) -> void = 0;

		private:
		static inline auto logLevel = LogLevel::Info;
		static inline Logger* logger = nullptr;
	};

	class ConsoleLogger final : public Logger
	{
		public:
		auto LogImpl(const LogLevel level, std::wstring_view msg,
			const std::stacktrace& trace) -> void override
		{
			auto logString = GetLogLevelString(level);
			const auto& entry = trace.at(0);

			const auto formatted = std::format(
				L"[{}] Line {} in {}\n[{}] {}\n",
				logString, entry.source_line(), StringToWString(entry.source_file()),
				logString, msg);

			OutputDebugStringW(formatted.c_str());
		}
	};

	auto LogFailed(const LogLevel level, HRESULT hr,
		const std::stacktrace& trace = std::stacktrace::current()) -> void
	{
		if (FAILED(hr))
		{
			Logger::Log(
				level,
				std::format(L"{:#x} {}", hr, HresultFromWin32(hr)),
				trace);
		}
	}

	auto LogFailed(const LogLevel level, WINERR err,
		const std::stacktrace& trace = std::stacktrace::current()) -> void
	{
		if (err != ERROR_SUCCESS)
		{
			Logger::Log(
				level,
				std::format(L"{:#x} {}", err, WinErrToString(err)),
				trace);
		}
	}
}

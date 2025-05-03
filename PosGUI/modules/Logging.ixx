module;
#include <Windows.h>
#include <system_error>
#include <string>
#include <format>
#include <string_view>
#include <stacktrace>

export module PGUI.Logging;

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

		static void Log(LogLevel level, std::wstring_view msg, 
			const std::stacktrace& trace = std::stacktrace::current())
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

		static void SetLogger(Logger* logger_) noexcept { Logger::logger = logger_; }
		[[nodiscard]] static auto GetLogger() noexcept -> Logger* { return logger; }
		static void SetLogLevel(LogLevel level) noexcept { logLevel = level; }
		[[nodiscard]] static auto GetLogLevel() noexcept -> LogLevel { return logLevel; }
		
		[[nodiscard]] static auto GetLogLevelString(LogLevel level) noexcept -> std::wstring
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
		virtual void LogImpl(LogLevel logLevel, std::wstring_view msg,
			const std::stacktrace& trace) = 0;

		private:
		static inline LogLevel logLevel = LogLevel::Info;
		static inline Logger* logger = nullptr;
	};

	class ConsoleLogger : public Logger
	{
		public:
		void LogImpl(LogLevel level, std::wstring_view msg,
			const std::stacktrace& trace) override
		{
			auto logString = GetLogLevelString(level);
			const auto& entry = trace.at(0);

			auto formatted = std::format(L"[{}] Line {} in {}\n[{}] {}\n", 
				logString, entry.source_line(), StringToWString(entry.source_file()),
				logString, msg);

			OutputDebugStringW(formatted.c_str());
		}
	};

	void LogFailed(LogLevel level, HRESULT hr, 
		const std::stacktrace& trace = std::stacktrace::current())
	{
		if (FAILED(hr))
		{
			Logger::Log(level, 
				std::format(L"{:#x} {}", hr, HresultFromWin32(hr)),
				trace);
		}
	}
	void LogFailed(LogLevel level, WINERR err,
		const std::stacktrace& trace = std::stacktrace::current())
	{
		if (err != ERROR_SUCCESS)
		{
			Logger::Log(level, 
				std::format(L"{:#x} {}", err, WinErrToString(err)),
				trace);
		}
	}
}

module;
#include <Windows.h>

export module PGUI.ErrorHandling:Logger;

import std;

import :Error;
import :ErrorCodes;
import :Exception;

export namespace PGUI
{
	enum class LogLevel
	{
		Trace = 0,
		Debug = 1,
		Info = 2,
		Warning = 3,
		Error = 4,
		Critical = 5
	};

	constexpr auto LogLevelToString(const LogLevel logLevel) noexcept -> std::wstring_view
	{
		switch (logLevel)
		{
			case LogLevel::Trace:
			{
				return L"Trace";
			}
			case LogLevel::Debug:
			{
				return L"Debug";
			}
			case LogLevel::Info:
			{
				return L"Info";
			}
			case LogLevel::Warning:
			{
				return L"Warning";
			}
			case LogLevel::Error:
			{
				return L"Error";
			}
			case LogLevel::Critical:
			{
				return L"Critical";
			}
			default:
				std::unreachable();
		}
	}

	class LogSink
	{
		public:
		virtual ~LogSink() = default;

		virtual auto Log(LogLevel logLevel, std::wstring_view message) -> void = 0;
	};

	struct DebugConsoleSink final : LogSink
	{
		auto Log(const LogLevel logLevel, const std::wstring_view message) -> void override
		{
			OutputDebugStringW(
				std::format(L"[{}] {}\n", LogLevelToString(logLevel), message).c_str()
			);
		}
	};

	class FileSink final : public LogSink
	{
		public:
		explicit FileSink(const std::filesystem::path& filePath)
			: file{ filePath }
		{
			if (!file)
			{
				throw Exception{ std::errc::no_such_file_or_directory }
				.AddDetail(L"filePath", filePath.wstring());
			}
		}

		auto Log(const LogLevel logLevel, const std::wstring_view message) -> void override
		{
			auto now = std::chrono::system_clock::now();

			file << std::format(L"[{:%Y-%m-%d %H:%M:%S}] [{}] {}\n",
				now, LogLevelToString(logLevel), message);
		}

		private:
		std::wofstream file;
	};

	class Logger final
	{
		public:
		static auto SetLogLevel(const LogLevel level) noexcept
		{
			defaultLogLevel = level;
		}

		[[nodiscard]] static auto GetLogLevel() noexcept
		{
			return defaultLogLevel;
		}

		static auto SetLogger(LogSink& loggerInstance) noexcept
		{
			std::scoped_lock lock{ loggingMutex };
			logger = &loggerInstance;
		}

		static auto SetLogger(LogSink* loggerInstance) noexcept
		{
			std::scoped_lock lock{ loggingMutex };
			logger = loggerInstance;
		}

		[[nodiscard]] static auto GetLogger() noexcept -> Result<std::reference_wrapper<LogSink>>
		{
			if (logger == nullptr)
			{
				return Unexpected{ E_NOT_SET };
			}
			return *logger;
		}

		template <std::derived_from<LogSink> Sink>
		[[nodiscard]] static auto& GetLogger() noexcept
		{
			return *dynamic_cast<Sink*>(logger);
		}

		static auto Log(const LogLevel logLevel, const std::wstring_view message)
		{
			if (logLevel < defaultLogLevel)
			{
				return;
			}
			std::scoped_lock lock{ loggingMutex };

			const auto logSink = GetLogger();
			if (!logSink)
			{
				throw Exception{ E_POINTER }
					.SuggestFix(L"Set logger before using")
					.AddDetail(L"message", message)
					.AddDetail(L"logLevel", LogLevelToString(logLevel));
			}

			logSink.value().get().Log(logLevel, message);
		}
		static auto Log(const std::wstring_view message)
		{
			Log(defaultLogLevel, message);
		}

		template <typename ...Args>
		static auto Log(
			const LogLevel logLevel,
			const std::wformat_string<Args...> formatString,
			Args&&... args)
		{
			auto formattedMessage = std::vformat(formatString.get(), std::make_wformat_args(args...));

			Log(logLevel, formattedMessage);
		}
		template <typename ...Args>
		static auto Log(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(defaultLogLevel, formatString, std::forward<Args>(args)...);
		}

		static auto Log(const LogLevel logLevel, const Error& error, const std::wstring_view message = L"")
		{
			if (message.empty())
			{
				// ReSharper disable once StringLiteralTypo
				Log(logLevel, std::format(L"{:cmsdfx}", error));
				return;
			}

			// ReSharper disable once StringLiteralTypo
			Log(logLevel, std::format(L"Message: {} | {:cmsdfx}", message, error));
		}
		static auto Log(const Error& error, const std::wstring_view message = L"")
		{
			Log(defaultLogLevel, error, message);
		}

		static auto Trace(const std::wstring_view message)
		{
			Log(LogLevel::Trace, message);
		}
		template <typename ...Args>
		static auto Trace(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(LogLevel::Trace, formatString, std::forward<Args>(args)...);
		}
		static auto Trace(const Error& error, const std::wstring_view message = L"")
		{
			Log(LogLevel::Trace, error, message);
		}

		static auto Debug(const std::wstring_view message)
		{
			Log(LogLevel::Debug, message);
		}
		template <typename ...Args>
		static auto Debug(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(LogLevel::Debug, formatString, std::forward<Args>(args)...);
		}
		static auto Debug(const Error& error, const std::wstring_view message = L"")
		{
			Log(LogLevel::Debug, error, message);
		}

		static auto Info(const std::wstring_view message)
		{
			Log(LogLevel::Info, message);
		}
		template <typename ...Args>
		static auto Info(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(LogLevel::Info, formatString, std::forward<Args>(args)...);
		}
		static auto Info(const Error& error, const std::wstring_view message = L"")
		{
			Log(LogLevel::Info, error, message);
		}

		static auto Warning(const std::wstring_view message)
		{
			Log(LogLevel::Warning, message);
		}
		template <typename ...Args>
		static auto Warning(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(LogLevel::Warning, formatString, std::forward<Args>(args)...);
		}
		static auto Warning(const Error& error, const std::wstring_view message = L"")
		{
			Log(LogLevel::Warning, error, message);
		}

		static auto Error(const std::wstring_view message)
		{
			Log(LogLevel::Error, message);
		}
		template <typename ...Args>
		static auto Error(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(LogLevel::Error, formatString, std::forward<Args>(args)...);
		}
		static auto Error(const PGUI::Error& error, const std::wstring_view message = L"")
		{
			Log(LogLevel::Error, error, message);
		}

		static auto Critical(const std::wstring_view message)
		{
			Log(LogLevel::Critical, message);
		}
		template <typename ...Args>
		static auto Critical(const std::wformat_string<Args...> formatString, Args&&... args)
		{
			Log(LogLevel::Critical, formatString, std::forward<Args>(args)...);
		}
		static auto Critical(const PGUI::Error& error, const std::wstring_view message = L"")
		{
			Log(LogLevel::Critical, error, message);
		}

		private:
		static inline std::mutex loggingMutex;
		static inline LogSink* logger = nullptr;
		static inline auto defaultLogLevel =
			#ifdef _DEBUG
			LogLevel::Debug;
		#else
			LogLevel::Info;
		#endif
	};

	auto LogIfFailed(const Error& error, const std::wstring_view message = L"")
	{
		if (error.IsSuccess())
		{
			return;
		}

		Logger::Error(error, message);
	}

	auto LogIfFailed(const LogLevel logLevel, const Error& error, const std::wstring_view message = L"")
	{
		if (error.IsSuccess())
		{
			return;
		}

		Logger::Log(logLevel, error, message);
	}
}

module;
#include <Windows.h>

export module PGUI.ErrorHandling:Error;

import std;

import PGUI.Utils;
import :ErrorCodes;

export namespace PGUI
{
	using WINERROR = DWORD;

	class Error;

	template <typename T>
	using Result = std::expected<T, Error>;

	using Unexpected = std::unexpected<Error>;

	class Error
	{
		public:
		explicit Error(
			const std::error_code code,
			const std::source_location& sourceLocation = std::source_location::current(),
			const std::chrono::system_clock::time_point& timeStamp = std::chrono::system_clock::now()) noexcept :
			code{ code }, sourceLocation{ sourceLocation }, timeStamp{ timeStamp }
		{
		}

		template <PGUIErrorCodeEnum ErrorType>
		explicit Error(
			const ErrorType error,
			const std::source_location& sourceLocation = std::source_location::current(),
			const std::chrono::system_clock::time_point& timeStamp = std::chrono::system_clock::now()) noexcept :
			Error{ std::make_error_code(error), sourceLocation, timeStamp }
		{
		}

		explicit Error(
			const HRESULT hresult,
			const std::source_location& sourceLocation = std::source_location::current(),
			const std::chrono::system_clock::time_point& timeStamp = std::chrono::system_clock::now()) noexcept :
			Error{ std::error_code{ hresult, std::system_category() }, sourceLocation, timeStamp }
		{
		}

		explicit Error(
			const std::errc errc,
			const std::source_location& sourceLocation = std::source_location::current(),
			const std::chrono::system_clock::time_point& timeStamp = std::chrono::system_clock::now()) noexcept :
			Error{ std::make_error_code(errc), sourceLocation, timeStamp }
		{
		}

		explicit Error(
			const WINERROR winError,
			const std::source_location& sourceLocation = std::source_location::current(),
			const std::chrono::system_clock::time_point& timeStamp = std::chrono::system_clock::now()) noexcept :
			Error{ HresultFromWin32(winError), sourceLocation, timeStamp }
		{
		}

		Error(const Error&) = default;
		Error(Error&&) noexcept = default;
		auto operator=(const Error&) -> Error & = default;
		auto operator=(Error&&) noexcept -> Error & = default;

		[[nodiscard]] auto Code() const noexcept
		{
			return code;
		}
		[[nodiscard]] const auto& SourceLocation() const noexcept
		{
			return sourceLocation;
		}
		[[nodiscard]] const auto& TimeStamp() const noexcept
		{
			return timeStamp;
		}
		[[nodiscard]] auto ErrorCode() const noexcept
		{
			return code.value();
		}
		[[nodiscard]] auto HResult() const noexcept
		{
			return code.value();
		}
		[[nodiscard]] auto Message() const noexcept
		{
			return code.message();
		}

		[[nodiscard]] const auto& Details() const noexcept
		{
			return details;
		}
		[[nodiscard]] auto HasDetails() const noexcept
		{
			return !details.empty();
		}
		[[nodiscard]] auto HasDetail(const std::wstring& key) const noexcept
		{
			return details.contains(key);
		}
		auto& AddDetail(std::wstring_view key, std::wstring_view value)
		{
			details.emplace(key, value);
			return *this;
		}
		auto& RemoveDetail(const std::wstring& key) noexcept
		{
			if (HasDetail(key))
			{
				details.erase(key);
			}

			return *this;
		}
		[[nodiscard]] auto GetDetail(const std::wstring& key) const noexcept -> Result<std::wstring_view>
		{
			if (!details.contains(key))
			{
				return Unexpected{ Error{
					std::make_error_code(std::errc::invalid_argument)
				} };
			}

			return details.at(key);
		}

		auto& SetCustomMessage(std::wstring_view message) noexcept
		{
			customMessage = message;
			return *this;
		}
		[[nodiscard]] auto CustomMessage() const noexcept
		{
			return customMessage;
		}
		[[nodiscard]] auto HasCustomMessage() const noexcept
		{
			return customMessage.has_value();
		}

		[[nodiscard]] const auto& FixSuggestions() const noexcept
		{
			return fixSuggestions;
		}
		[[nodiscard]] auto HasSuggestions() const noexcept
		{
			return !fixSuggestions.empty();
		}
		auto& SuggestFix(const std::wstring_view suggestion)
		{
			fixSuggestions.emplace_back(suggestion);
			return *this;
		}

		[[nodiscard]] auto IsSuccess() const noexcept
		{
			return SUCCEEDED(HResult());
		}

		[[nodiscard]] auto IsFailure() const noexcept
		{
			return !IsSuccess();
		}

		[[nodiscard]] auto operator[](const std::wstring& key) const noexcept
		{
			return GetDetail(key);
		}

		[[nodiscard]] auto operator==(const Error& other) const noexcept
		{
			return code == other.code &&
				sourceLocation.file_name() == other.sourceLocation.file_name() &&
				sourceLocation.function_name() == other.sourceLocation.function_name() &&
				sourceLocation.line() == other.sourceLocation.line() &&
				sourceLocation.column() == other.sourceLocation.column() &&
				details == other.details;
		}

		private:
		std::error_code code;
		std::source_location sourceLocation;
		std::chrono::system_clock::time_point timeStamp;
		std::unordered_map<std::wstring, std::wstring> details{ };
		std::optional<std::wstring> customMessage;
		std::vector<std::wstring> fixSuggestions;
	};
}

export template <typename Char>
struct std::formatter<PGUI::Error, Char>
{
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto it = ctx.begin();
		const auto end = ctx.end();
		if (it == end || *it == '}')
		{
			return it;
		}

		outCode = false;
		outSource = false;
		outDetails = false;

		while (it != end && *it != '}')
		{
			const auto current = *it;

			if (current == 'a' || current == 'A')
			{
				std::advance(it, 1);
				if (it != end && *it != '}')
				{
					throw std::format_error{ "All flags are turned on but extra characters found" };
				}

				outCode = true;
				outMessage = true;
				outSource = true;
				outDetails = true;
				outTime = true;
				outCustomMessage = true;
				outSuggest = true;

				break;
			}
			if (current == 'c' || current == 'C')
			{
				outCode = true;
			}
			else if (current == 'm' || current == 'M')
			{
				outMessage = true;
			}
			else if (current == 's' || current == 'S')
			{
				outSource = true;
			}
			else if (current == 'd' || current == 'D')
			{
				outDetails = true;
			}
			else if (current == 't' || current == 'T')
			{
				outTime = true;
			}
			else if (current == 'x' || current == 'X')
			{
				outCustomMessage = true;
			}
			else if (current == 'f' || current == 'F')
			{
				outSuggest = true;
			}
			else
			{
				throw std::format_error{ "Unknown format specifier" };
			}

			std::advance(it, 1);
		}

		return it;
	}

	template <typename FormatContext>
	auto format(const PGUI::Error& error, FormatContext& ctx) const
	{
		if (outCode)
		{
			std::format_to(ctx.out(), "Error Code: 0x{:08X}",
				static_cast<unsigned int>(error.HResult()));
		}
		if (outMessage)
		{
			std::format_to(ctx.out(), "{}Error Message: {}",
				outCode || outMessage ? " | " : "",
				error.Message());
		}
		if (outTime)
		{
			std::format_to(ctx.out(), "{}Time Stamp: {}", outCode || outMessage ? " | " : "",
				error.TimeStamp());
		}
		if (outSource)
		{
			std::format_to(ctx.out(), "{}Source: {}:{} ({})",
				outCode || outMessage || outTime ? " | " : "",
				error.SourceLocation().file_name(),
				error.SourceLocation().line(),
				error.SourceLocation().function_name());
		}
		if (outDetails)
		{
			if (!error.HasDetails())
			{
				std::format_to(ctx.out(), "{}No details specified.",
					outCode || outMessage || outTime || outSource ? " | " : "");
			}
			else
			{
				std::format_to(ctx.out(), "{}Details: ",
					outCode || outMessage || outTime || outSource ? " | " : "");
				std::vector<std::wstring> details;
				details.resize(error.Details().size());

				for (const auto& [index, detail] : error.Details() | views::enumerate)
				{
					details[index] = std::format(L"{}", detail);
				}

				const auto detailString = details
					| views::join_with(std::wstring{ L", " })
					| ranges::to<std::wstring>();
				std::format_to(ctx.out(), "{}",
					PGUI::WStringToString(detailString));
			}
		}
		if (outCustomMessage)
		{
			if (!error.HasCustomMessage())
			{
				std::format_to(ctx.out(), "{}No custom message specified",
					outCode || outMessage || outTime || outSource || outDetails ? " | " : "");
			}
			else
			{
				const auto customMessageString = *error.CustomMessage();

				std::format_to(ctx.out(), L"{}Custom Message: {}",
					outCode || outMessage || outTime || outSource || outDetails ? L" | " : L"", customMessageString);
			}
		}
		if (outSuggest)
		{
			if (!error.HasSuggestions())
			{
				std::format_to(ctx.out(), "{}No suggestions available",
					outCode || outMessage || outTime || outSource || outDetails || outCustomMessage ? " | " : "");
			}
			else
			{
				const auto suggestionsString = error.FixSuggestions()
					| std::views::transform([](const auto& str)
					{
						return std::format(L"\"{}\"", str);
					})
					| std::views::join_with(std::wstring{ L", " })
					| std::ranges::to<std::wstring>();
				
				std::format_to(ctx.out(), "{}Suggestions: {}",
					outCode || outMessage || outTime || outSource || outDetails || outCustomMessage ? " | " : "",
					PGUI::WStringToString(suggestionsString));
			}
		}

		return ctx.out();
	}

	private:
	bool outCode = true;
	bool outMessage = false;
	bool outTime = false;
	bool outSource = true;
	bool outDetails = true;
	bool outCustomMessage = false;
	bool outSuggest = false;
};

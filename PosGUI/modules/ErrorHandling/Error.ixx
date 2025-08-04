module;
#include <Windows.h>

export module PGUI.ErrorHandling:Error;

import std;

import PGUI.Utils;

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
		auto& AddTag(std::wstring_view tag)
		{
			tags.emplace(tag);
			return *this;
		}

		[[nodiscard]] const auto& Tags() const noexcept
		{
			return tags;
		}
		[[nodiscard]] auto HasTags() const noexcept
		{
			return !tags.empty();
		}
		[[nodiscard]] auto HasTag(const std::wstring& tag) const noexcept
		{
			return tags.contains(tag);
		}
		auto& RemoveTag(const std::wstring& tag)
		{
			tags.erase(tag);
			return *this;
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
				tags == other.tags &&
				details == other.details;
		}

		private:
		std::error_code code;
		std::source_location sourceLocation;
		std::chrono::system_clock::time_point timeStamp;
		std::unordered_map<std::wstring, std::wstring> details{ };
		std::unordered_set<std::wstring> tags;
		std::vector<std::wstring> fixSuggestions;
	};
}

export template <typename Char>
class std::formatter<PGUI::Error, Char>
{
	public:
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto iter = ctx.begin();
		const auto end = ctx.end();
		if (iter == end || *iter == '}')
		{
			return iter;
		}

		outCode = false;
		outSource = false;
		outDetails = false;

		while (iter != end && *iter != '}')
		{
			const auto current = *iter;

			if (current == 'a' || current == 'A')
			{
				std::advance(iter, 1);
				if (iter != end && *iter != '}')
				{
					throw std::format_error{ "All flags are turned on but extra characters found" };
				}

				outCode = true;
				outMessage = true;
				outSource = true;
				outDetails = true;
				outTime = true;
				outTags = true;
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
			else if (current == 'g' || current == 'G')
			{
				outTags = true;
			}
			else if (current == 'f' || current == 'F')
			{
				outSuggest = true;
			}
			else
			{
				throw std::format_error{ "Unknown format specifier" };
			}

			std::advance(iter, 1);
		}

		return iter;
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
		if (outTags)
		{
			if (!error.HasTags())
			{
				std::format_to(ctx.out(), "{}No tags specified",
					outCode || outMessage || outTime || outSource || outDetails ? " | " : "");
			}
			else
			{
				const auto tagsString = error.Tags()
					| std::views::join_with(std::wstring{ L", " })
					| std::ranges::to<std::wstring>();

				std::format_to(ctx.out(), "{}Tags: [{}]",
					outCode || outMessage || outTime || outSource || outDetails ? " | " : "",
					PGUI::WStringToString(tagsString));
			}
		}
		if (outSuggest)
		{
			if (!error.HasSuggestions())
			{
				std::format_to(ctx.out(), "{}No suggestions available",
					outCode || outMessage || outTime || outSource || outDetails || outTags ? " | " : "");
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
					outCode || outMessage || outTime || outSource || outDetails || outTags ? " | " : "",
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
	bool outTags = false;
	bool outSuggest = false;
};

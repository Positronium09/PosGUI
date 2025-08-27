module;
#include <Windows.h>

export module PGUI.ErrorHandling:Exception;

import std;

import PGUI.Utils;
import :Error;
import :ErrorCodes;

export namespace PGUI
{
	class Exception final : public std::exception
	{
		public:
		explicit Exception(const std::error_code& code,
			const std::optional<std::wstring_view>& customMessage = std::nullopt,
			const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept :
			error{ code }, stacktrace{ stacktrace }
		{
			if (customMessage.has_value())
			{
				error.SetCustomMessage(customMessage.value());
			}
		}

		explicit Exception(const Error& error,
			const std::optional<std::wstring_view>& customMessage = std::nullopt,
			const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept :
			error{ error }, stacktrace{ stacktrace }
		{
			if (customMessage.has_value())
			{
				this->error.SetCustomMessage(customMessage.value());
			}
		}
		template <PGUIErrorCodeEnum ErrorType>
		explicit Exception(
			const ErrorType error,
			const std::optional<std::wstring_view>& customMessage = std::nullopt,
			const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept :
			Exception{ std::make_error_code(error), customMessage, stacktrace }
		{
		}

		explicit Exception(const std::errc& errc,
			const std::optional<std::wstring_view>& customMessage = std::nullopt,
			const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept :
			Exception{ std::make_error_code(errc), customMessage, stacktrace }
		{
		}
		explicit Exception(const HRESULT hresult,
			const std::optional<std::wstring_view>& customMessage = std::nullopt,
			const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept :
			Exception{ std::error_code{ hresult, std::system_category() }, customMessage, stacktrace }
		{
		}
		explicit Exception(const WINERROR winError,
			const std::optional<std::wstring_view>& customMessage = std::nullopt,
			const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept :
			Exception{ HresultFromWin32(winError), customMessage, stacktrace }
		{
		}

		Exception(const Exception&) = default;
		Exception(Exception&&) noexcept = default;
		auto operator=(const Exception&) -> Exception & = default;
		auto operator=(Exception&&) noexcept -> Exception & = default;

		~Exception() noexcept override = default;

		[[nodiscard]] auto what() const noexcept -> const char* override
		{
			if (error.HasCustomMessage())
			{
				return WStringToString(*error.CustomMessage()).c_str();
			}
			return error.Message().c_str();
		}
		// ReSharper disable once IdentifierTypo
		[[nodiscard]] auto wwhat() const noexcept
		{
			if (error.HasCustomMessage())
			{
				return *error.CustomMessage();
			}
			return StringToWString(error.Message());
		}
		[[nodiscard]] const auto& GetError() const noexcept
		{
			return error;
		}
		[[nodiscard]] const auto& Stacktrace() const noexcept
		{
			return stacktrace;
		}

		[[nodiscard]] auto ErrorCode() const noexcept
		{
			return error.ErrorCode();
		}
		[[nodiscard]] auto HResult() const noexcept
		{
			return error.HResult();
		}
		[[nodiscard]] const auto& SourceLocation() const noexcept
		{
			return error.SourceLocation();
		}
		[[nodiscard]] const auto& TimeStamp() const noexcept
		{
			return error.TimeStamp();
		}
		[[nodiscard]] auto HasDetails() const noexcept -> bool
		{
			return error.HasDetails();
		}
		[[nodiscard]] const auto& Details() const noexcept
		{
			return error.Details();
		}
		auto& AddDetail(const std::wstring_view key, const std::wstring_view value)
		{
			error.AddDetail(key, value);
			return *this;
		}
		auto& SuggestFix(const std::wstring_view suggestion)
		{
			error.SuggestFix(suggestion);
			return *this;
		}

		private:
		Error error;
		std::stacktrace stacktrace;
	};

	inline auto ThrowFailed(
		const HRESULT hr, 
		const std::optional<std::wstring_view>& customMessage = std::nullopt)
	{
		if (FAILED(hr))
		{
			throw Exception{ hr, customMessage };
		}
	}

	inline auto ThrowFailed(
		const WINERROR wr,
		const std::optional<std::wstring_view>& customMessage = std::nullopt)
	{
		if (wr != ERROR_SUCCESS)
		{
			throw Exception{ wr, customMessage };
		}
	}

	template <typename T>
	auto ThrowFailed(
		const Result<T>& result,
		const std::optional<std::wstring_view>& customMessage = std::nullopt
	)
	{
		if (!result.has_value())
		{
			throw Exception{ result.error(), customMessage };
		}
	}

	inline auto ThrowError(
		const Error& error,
		const std::optional<std::wstring_view>& customMessage = std::nullopt
	)
	{
		throw Exception{ error, customMessage };
	}
}

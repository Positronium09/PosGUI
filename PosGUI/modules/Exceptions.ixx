module;
#include <Windows.h>

export module PGUI.Exceptions;

import std;

import PGUI.Utils;

export namespace PGUI
{
	using WINERR = DWORD;

	class Exception : public std::exception
	{
		public:
		Exception() noexcept = default;

		explicit Exception(const std::wstring_view message) noexcept :
			message{ message }
		{ }

		[[nodiscard]] auto what() const noexcept -> const char* override
		{
			return WStringToString(message).c_str();
		}

		private:
		std::wstring message;
	};

	class Win32Exception final : public Exception
	{
		public:
		Win32Exception() noexcept :
			errorCode{ GetLastError() }
		{ }

		explicit Win32Exception(const WINERR errorCode) noexcept :
			Exception{ HResultToString(errorCode) }, errorCode{ errorCode }
		{ }

		Win32Exception(const WINERR errorCode, const std::wstring_view message) noexcept :
			Exception{ message }, errorCode{ errorCode }
		{ }

		[[nodiscard]] auto ErrorCode() const noexcept { return errorCode; }

		private:
		WINERR errorCode = ERROR_SUCCESS;
	};

	class HResultException final : public Exception
	{
		public:
		HResultException() noexcept = default;

		explicit HResultException(const HRESULT hresult) noexcept :
			Exception{ HResultToString(hresult) }, hresult{ hresult }
		{ }

		HResultException(const HRESULT hresult, const std::wstring_view message) noexcept :
			Exception{ message }, hresult{ hresult }
		{ }

		[[nodiscard]] auto HResult() const noexcept { return hresult; }

		private:
		HRESULT hresult = S_OK;
	};

	inline auto ThrowFailed(const HRESULT hr) -> void
	{
		if (FAILED(hr))
		{
			throw HResultException{ hr };
		}
	}

	inline auto ThrowFailed(const WINERR wr) -> void
	{
		if (wr != ERROR_SUCCESS)
		{
			throw Win32Exception{ wr };
		}
	}
}

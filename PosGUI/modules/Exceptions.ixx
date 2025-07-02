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
		Exception(std::wstring_view message) noexcept : 
			message{ message }
		{
		}
		
		[[nodiscard]] auto what() const noexcept -> const char* override
		{
			return WStringToString(message).c_str();
		}

		private:
		std::wstring message;
	};

	class Win32Exception : public Exception
	{
		public:
		Win32Exception() noexcept : 
			errorCode{ GetLastError() }
		{
		}
		Win32Exception(WINERR errorCode) noexcept :
			errorCode{ errorCode }, Exception::Exception{ HResultToString(errorCode) }
		{
		}
		Win32Exception(WINERR errorCode, std::wstring_view message) noexcept :
			Exception{ message }, errorCode{ errorCode }
		{
		}

		[[nodiscard]] auto ErrorCode() const noexcept { return errorCode; }

		private:
		WINERR errorCode = ERROR_SUCCESS;
	};

	class HResultException : public Exception
	{
		public:
		HResultException() noexcept = default;
		HResultException(HRESULT hresult) noexcept :
			hresult{ hresult }, Exception::Exception{ HResultToString(hresult) }
		{
		}
		HResultException(HRESULT hresult, std::wstring_view message) noexcept :
			Exception{ message }, hresult{ hresult }
		{
		}

		[[nodiscard]] auto HResult() const noexcept { return hresult; }
		
		private:
		HRESULT hresult = S_OK;
	};

	inline void ThrowFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw HResultException{ hr };
		}
	}
	inline void ThrowFailed(WINERR wr)
	{
		if (wr != ERROR_SUCCESS)
		{
			throw Win32Exception{ wr };
		}
	}
}

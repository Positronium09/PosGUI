module;
#include <winrt/base.h>

export module PGUI.ErrorHandling;

import std;

export import :Error;
export import :Exception;
export import :Logger;
export import :ErrorCodes;

export namespace PGUI
{
	template <typename F>
	requires std::invocable<F>
	[[nodiscard]] auto WrapThrowing(
		F&& f,
		const std::source_location& location = std::source_location::current()) ->
		Result<std::remove_cvref_t<std::invoke_result_t<F>>>
	{
		try
		{
			if constexpr (std::is_void_v<std::invoke_result_t<F>>)
			{
				std::invoke(std::forward<F>(f));
				return EmptyResult;
			}
			else
			{
				return std::invoke(std::forward<F>(f));
			}
		}
		catch (const Exception& e)
		{
			return Unexpected{ Error{ e.GetError() } };
		}
		catch (const std::exception& e)
		{
			return Unexpected{ Error{ e, location } };
		}
		catch (const winrt::hresult_error& e)
		{
			return Unexpected{ Error{ e, location } };
		}
		catch (...)
		{
			return Unexpected{ Error{ ErrorCode::Failure, location } };
		}
	}
}

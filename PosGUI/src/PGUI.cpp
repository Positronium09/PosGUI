module;
#include <dwmapi.h>
#include <Windows.h>
#include <wrl.h>
#define WIL_SUPPRESS_EXCEPTIONS 0
#include <wil/result_macros.h>
#include <winrt/base.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <winrt/windows.foundation.h>

module PGUI;

import PGUI.Factories;
import PGUI.ErrorHandling;
import PGUI.UI.Theming.SystemTheme;
import PGUI.UI.Theming.Theme;
import PGUI.UI.DirectXCompositionWindow;
import PGUI.UI.Animation;

extern "C" extern __declspec(selectany) void (__stdcall*winrt_throw_hresult_handler)
	(uint32_t lineNumber,
	 const char* fileName, const char* functionName,
	 void* returnAddress, const winrt::hresult result) noexcept;

template <typename T, typename Secret>
struct PublicCast
{
	static inline T m{ };
};

template <typename Secret, auto Member>
struct Accessor
{
	static inline const auto member = PublicCast<decltype(Member), Secret>::m = Member;
};

template struct Accessor<class SLLSecret, &std::source_location::_Line>;
template struct Accessor<class SLCSecret, &std::source_location::_Column>;
template struct Accessor<class SLFISecret, &std::source_location::_File>;
template struct Accessor<class SLFUSecret, &std::source_location::_Function>;

auto ConstructSourceLocationWithCustomData(
	const std::uint_least32_t line,
	const std::uint_least32_t column,
	const char* file,
	const char* function) noexcept
{
	std::source_location location;

	location.*PublicCast<std::uint_least32_t std::source_location::*, SLLSecret>::m = line;
	location.*PublicCast<std::uint_least32_t std::source_location::*, SLCSecret>::m = column;
	location.*PublicCast<const char* std::source_location::*, SLFISecret>::m = file;
	location.*PublicCast<const char* std::source_location::*, SLFUSecret>::m = function;

	return location;
}

auto WILToPGUIError(const wil::FailureInfo& failure) noexcept -> PGUI::Error
{
	using namespace PGUI;

	const auto location =
		ConstructSourceLocationWithCustomData(
			static_cast<std::uint_least32_t>(failure.uLineNumber),
			0,
			failure.pszFile,
			failure.pszFunction
		);

	Error error{ failure.hr, location };
	error.AddDetail(L"Failure ID", std::to_wstring(failure.failureId));
	error.AddDetail(L"Thread ID", std::format(L"{:X}", failure.threadId));
	error.SetCustomMessage(failure.pszMessage);
	error.AddDetail(L"CallContext", StringToWString(failure.pszCallContext));

	return error;
}

auto __stdcall WILLoggingCallback(const wil::FailureInfo& failure) noexcept -> void
{
	using namespace PGUI;

	if (!Logger::IsLoggerPresent())
	{
		return;
	}

	Logger::Error(WILToPGUIError(failure));
}

auto __stdcall WILResultMessageCallback(
	_Inout_ wil::FailureInfo* failure,
	_Inout_updates_opt_z_(cchDebugMessage) PWSTR pszDebugMessage,
	_Pre_satisfies_(cchDebugMessage > 0) size_t cchDebugMessage) noexcept -> void
{
	if (pszDebugMessage == nullptr || cchDebugMessage == 0)
	{
		return;
	}

	pszDebugMessage[0] = L'\0';

	using namespace PGUI;
	auto error = WILToPGUIError(*failure);
	try
	{
		const auto [out, size] = std::format_to_n(pszDebugMessage, cchDebugMessage - 1,
		                                          // ReSharper disable once StringLiteralTypo
		                                          L"PGUI WIL Failure: {:cmsdfx}", error);
		*out = L'\0';
	}
	catch (...)
	{
		const auto hr = StringCchPrintfW(pszDebugMessage, cchDebugMessage,
		                                 L"FAILURE [0x%08X] (Formatting Exception)",
		                                 failure->hr);
		if (Error err{ hr }; error.IsFailure() && Logger::IsLoggerPresent())
		{
			Logger::Error(err, L"catastrophic failure on formatting the string");
		}
	}
}

auto __stdcall WILResultFromCaughtExceptionCallback() noexcept -> HRESULT
{
	try
	{
		throw;
	}
	catch (const PGUI::Exception& exception)
	{
		return exception.HResult();
	}
	catch (...)
	{
		return S_OK;
	}
}

auto WinrtThrowHResultHandler(
	const uint32_t lineNumber, const char* fileName,
	const char* functionName, void* returnAddress, const winrt::hresult result) noexcept
{
	if (!PGUI::Logger::IsLoggerPresent())
	{
		return;
	}

	const auto location = ConstructSourceLocationWithCustomData(
		lineNumber, 0,
		fileName, functionName
	);

	PGUI::Error error{ static_cast<HRESULT>(result), location };
	error.AddDetail(L"Return address", std::format(L"{:p}", returnAddress));

	PGUI::Logger::Error(error, L"From winrt throw handler");
}

namespace PGUI
{
	auto Init() -> void
	{
		winrt::init_apartment();

		wil::SetResultLoggingCallback(WILLoggingCallback);
		wil::SetResultMessageCallback(WILResultMessageCallback);
		wil::SetResultFromCaughtExceptionCallback(WILResultFromCaughtExceptionCallback);

		winrt_throw_hresult_handler = WinrtThrowHResultHandler;

		auto val = FALSE;
		if (const auto res = SetUserObjectInformationW(
				GetCurrentProcess(),
				UOI_TIMERPROC_EXCEPTION_SUPPRESSION,
				&val, sizeof(val));
			res == 0)
		{
			if (Logger::IsLoggerPresent())
			{
				Logger::Error(Error{ GetLastError() });
			}
		}

		(void)Factories::D2DFactory::GetFactory();
		(void)Factories::DXGIFactory::GetFactory();
		(void)Factories::DWriteFactory::GetFactory();
		(void)Factories::WICFactory::GetFactory();
		(void)UI::Animation::AnimationManager::GetGlobalInstance();
		(void)UI::Animation::AnimationTransitionLibrary::GetInstance();

		UI::Theming::SystemTheme::RegisterSystemEventChanges();
		UI::Theming::ThemeContext::InitializeThemes();
		
		if (UI::Theming::SystemTheme::IsDarkMode())
		{
			UI::Theming::ThemeContext::ChangeCurrentTheme(UI::Theming::ThemeContext::DarkTheme);
		}
		else
		{
			UI::Theming::ThemeContext::ChangeCurrentTheme(UI::Theming::ThemeContext::LightTheme);
		}

		UI::Theming::SystemTheme::ColorValuesChanged().AddCallback(
			&UI::Theming::ThemeContext::OnSystemThemeChanged
		);

		auto* awarenessContext = GetDpiAwarenessContextForProcess(nullptr);
		if (const auto awareness = GetAwarenessFromDpiAwarenessContext(awarenessContext);
			awareness != DPI_AWARENESS_PER_MONITOR_AWARE)
		{
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		}

		UI::DirectXCompositionWindow::InitD3D11Device();
		UI::DirectXCompositionWindow::InitDCompDevice();
		UI::DirectXCompositionWindow::InitD2D1Device();
	}
}

export module PGUI.ErrorHandling:Tags;

import std;

export namespace PGUI::ErrorTags
{
	using namespace std::string_view_literals;

	constexpr auto Initialization = L"Initialization"sv;
	constexpr auto Logging = L"Logging"sv;
	constexpr auto Window = L"Window"sv;
	constexpr auto Creation = L"Creation"sv;
	constexpr auto WindowMessage = L"Window Message"sv;
	constexpr auto System = L"System"sv;
	constexpr auto DirectX = L"DirectX"sv;
	constexpr auto Animation = L"Animation"sv;
	constexpr auto D2D = L"Direct2D"sv;
	constexpr auto Direct2D = D2D;
	constexpr auto Imaging = L"Imaging"sv;
	constexpr auto Font = L"Font"sv;
}

export module PGUI.Utils:HashUtils;

import std;

import :MetaUtils;
import :StringUtils;

export template <typename CharT, typename Traits>
struct std::hash<PGUI::basic_zstring_view<CharT, Traits>>
{
	[[nodiscard]] auto operator()(const PGUI::basic_zstring_view<CharT, Traits> sv) const noexcept -> size_t
	{
		return std::hash<std::basic_string_view<CharT, Traits>>{ }(sv);
	}
};

export namespace PGUI
{
	namespace Hash
	{
		constexpr auto GoldenRatioSeed = std::size_t{ 0x9E3779B9 };

		template <typename T>
		auto CombineHash(std::size_t& seed, const T& value) noexcept -> void
		{
			std::hash<T> hasher;

			seed ^= hasher(value) + GoldenRatioSeed + (seed << 6) + (seed >> 2);
		}

		template <typename... ToHash>
		[[nodiscard]] auto ComputeHash(const ToHash&... values) noexcept
		{
			if constexpr (TypeCount<ToHash...> == 1)
			{
				return std::hash<FirstTypeOf<ToHash...>>{ }(std::get<0>(std::forward_as_tuple(values...)));
			}

			std::size_t seed = 0;

			(CombineHash(seed, values), ...);

			return seed;
		}
	}

	struct StringHash
	{
		// ReSharper disable once CppInconsistentNaming
		using is_transparent = void;

		[[nodiscard]] auto operator()(const char* txt) const -> size_t { return std::hash<std::string_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::string_view txt) const -> size_t { return std::hash<std::string_view>{ }(txt); }
		[[nodiscard]] auto operator()(const zstring_view txt) const -> size_t { return std::hash<zstring_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::string& txt) const -> size_t { return std::hash<std::string_view>{ }(txt); }
	};

	struct WStringHash
	{
		// ReSharper disable once CppInconsistentNaming
		using is_transparent = void;

		[[nodiscard]] auto operator()(const wchar_t* txt) const -> size_t { return std::hash<std::wstring_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::wstring_view txt) const -> size_t { return std::hash<std::wstring_view>{ }(txt); }
		[[nodiscard]] auto operator()(const wzstring_view txt) const -> size_t { return std::hash<wzstring_view>{ }(txt); }
		[[nodiscard]] auto operator()(const std::wstring& txt) const -> size_t { return std::hash<std::wstring_view>{ }(txt); }
	};
}

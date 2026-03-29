export module PGUI.Utils:StringUtils;

import std;

export namespace PGUI
{
	// ReSharper disable CppInconsistentNaming
	// ReSharper disable IdentifierTypo

	template <typename CharT, typename Traits = std::char_traits<CharT>>
	struct basic_zstring_view;

	using zstring_view = basic_zstring_view<char>;
	using wzstring_view = basic_zstring_view<wchar_t>;
	using u8zstring_view = basic_zstring_view<char8_t>;
	using u16zstring_view = basic_zstring_view<char16_t>;
	using u32zstring_view = basic_zstring_view<char32_t>;

	template <typename CharT, typename Traits>
	struct basic_zstring_view
	{
		using underlying_view = std::basic_string_view<CharT, Traits>;

		using traits_type = underlying_view::traits_type;
		using value_type = underlying_view::value_type;
		using pointer = underlying_view::pointer;
		using const_pointer = underlying_view::const_pointer;
		using reference = underlying_view::reference;
		using const_reference = underlying_view::const_reference;
		using const_iterator = underlying_view::const_iterator;
		using iterator = underlying_view::iterator;
		using const_reverse_iterator = underlying_view::const_reverse_iterator;
		using reverse_iterator = underlying_view::reverse_iterator;
		using size_type = underlying_view::size_type;
		using difference_type = underlying_view::difference_type;

		static constexpr auto npos = underlying_view::npos;

		constexpr basic_zstring_view() noexcept : 
			underlyingView{ &nullChar, 0 }
		{
		}
		constexpr basic_zstring_view(const basic_zstring_view&) noexcept = default;
		constexpr basic_zstring_view(basic_zstring_view&&) noexcept = default;
		constexpr auto operator=(const basic_zstring_view&) noexcept -> basic_zstring_view& = default;
		constexpr auto operator=(basic_zstring_view&&) noexcept -> basic_zstring_view& = default;

		explicit(false) constexpr basic_zstring_view(const CharT* const ptr) noexcept : 
			underlyingView{ ptr }
		{
		}

		template <std::size_t N>
		explicit(false) constexpr basic_zstring_view(const CharT (&arr)[N]) noexcept : 
			underlyingView{ arr }
		{
		}

		template <typename Allocator>
		explicit(false) constexpr basic_zstring_view(const std::basic_string<CharT, Traits, Allocator>& str) noexcept : 
			underlyingView{ str }
		{
		}

		explicit(false) constexpr basic_zstring_view(std::nullptr_t) noexcept = delete;
		explicit(false) constexpr basic_zstring_view(underlying_view) noexcept = delete;

		[[nodiscard]] constexpr auto c_str() const noexcept -> const_pointer { return underlyingView.data(); }
		[[nodiscard]] constexpr auto data() const noexcept -> const_pointer { return underlyingView.data(); }

		[[nodiscard]] constexpr auto size() const noexcept -> size_type { return underlyingView.size(); }
		[[nodiscard]] constexpr auto length() const noexcept -> size_type { return underlyingView.length(); }
		[[nodiscard]] constexpr auto empty() const noexcept -> bool { return underlyingView.empty(); }
		[[nodiscard]] constexpr auto max_size() const noexcept -> size_type { return underlyingView.max_size(); }

		[[nodiscard]] constexpr auto operator[](size_type pos) const noexcept { return underlyingView[pos]; }
		[[nodiscard]] constexpr auto at(size_type pos) const { return underlyingView.at(pos); }
		[[nodiscard]] constexpr auto front() const noexcept { return underlyingView.front(); }
		[[nodiscard]] constexpr auto back() const noexcept { return underlyingView.back(); }

		[[nodiscard]] constexpr auto begin() const noexcept { return underlyingView.begin(); }
		[[nodiscard]] constexpr auto end() const noexcept { return underlyingView.end(); }
		[[nodiscard]] constexpr auto cbegin() const noexcept { return underlyingView.cbegin(); }
		[[nodiscard]] constexpr auto cend() const noexcept { return underlyingView.cend(); }
		[[nodiscard]] constexpr auto rbegin() const noexcept { return underlyingView.rbegin(); }
		[[nodiscard]] constexpr auto rend() const noexcept { return underlyingView.rend(); }
		[[nodiscard]] constexpr auto crbegin() const noexcept { return underlyingView.crbegin(); }
		[[nodiscard]] constexpr auto crend() const noexcept { return underlyingView.crend(); }

		constexpr auto remove_prefix(size_type n) noexcept { underlyingView.remove_prefix(n); }

		[[nodiscard]] constexpr auto suffix(size_type pos) const
		{
			return basic_zstring_view{ data() + pos, size() - pos };
		}

		[[nodiscard]] constexpr auto find(underlying_view sv, size_type pos = 0) const noexcept { return underlyingView.find(sv, pos); }
		[[nodiscard]] constexpr auto find(CharT ch, size_type pos = 0) const noexcept { return underlyingView.find(ch, pos); }
		[[nodiscard]] constexpr auto rfind(underlying_view sv, size_type pos = npos) const noexcept { return underlyingView.rfind(sv, pos); }
		[[nodiscard]] constexpr auto rfind(CharT ch, size_type pos = npos) const noexcept { return underlyingView.rfind(ch, pos); }
		[[nodiscard]] constexpr auto find_first_of(underlying_view sv, size_type pos = 0) const noexcept { return underlyingView.find_first_of(sv, pos); }
		[[nodiscard]] constexpr auto find_last_of(underlying_view sv, size_type pos = npos) const noexcept { return underlyingView.find_last_of(sv, pos); }
		[[nodiscard]] constexpr auto find_first_not_of(underlying_view sv, size_type pos = 0) const noexcept { return underlyingView.find_first_not_of(sv, pos); }
		[[nodiscard]] constexpr auto find_last_not_of(underlying_view sv, size_type pos = npos) const noexcept { return underlyingView.find_last_not_of(sv, pos); }

		[[nodiscard]] constexpr auto compare(underlying_view sv) const noexcept { return underlyingView.compare(sv); }

		[[nodiscard]] constexpr auto starts_with(underlying_view sv) const noexcept { return underlyingView.starts_with(sv); }
		[[nodiscard]] constexpr auto starts_with(CharT ch) const noexcept { return underlyingView.starts_with(ch); }
		[[nodiscard]] constexpr auto ends_with(underlying_view sv) const noexcept { return underlyingView.ends_with(sv); }
		[[nodiscard]] constexpr auto ends_with(CharT ch) const noexcept { return underlyingView.ends_with(ch); }
		[[nodiscard]] constexpr auto contains(underlying_view sv) const noexcept { return underlyingView.contains(sv); }
		[[nodiscard]] constexpr auto contains(CharT ch) const noexcept { return underlyingView.contains(ch); }

		template <typename Self>
		constexpr auto&& view(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.underlyingView);
		}

		explicit(false) constexpr operator underlying_view() const noexcept
		{
			return underlyingView;
		}

		[[nodiscard]] constexpr auto operator==(const basic_zstring_view& other) const noexcept -> bool = default;
		[[nodiscard]] constexpr auto operator<=>(const basic_zstring_view& other) const noexcept = default;
		[[nodiscard]] constexpr auto operator==(underlying_view other) const noexcept -> bool { return underlyingView == other; }
		[[nodiscard]] constexpr auto operator<=>(underlying_view other) const noexcept { return underlyingView <=> other; }
		[[nodiscard]] friend auto operator==(underlying_view lhs, const basic_zstring_view& rhs) noexcept -> bool { return lhs == rhs.underlyingView; }
		[[nodiscard]] friend auto operator<=>(underlying_view lhs, const basic_zstring_view& rhs) noexcept { return lhs <=> rhs.underlyingView; }
		
		[[nodiscard]] friend auto operator<<(
			std::basic_ostream<CharT, Traits>& os, 
			const basic_zstring_view& str) -> std::basic_ostream<CharT, Traits>&
		{
			return os << str.underlyingView;
		}

		private:
		constexpr basic_zstring_view(const CharT* ptr, size_type count) noexcept : 
			underlyingView{ ptr, count }
		{
		}
		underlying_view underlyingView;
		inline static CharT nullChar = CharT{ };
	};

	// ReSharper restore CppInconsistentNaming
	// ReSharper restore IdentifierTypo

	[[nodiscard]] auto StringToWString(zstring_view string) noexcept -> std::wstring;

	[[nodiscard]] auto WStringToString(wzstring_view string) noexcept -> std::string;
}

export template <typename CharT, typename Traits>
struct std::formatter<PGUI::basic_zstring_view<CharT, Traits>, CharT> :
	std::formatter<std::basic_string_view<CharT, Traits>, CharT>
{
	template <typename FormatContext>
	auto format(const PGUI::basic_zstring_view<CharT, Traits> str, FormatContext& ctx) const
	{
		return std::formatter<std::basic_string_view<CharT, Traits>, CharT>::format(
			str.view(), ctx);
	}
};

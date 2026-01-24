module;
#define WIL_SUPPRESS_EXCEPTIONS 0
#include <bit>
#include <wil/com.h>

export module PGUI.ComPtr;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

export namespace PGUI
{
	template <typename T>
	using ComPtr = wil::com_ptr_nothrow<T>;

	template <typename T, typename... Policies>
	[[nodiscard]] consteval auto GetIID(const wil::com_ptr_t<T, Policies...>&) noexcept
	{
		return __uuidof(T);
	}

	template <typename T>
	[[nodiscard]] consteval auto GetIID() noexcept
	{
		return __uuidof(T);
	}

	template <typename... Interfaces>
	class ComPtrHolder
	{
		static_assert(sizeof...(Interfaces) > 0, "At least one interface must be provided.");

		using FirstType = std::tuple_element_t<0, std::tuple<Interfaces...>>;

		public:
		constexpr ComPtrHolder() noexcept = default;

		explicit(false) ComPtrHolder(const ComPtr<Interfaces>&... args) noexcept :
			interfaces{ std::make_tuple(args...) }
		{ }

		explicit(false) ComPtrHolder(ComPtr<Interfaces>&&... args) noexcept :
			interfaces{ std::make_tuple(std::forward<ComPtr<Interfaces>>(args)...) }
		{ }

		explicit(false) ComPtrHolder(std::nullptr_t) noexcept
		{ }

		template <IsAnyOf<Interfaces...> T>
		constexpr auto operator=(const ComPtr<T>& ptr) -> ComPtrHolder&
		{
			Set(ptr);
			return *this;
		}

		template <IsAnyOf<Interfaces...> T>
		constexpr auto operator=(ComPtr<T>&& ptr) -> ComPtrHolder&
		{
			Set(ptr);
			return *this;
		}

		template <IsAnyOf<Interfaces...> T>
		constexpr auto operator=(T* ptr) -> ComPtrHolder&
		{
			Set(ptr);
			return *this;
		}

		template <IsAnyOf<Interfaces...> T>
		constexpr auto Set(const ComPtr<T>& ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = ptr;
		}

		template <IsAnyOf<Interfaces...> T>
		constexpr auto Set(ComPtr<T>&& ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = std::move(ptr);
		}

		template <IsAnyOf<Interfaces...> T>
		constexpr auto Set(T* ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = ptr;
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr auto& Get() noexcept
		{
			return std::get<ComPtr<T>>(interfaces);
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr const auto& Get() const noexcept
		{
			return std::get<ComPtr<T>>(interfaces);
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto GetAs() const noexcept
		{
			return Get<T>().template try_query<U>();
		}

		template <typename U>
		[[nodiscard]] auto GetAs() const noexcept
		{
			return Get<FirstType>().template try_query<U>();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto PutVoid() noexcept
		{
			return Get<T>().put_void();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto Put() noexcept
		{
			return Get<T>().put();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto PutUnknown() noexcept
		{
			return Get<T>().put_unknown();
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto PutAs() noexcept
		{
			return GetAs<T, U>().put();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto AddressOf() noexcept
		{
			return Get<T>().addressof();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetRaw() const noexcept
		{
			return Get<T>().get();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetRaw() noexcept
		{
			return Get<T>().get();
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto GetRawAs() const noexcept
		{
			return GetAs<T, U>().get();
		}

		template <typename U>
		[[nodiscard]] auto GetRawAs() const noexcept
		{
			return GetAs<U>().get();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto Detach() noexcept
		{
			return Get<T>().detach();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		auto Attach(T* ptr) noexcept -> void
		{
			Get<T>().attach(ptr);
		}

		template <IsAnyOf<Interfaces...> T, IsAnyOf<Interfaces...> U>
		auto AsAssign() noexcept -> Error
		{
			auto ptr = GetAs<T, U>();
			auto error = Error{ ptr.get() == nullptr ? E_NOINTERFACE : S_OK };
			error
				.AddDetail(L"From", StringToWString(typeid(T).name()))
				.AddDetail(L"To", StringToWString(typeid(U).name()));

			if (error.IsFailure())
			{
				Logger::Error(error, L"Cannot cast between interfaces");
				return error;
			}

			Set(ptr);
			return error;
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr auto IsInitialized() const noexcept -> bool
		{
			return Get<T>().get() != nullptr;
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		explicit(false) constexpr operator ComPtr<T>() const noexcept
		{
			return Get<T>();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		explicit(false) constexpr operator T*() const noexcept
		{
			return GetRaw<T>();
		}

		private:
		std::tuple<ComPtr<Interfaces>...> interfaces{ };
	};
}

export template <typename Char>
struct std::formatter<IID, Char>
{
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto iter = ctx.begin();
		const auto end = ctx.end();
		if (iter == end || *iter == '}')
		{
			return iter;
		}

		throw std::format_error{ "No format specifiers is supported" };
	}

	template <typename FormatContext>
	auto format(const IID& iid, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{{{:08X}-{:04X}-{:04X}-{{{:02X}{:02X}}}-{{{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}}}}}",
			iid.Data1, iid.Data2, iid.Data3,
			iid.Data4[0], iid.Data4[1],
			iid.Data4[2], iid.Data4[3], iid.Data4[4], iid.Data4[5], iid.Data4[6], iid.Data4[7]);
	}
};


export template <typename T, typename Char, typename... Policies>
struct std::formatter<wil::com_ptr_t<T, Policies...>, Char>
{
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto iter = ctx.begin();
		const auto end = ctx.end();
		if (iter == end || *iter == '}')
		{
			return iter;
		}
		throw std::format_error{ "No format specifiers is supported" };
	}

	template <typename FormatContext>
	auto format(const wil::com_ptr_t<T, Policies...>& ptr, FormatContext& ctx) const
	{
		std::string_view name = typeid(T).name();

		if (name.starts_with("struct "))
		{
			name.remove_prefix(7);
		}
		else if (name.starts_with("class "))
		{
			name.remove_prefix(6);
		}

		constexpr auto width = 2 * sizeof(nullptr);

		return std::format_to(
			ctx.out(), "ComPtr<{}>(0x{:0{}x})",
			name,
			std::bit_cast<std::uintptr_t>(ptr.get()),
			width);
	}
};
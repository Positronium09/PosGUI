module;
#include <wrl.h>

export module PGUI.ComPtr;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

export namespace PGUI
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

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
			interfaces{ std::make_tuple(std::move(args)...) }
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
			ComPtr<U> ptr;
			Get<T>().As(&ptr);
			return ptr;
		}

		template <typename U>
		[[nodiscard]] auto GetAs() const noexcept
		{
			ComPtr<U> ptr;
			Get<FirstType>().As(&ptr);
			return ptr;
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetVoidAddress() const noexcept
		{
			return std::bit_cast<void**>(Get<T>().GetAddressOf());
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetAddress() const noexcept
		{
			return Get<T>().GetAddressOf();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetAddress() noexcept
		{
			return Get<T>().GetAddressOf();
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto GetAddressAs() const noexcept
		{
			return GetAs<T, U>().GetAddressOf();
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto GetAddressAs() noexcept
		{
			return GetAs<T, U>().GetAddressOf();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetRaw() const noexcept
		{
			return Get<T>().Get();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetRaw() noexcept
		{
			return Get<T>().Get();
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto GetRawAs() const noexcept
		{
			ComPtr<U> ptr;
			Get<T>().As(&ptr);
			return ptr.Get();
		}

		template <typename U>
		[[nodiscard]] auto GetRawAs() const noexcept
		{
			ComPtr<U> ptr;
			Get<FirstType>().As(&ptr);
			return ptr.Get();
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		[[nodiscard]] auto CastRawAs() noexcept
		{
			return std::bit_cast<U**>(Get<T>().GetAddressOf());
		}

		template <typename U>
		[[nodiscard]] auto CastRawAs() noexcept
		{
			return std::bit_cast<U**>(Get<FirstType>().GetAddressOf());
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] auto Detach() noexcept
		{
			return Get<T>().Detach();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		auto Attach(T* ptr) noexcept -> void
		{
			Get<T>().Attach(ptr);
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		auto As() const noexcept -> Result<ComPtr<U>>
		{
			ComPtr<U> ptr;

			if (auto hresult = Get<T>().As(&ptr);
				FAILED(hresult))
			{
				Error error{ hresult };
				error
					.AddDetail(L"From", StringToWString(typeid(T).name()))
					.AddDetail(L"To", StringToWString(typeid(U).name()));

				Logger::Error(error, L"Cannot cast between interfaces");
				return Unexpected{ error };
			}

			return ptr;
		}

		template <IsAnyOf<Interfaces...> T, typename U>
		auto As(const std::nothrow_t&) const -> Result<ComPtr<U>>
		{
			ComPtr<U> ptr;

			if (auto hresult = Get<T>().As(&ptr);
				FAILED(hresult))
			{
				Error error{ hresult };
				error
					.AddDetail(L"From", StringToWString(typeid(T).name()))
					.AddDetail(L"To", StringToWString(typeid(U).name()));
				throw Exception{ error };
			}

			return ptr;
		}

		template <IsAnyOf<Interfaces...> T, IsAnyOf<Interfaces...> U>
		auto AsAssign() noexcept -> Error
		{
			ComPtr<U> ptr;
			auto error = Error{ Get<T>().As(&ptr) };

			if (error.IsSuccess())
			{
				Set(ptr);
			}

			return error;

		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr auto IsInitialized() const noexcept -> bool
		{
			return std::get<ComPtr<T>>(interfaces);
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		explicit(false) constexpr operator ComPtr<T>() const noexcept
		{
			return Get<T>();
		}

		template <IsAnyOf<Interfaces...> T = FirstType>
		explicit(false) constexpr operator T*() const noexcept
		{
			return Get<ComPtr<T>>().Get();
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

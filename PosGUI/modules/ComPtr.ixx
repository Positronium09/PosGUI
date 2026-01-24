module;
#define WIL_SUPPRESS_EXCEPTIONS 0
#include <bit>
#include <wil/com.h>
#include <winrt/base.h>

export module PGUI.ComPtr;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

export namespace PGUI
{
	template <typename T>
	using ComPtr = wil::com_ptr_nothrow<T>;

	template <typename T>
	concept HasUUID = requires { __uuidof(T); };

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

	template <typename T>
	[[nodiscard]] consteval auto GetIID(const T&) noexcept
	{
		return __uuidof(T);
	}

	template <typename T>
	[[nodiscard]] consteval auto GetIID(const T* const) noexcept
	{
		return __uuidof(T);
	}

	// ReSharper disable once CppInconsistentNaming
	template <typename T, typename... Args>
	[[nodiscard]] auto make_self_wil(Args&&... args) noexcept -> ComPtr<T>
	{
		auto rtPtr = winrt::make_self<T>(std::forward<Args>(args)...);

		ComPtr<T> wilPtr;
		wilPtr.attach(rtPtr.detach());

		return wilPtr;
	}

	template <typename T, typename... Args>
	[[nodiscard]] auto MakeComPtr(Args&&... args) noexcept
	{
		auto* raw = new (std::nothrow) T(std::forward<Args>(args)...);
		
		if (raw == nullptr)
		{
			return ComPtr<T>{};
		}

		return ComPtr<T>{ raw };
	}

	template <HasUUID... Interfaces>
	class ComPtrHolder
	{
		static_assert(sizeof...(Interfaces) > 0, "At least one interface must be provided.");

		using FirstType = FirstTypeOf<Interfaces...>;

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

		template <IsInTypeList<Interfaces...> T>
		constexpr auto operator=(const ComPtr<T>& ptr) -> ComPtrHolder&
		{
			Set(ptr);
			return *this;
		}

		template <IsInTypeList<Interfaces...> T>
		constexpr auto operator=(ComPtr<T>&& ptr) -> ComPtrHolder&
		{
			Set(ptr);
			return *this;
		}

		template <IsInTypeList<Interfaces...> T>
		constexpr auto operator=(T* ptr) -> ComPtrHolder&
		{
			Set(ptr);
			return *this;
		}

		template <IsInTypeList<Interfaces...> T>
		constexpr auto Set(const ComPtr<T>& ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = ptr;
		}

		template <IsInTypeList<Interfaces...> T>
		constexpr auto Set(ComPtr<T>&& ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = std::move(ptr);
		}

		template <IsInTypeList<Interfaces...> T>
		constexpr auto Set(T* ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = ptr;
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr auto& Get() noexcept
		{
			return std::get<ComPtr<T>>(interfaces);
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr const auto& Get() const noexcept
		{
			return std::get<ComPtr<T>>(interfaces);
		}

		template <IsInTypeList<Interfaces...> T, typename U>
		[[nodiscard]] auto GetAs() const noexcept
		{
			return Get<T>().template try_query<U>();
		}

		template <typename U>
		[[nodiscard]] auto GetAs() const noexcept
		{
			return Get<FirstType>().template try_query<U>();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto PutVoid() noexcept
		{
			return Get<T>().put_void();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto Put() noexcept
		{
			return Get<T>().put();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto PutUnknown() noexcept
		{
			return Get<T>().put_unknown();
		}

		template <IsInTypeList<Interfaces...> T, typename U>
		[[nodiscard]] auto PutAs() noexcept
		{
			return GetAs<T, U>().put();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto AddressOf() noexcept
		{
			return Get<T>().addressof();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetRaw() const noexcept
		{
			return Get<T>().get();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto GetRaw() noexcept
		{
			return Get<T>().get();
		}

		template <IsInTypeList<Interfaces...> T, typename U>
		[[nodiscard]] auto GetRawAs() const noexcept
		{
			return GetAs<T, U>().get();
		}

		template <typename U>
		[[nodiscard]] auto GetRawAs() const noexcept
		{
			return GetAs<U>().get();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] auto Detach() noexcept
		{
			return Get<T>().detach();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		auto Attach(T* ptr) noexcept -> void
		{
			Get<T>().attach(ptr);
		}

		template <IsInTypeList<Interfaces...> T, IsInTypeList<Interfaces...> U>
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

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr auto IsInitialized() const noexcept -> bool
		{
			return Get<T>().get() != nullptr;
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		explicit(false) constexpr operator ComPtr<T>() const noexcept
		{
			return Get<T>();
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		explicit(false) constexpr operator T*() const noexcept
		{
			return GetRaw<T>();
		}

		private:
		std::tuple<ComPtr<Interfaces>...> interfaces{ };
	};

	template <typename Derived, HasUUID... Interfaces>
	struct Implements : Interfaces...
	{
		virtual ~Implements() noexcept = default;

		auto __stdcall QueryInterface(
			const IID& iid,
			_COM_Outptr_ void** obj) -> HRESULT final
		{
			if (obj == nullptr)
			{
				return E_POINTER;
			}
			if (iid == IID_IUnknown)
			{
				*obj = static_cast<IUnknown*>(static_cast<FirstTypeOf<Interfaces...>*>(this));
				AddRef();
				return S_OK;
			}
			if ((CheckAssign<Interfaces>(iid, obj) || ...))
			{
				AddRef();
				return S_OK;
			}

			*obj = nullptr;
			return E_NOINTERFACE;
		}

		auto __stdcall AddRef() -> ULONG final
		{
			return refCount.fetch_add(1, std::memory_order_relaxed) + 1;
		}

		auto __stdcall Release() -> ULONG final
		{
			const auto count = refCount.fetch_sub(1, std::memory_order_acq_rel) - 1;
			if (count == 0)
			{
				delete static_cast<Derived*>(this);
			}
			return count;
		}

		private:

		template <typename T>
		auto CheckAssign(const IID& iid, void** obj) -> bool
		{
			if (iid == __uuidof(T))
			{
				*obj = static_cast<T*>(this);
				return true;
			}
			return false;
		}

		std::atomic_long refCount{ 1 };
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
module;
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
	concept ComInterface = std::derived_from<T, IUnknown>;

	template <typename T>
	concept WinrtProjection = std::derived_from<T, winrt::Windows::Foundation::IUnknown>;

	template <typename T>
	concept SmartPtrType =
		IsSpecialization<std::remove_cvref_t<T>, wil::com_ptr_t> ||
		IsSpecialization<std::remove_cvref_t<T>, winrt::com_ptr>;

	template <typename T>
	using ComPtr = wil::com_ptr_nothrow<T>;

	template <typename T, typename I>
	concept SmartPtrOf =
		std::same_as<std::remove_cvref_t<T>, ComPtr<I>> ||
		std::same_as<std::remove_cvref_t<T>, winrt::com_ptr<I>>;

	template <typename T>
	concept HasUUID = requires { __uuidof(T); };
}

namespace PGUI::Detail
{
	template <typename Derived, ComInterface... Interfaces> 
	struct ImplementsRawCom : Interfaces...
	{
		virtual ~ImplementsRawCom() noexcept = default;

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

	template <bool, typename Derived, typename... Interfaces>
	struct ImplementsSelector
	{
		using Type = winrt::implements<Derived, Interfaces...>;
	};

	template <typename Derived, typename... Interfaces>
	struct ImplementsSelector<true, Derived, Interfaces...>
	{
		using Type = ImplementsRawCom<Derived, Interfaces...>;
	};

	[[nodiscard]] consteval auto GuidsEqual(const winrt::guid& left, const winrt::guid& right) noexcept -> bool
	{
		return left.Data1 == right.Data1 &&
			left.Data2 == right.Data2 &&
			left.Data3 == right.Data3 &&
			std::ranges::equal(left.Data4, right.Data4);
	}

	template <typename T, typename I>
	concept ComPtrHolderArg = 
		SmartPtrOf<T, I> ||
		std::convertible_to<std::remove_cvref_t<T>, I*>;
}

export namespace PGUI
{
	template <typename T, typename P>
	concept DefaultInterfaceOf =
		ComInterface<T> &&
		WinrtProjection<P> &&
		Detail::GuidsEqual(winrt::guid_of<P>(), winrt::guid{ __uuidof(T) });

	template <typename T, typename... Policies>
	[[nodiscard]] consteval auto GetIID(const wil::com_ptr_t<T, Policies...>&) noexcept
	{
		return __uuidof(T);
	}

	template <HasUUID T>
	[[nodiscard]] consteval auto GetIID(const winrt::com_ptr<T>&) noexcept
	{
		return __uuidof(T);
	}

	template <HasUUID T>
	[[nodiscard]] consteval auto GetIID() noexcept
	{
		return __uuidof(T);
	}

	template <HasUUID T>
	[[nodiscard]] consteval auto GetIID(const T&) noexcept
	{
		return __uuidof(T);
	}

	template <HasUUID T>
	[[nodiscard]] consteval auto GetIID(const T* const) noexcept
	{
		return __uuidof(T);
	}

	template <typename T, typename... Args>
	[[nodiscard]] auto MakeComPtr(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) -> ComPtr<T>
	{
		auto* raw = new (std::nothrow) T(std::forward<Args>(args)...);

		if (raw == nullptr)
		{
			return ComPtr<T>{ };
		}

		ComPtr<T> ptr;
		ptr.attach(raw);

		return ptr;
	}

	template <ComInterface T>
	[[nodiscard]] auto ComPtrToWinrt(const ComPtr<T>& wil) noexcept -> winrt::com_ptr<T>
	{
		winrt::com_ptr<T> ptr;
		ptr.copy_from(wil.get());

		return ptr;
	}

	template <ComInterface T>
	[[nodiscard]] auto ComPtrToWinrt(ComPtr<T>&& wil) noexcept -> winrt::com_ptr<T>
	{
		winrt::com_ptr<T> ptr;
		ptr.attach(wil.detach());

		return ptr;
	}

	template <ComInterface T>
	[[nodiscard]] auto WinrtToComPtr(const winrt::com_ptr<T>& wrt) noexcept -> ComPtr<T>
	{
		return ComPtr<T>{ wrt.get() };
	}

	template <ComInterface T>
	[[nodiscard]] auto WinrtToComPtr(winrt::com_ptr<T>&& wrt) noexcept -> ComPtr<T>
	{
		ComPtr<T> ptr;
		ptr.attach(wrt.detach());

		return ptr;
	}

	template <ComInterface AbiInterface, WinrtProjection P> requires
		DefaultInterfaceOf<AbiInterface, P>
	[[nodiscard]] auto ProjectionToComPtr(const P& projection) noexcept -> ComPtr<AbiInterface>
	{
		return ComPtr<AbiInterface>{
			static_cast<AbiInterface*>(winrt::get_abi(projection))
		};
	}

	template <ComInterface AbiInterface, WinrtProjection P> requires
		DefaultInterfaceOf<AbiInterface, P>
	[[nodiscard]] auto ProjectionToComPtr(P&& projection) noexcept -> ComPtr<AbiInterface>
	{
		ComPtr<AbiInterface> ptr;
		ptr.attach(static_cast<AbiInterface*>(winrt::detach_abi(projection)));

		return ptr;
	}

	template <WinrtProjection P>
	[[nodiscard]] auto ProjectionToUnknown(const P& projection) noexcept -> ComPtr<IUnknown>
	{
		return ComPtr<IUnknown>{ winrt::get_unknown(projection) };
	}

	template <WinrtProjection P, ComInterface T> requires
		DefaultInterfaceOf<T, P>
	[[nodiscard]] auto ComPtrToProjection(const ComPtr<T>& ptr) noexcept -> P
	{
		P projection{ nullptr };
		winrt::copy_from_abi(projection, ptr.get());

		return projection;
	}

	template <WinrtProjection P, ComInterface T> requires
		DefaultInterfaceOf<T, P>
	[[nodiscard]] auto ComPtrToProjection(ComPtr<T>&& ptr) noexcept -> P
	{
		P projection{ nullptr };
		winrt::attach_abi(projection, ptr.detach());

		return projection;
	}

	template <ComInterface AbiInterface, WinrtProjection P> requires
		DefaultInterfaceOf<AbiInterface, P>
	[[nodiscard]] auto ProjectionPut(P& projection) noexcept -> AbiInterface**
	{
		return reinterpret_cast<AbiInterface**>(winrt::put_abi(projection));
	}

	template <typename T>
	[[nodiscard]] auto NormalizeSmartPtr(T&& ptr) noexcept
	{
		if constexpr (IsSpecialization<std::remove_cvref_t<T>, winrt::com_ptr>)
		{
			return WinrtToComPtr(std::forward<T>(ptr));
		}
		else
		{
			return std::forward<T>(ptr);
		}
	}

	template <typename T, typename... Args>
	[[nodiscard]] auto MakeSelf(Args&&... args) noexcept -> Result<winrt::com_ptr<T>>
	{
		try
		{
			return winrt::make_self<T>(std::forward<Args>(args)...);
		}
		catch (const winrt::hresult_error& e)
		{
			return Unexpected{ Error{ static_cast<HRESULT>(e.code()) } };
		}
		catch (const std::exception& e)
		{
			return Unexpected{ Error{ e } };
		}
	}

	template <ComInterface... Interfaces>
	class ComPtrHolder
	{
		static_assert(sizeof...(Interfaces) > 0, "At least one interface must be provided.");

		using FirstType = FirstTypeOf<Interfaces...>;

		public:
		constexpr ComPtrHolder() noexcept = default;

		explicit(false) ComPtrHolder(Detail::ComPtrHolderArg<Interfaces> auto&&... args) noexcept :
			interfaces{ std::make_tuple(std::forward<ComPtr<Interfaces>>(NormalizeSmartPtr(args))...) }
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
			Set(MoveChecked(ptr));
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
			std::get<ComPtr<T>>(interfaces) = MoveChecked(ptr);
		}

		template <IsInTypeList<Interfaces...> T>
		constexpr auto Set(T* ptr) noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces) = ptr;
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		constexpr auto Reset() noexcept -> void
		{
			std::get<ComPtr<T>>(interfaces).reset();
		}

		constexpr auto ResetAll() noexcept -> void
		{
			std::apply([](auto&... ptr)
			{
				(ptr.reset(), ...);
			}, interfaces);
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

		template <IsInTypeList<Interfaces...> T, typename U> requires
			std::derived_from<T, U>
		[[nodiscard]] auto GetUpCast() const noexcept
		{
			return static_cast<U*>(GetRaw<T>());
		}

		template <typename U> requires
			std::derived_from<FirstType, U>
		[[nodiscard]] auto GetUpCast() const noexcept
		{
			return static_cast<U*>(GetRaw<FirstType>());
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
		[[nodiscard]] auto AsAssign() noexcept -> Result<void>
		{
			auto ptr = GetAs<T, U>();

			if (ptr.get() == nullptr)
			{
				Error error{ SystemErrorCode::InterfaceNotSupported };
				error
					.AddDetail(L"From", StringToWString(typeid(T).name()))
					.AddDetail(L"To", StringToWString(typeid(U).name()));
				Logger::Error(error, L"Cannot cast between interfaces");
				return Unexpected{ error };
			}

			Set(ptr);
			return EmptyResult;
		}

		template <IsInTypeList<Interfaces...> T = FirstType>
		[[nodiscard]] constexpr auto IsInitialized() const noexcept -> bool
		{
			return Get<T>().get() != nullptr;
		}

		[[nodiscard]] constexpr auto AreAllInitialized() const noexcept -> bool
		{
			return std::apply([](const auto&... ptr)
			{
				return ((ptr.get() != nullptr) && ...);
			}, interfaces);
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

	template <typename Derived, typename ...Interfaces>
	using Implements = Detail::ImplementsSelector<
		LogicalAnd<ComInterface<Interfaces>...>,
		Derived,
		Interfaces...>::Type;
}

template <typename Char>
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

		throw std::format_error{ "No format specifiers are supported" };
	}

	template <typename FormatContext>
	auto format(const IID& iid, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{{{:08X}-{:04X}-{:04X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}}}",
			iid.Data1, iid.Data2, iid.Data3,
			iid.Data4[0], iid.Data4[1],
			iid.Data4[2], iid.Data4[3], iid.Data4[4], iid.Data4[5], iid.Data4[6], iid.Data4[7]);
	}
};

template <typename T, typename Char, typename... Policies>
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
		throw std::format_error{ "No format specifiers are supported" };
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

template <std::derived_from<IUnknown> T, typename Char>
struct std::formatter<T*, Char> : std::formatter<const void*, Char>
{
	template <typename FormatContext>
	auto format(T* ptr, FormatContext& ctx) const
	{
		return std::formatter<const void*, Char>::format(static_cast<const void*>(ptr), ctx);
	}
};

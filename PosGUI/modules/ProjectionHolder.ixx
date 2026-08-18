module;
#include <winrt/base.h>

export module PGUI.ProjectionHolder;

import std;

import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;

export namespace PGUI
{
	template <typename Derived, typename Base>
	concept DerivedFromProjected =
		std::convertible_to<Derived, Base> &&
		WinrtProjection<Base> &&
		WinrtProjection<Derived>;

	template <WinrtProjection P>
	struct ProjectionHolder
	{
		ProjectionHolder() noexcept = default;

		explicit(false) ProjectionHolder(std::nullptr_t) noexcept
		{ }
		
		explicit ProjectionHolder(P&& projection) noexcept : 
			projection{ MoveChecked(projection) }
		{ }
		explicit ProjectionHolder(const P& projection) noexcept :
			projection{ projection }
		{}

		template <typename Self>
		[[nodiscard]] auto&& Get(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.projection);
		}

		auto operator=(P&& val) noexcept -> ProjectionHolder&
		{
			Set(MoveChecked(val));
			return *this;
		}
		auto operator=(const P& val) noexcept -> ProjectionHolder&
		{
			Set(val);
			return *this;
		}

		auto Set(P&& newValue) noexcept -> void
		{
			projection = MoveChecked(newValue);
		}
		auto Set(const P& newValue) noexcept -> void
		{
			projection = newValue;
		}

		auto Reset() noexcept -> void
		{
			projection = nullptr;
		}

		[[nodiscard]] auto IsInitialized() const noexcept
		{
			return static_cast<bool>(projection);
		}

		private:
		P projection{ nullptr };
	};

	template <typename F>
	[[nodiscard]] auto CallProjected(
		F&& f, 
		const std::source_location& location = std::source_location::current()) noexcept -> Result<std::invoke_result_t<F>>
	{
		try
		{
			if constexpr (std::is_void_v<std::invoke_result_t<F>>)
			{
				std::forward<F>(f)();
				return EmptyResult;
			}
			else { return std::forward<F>(f)(); }
		}
		catch (const winrt::hresult_error& e)
		{
			return Unexpected{
				Error{ std::error_code{ static_cast<int>(e.code()), std::system_category() }, location }
				.SetCustomMessage(e.message().c_str())
			};
		}
	}
}

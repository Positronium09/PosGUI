export module PGUI.Utils;

import std;

export import :WindowsUtils;
export import :StringUtils;
export import :ArithmeticUtils;
export import :EnumUtils;
export import :MetaUtils;
export import :HashUtils;

import PGUI.Mutex;

export namespace PGUI
{
	constexpr auto IsDebugBuild =
		#if defined(_DEBUG) || !defined(NDEBUG)
		true;
		#else
		false;
		#endif

	template <typename T> requires !std::is_same_v<T, void>
	using RawPtr = T*;
	template <typename T> requires !std::is_same_v<T, void>
	using ConstRawPtr = const T*;
	
	enum class PositioningMode
	{
		Absolute,
		Relative
	};

	template <typename T> requires std::is_trivially_copyable_v<T>
	struct ScopedValue
	{
		constexpr ScopedValue(T& variableRef, T newValue) noexcept :
			variable{ variableRef }, oldValue{ std::exchange(variableRef, newValue) }
		{
		}
		constexpr ~ScopedValue() noexcept
		{
			variable.get() = oldValue;
		}

		ScopedValue(const ScopedValue&) = delete;
		auto operator=(const ScopedValue&) -> ScopedValue& = delete;
		ScopedValue(ScopedValue&&) noexcept = delete;
		auto operator=(ScopedValue&&) noexcept -> ScopedValue& = delete;

		private:
		std::reference_wrapper<T> variable;
		T oldValue;
	};

	template <typename Func>
	struct ScopedDefer
	{
		explicit ScopedDefer(Func func) noexcept : func{ std::move(func) }
		{
		}
		~ScopedDefer() noexcept
		{
			func();
		}

		ScopedDefer(const ScopedDefer&) = delete;
		auto operator=(const ScopedDefer&) -> ScopedDefer& = delete;
		ScopedDefer(ScopedDefer&&) noexcept = delete;
		auto operator=(ScopedDefer&&) -> ScopedDefer& = delete;

		private:
		const Func func;
	};

	template <typename T, Mutex::SharedMutexType MutexType>
	struct AccessorProxy
	{
		AccessorProxy(const T& value, MutexType& mutex) :
			value{ value }, lock{ mutex }
		{
		}

		AccessorProxy(const AccessorProxy&) = delete;
		auto operator=(const AccessorProxy&) -> AccessorProxy& = delete;
		AccessorProxy(AccessorProxy&&) = default;
		auto operator=(AccessorProxy&&) -> AccessorProxy& = default;

		[[nodiscard]] auto operator->() const noexcept -> const T*
		{
			return std::addressof(value);
		}
		[[nodiscard]] auto operator*() const noexcept -> const T&
		{
			return value;
		}

		[[nodiscard]] explicit(false) operator const T&() const noexcept
		{
			return value;
		}

		[[nodiscard]] auto Get() const noexcept -> const T&
		{
			return value;
		}

		private:
		const T& value;
		std::shared_lock<MutexType> lock;
	};
}

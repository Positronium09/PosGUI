module;
#include <winrt/base.h>

export module PGUI.Event;

import std;
import PGUI.Mutex;

export namespace PGUI
{
	enum class CallbackPriority
	{
		Low = 0,
		Normal = 1,
		High = 2
	};

	template <typename T, typename... Args>
	concept CallbackType =
		std::invocable<T, Args...> &&
		(std::is_same_v<bool, std::invoke_result_t<T, Args...>> ||
			std::is_same_v<void, std::invoke_result_t<T, Args...>>);
	using CallbackId = std::size_t;

	template <typename MutexType, typename... Args>
	class EventT
	{
		public:
		using CancellingCallback = std::function<bool(Args...)>;
		using NonCancellingCallback = std::function<void(Args...)>;
		using Callback = std::variant<NonCancellingCallback, CancellingCallback>;

		private:
		struct CallbackData
		{
			CallbackId id;
			CallbackPriority priority;
			Callback callback;

			auto operator<=>(const CallbackData& other) const noexcept -> auto
			{
				if (const auto cmp = std::to_underlying(priority) <=> std::to_underlying(other.priority);
					cmp != 0)
				{
					return cmp;
				}
				
				return id <=> other.id;
			}
		};

		public:
		template <CallbackType<Args...> Callable>
		auto AddCallback(const Callable& callback, CallbackPriority priority = CallbackPriority::Normal) noexcept
		{
			using CallbackType = std::conditional_t<
				std::is_same_v<bool, std::invoke_result_t<Callable, Args...>>,
				CancellingCallback,
				NonCancellingCallback>;

			std::scoped_lock lock{ callbackMutex };

			callbacks.emplace_back(nextCallbackId, priority, CallbackType{ callback });

			std::ranges::stable_sort(callbacks, std::ranges::greater{ }, &CallbackData::priority);

			return nextCallbackId++;
		}

		auto RemoveCallback(CallbackId id) noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			std::erase_if(callbacks,
				[id](const CallbackData& data) noexcept
			{
				return data.id == id;
			});
		}

		auto ClearCallbacks() noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			callbacks.clear();
		}

		auto Invoke(Args... args) const noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for ([[maybe_unused]] const auto& [id, priority, callback] : callbacks)
			{
				if (std::holds_alternative<CancellingCallback>(callback))
				{
					auto& cancellingCallback = std::get<CancellingCallback>(callback);
					if (!cancellingCallback(args...))
					{
						return;
					}
				}
				else
				{
					auto& nonCancellingCallback = std::get<NonCancellingCallback>(callback);
					nonCancellingCallback(args...);
				}
			}

		}

		auto InvokeAsync(Args... args) const noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for ([[maybe_unused]] const auto& [id, priority, callback] : callbacks)
			{
				[callbackCopy = callback, ...argsCopy = args] -> winrt::fire_and_forget
				{
					co_await winrt::resume_background();

					if (std::holds_alternative<CancellingCallback>(callbackCopy))
					{
						auto& cancellingCallback = std::get<CancellingCallback>(callbackCopy);
						if (!cancellingCallback(argsCopy...))
						{
							// Asynchronous invocation cannot be cancelled
						}
					}
					else
					{
						auto& nonCancellingCallback = std::get<NonCancellingCallback>(callbackCopy);
						nonCancellingCallback(argsCopy...);
					}
				}();
			}
		}

		EventT() noexcept = default;

		~EventT() = default;

		EventT(EventT&&) = default;

		auto operator=(EventT&&) -> EventT& = default;

		EventT(const EventT&) = delete;

		auto operator=(const EventT&) -> EventT& = delete;

		auto operator+=(const CallbackType<Args...> auto& callback) -> EventT&
		{
			AddCallback(callback);
			return *this;
		}

		private:
		CallbackId nextCallbackId = 0;
		mutable MutexType callbackMutex;
		std::vector<CallbackData> callbacks;
	};

	// ReSharper disable IdentifierTypo
	// ReSharper disable CppInconsistentNaming

	template <typename ...Args>
	using Event = EventT<Mutex::CSMutex, Args...>;
	template <typename ...Args>
	using EventCSM = Event<Args...>;
	template <typename ...Args>
	using EventSRWM = EventT<Mutex::SRWMutex, Args...>;
	template <typename ...Args>
	using EventNM = EventT<Mutex::NullMutex, Args...>;
	template <typename ...Args>
	using EventKM = EventT<Mutex::KMutex, Args...>;

	// ReSharper restore CppInconsistentNaming
	// ReSharper restore IdentifierTypo

	template <typename MutexType, typename... Args>
	class ScopedCallback final
	{
		using EventType = EventT<MutexType, Args...>;

		public:
		ScopedCallback(EventType& event, const CallbackId id) noexcept :
			event{ std::ref(event) }, id{ id }
		{
		}

		~ScopedCallback() noexcept
		{
			if (event.has_value())
			{
				event->get().RemoveCallback(id);
			}
		}

		ScopedCallback(ScopedCallback&& other) noexcept : 
			event{ std::exchange(other.event, std::nullopt) }, id{ other.id }
		{
		}
		auto operator=(ScopedCallback&& other) noexcept -> ScopedCallback&
		{
			std::swap(event, other.event);
			std::swap(id, other.id);

			return *this;
		}

		ScopedCallback(const ScopedCallback&) = delete;

		auto operator=(const ScopedCallback&) -> ScopedCallback& = delete;

		private:
		std::optional<std::reference_wrapper<EventType>> event = std::nullopt;
		CallbackId id{ };
	};
}

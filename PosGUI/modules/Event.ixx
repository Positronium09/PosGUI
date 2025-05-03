module;
#include <vector>
#include <functional>
#include <utility>
#include <unordered_map>
#include <mutex>
#include <concepts>
#include <variant>
#include <ranges>
#include <future>
#include <algorithm>
#include <type_traits>

export module PGUI.Event;

export namespace PGUI
{
	enum class EventPriority
	{
		Low = 0,
		Normal = 1,
		High = 2
	};

	template <typename T, typename ...Args>
	concept CallbackType = std::invocable<T, Args...> &&
		(std::is_same_v<bool, std::invoke_result_t<T, Args...>> ||
			std::is_same_v<void, std::invoke_result_t<T, Args...>>);
	using CallbackId = std::size_t;	

	template <typename ...Args>
	class Event
	{
		public:
		using CancellingCallback = std::function<bool(Args&&...)>;
		using NonCancellingCallback = std::function<void(Args&&...)>;
		using Callback = std::variant<NonCancellingCallback, CancellingCallback>;

		private:
		struct CallbackData
		{
			CallbackId id;
			Callback callback;
		};

		public:
		template <CallbackType<Args...> Callable>
		auto AddCallback(const Callable& callback, EventPriority priority = EventPriority::Normal) noexcept
		{
			using CallbackType = std::conditional_t<
				std::is_same_v<bool, std::invoke_result_t<Callable, Args...>>,
				CancellingCallback,
				NonCancellingCallback>;

			std::lock_guard lock{ callbackMutex };

			CallbackData data{ nextCallbackId, CallbackType{ callback } };
			callbacks.at(priority).push_back(data);

			return nextCallbackId++;
		}

		void RemoveCallback(CallbackId id) noexcept
		{
			std::lock_guard lock{ callbackMutex };

			for (auto& [priority, callbackList] : callbacks)
			{
				auto erased = std::erase_if(callbackList, 
					[id](const CallbackData& data) { return data.id == id; });
				if (erased > 0)
				{
					break;
				}
			}
		}

		void ClearCallbacks() noexcept
		{
			std::lock_guard lock{ callbackMutex };

			callbacks[EventPriority::Low].clear();
			callbacks[EventPriority::Normal].clear();
			callbacks[EventPriority::High].clear();
		}

		void Invoke(Args&&... args) const noexcept
		{
			std::lock_guard lock{ callbackMutex };

			for (auto& [id, callback] : callbacks.at(EventPriority::High))
			{
				if (std::holds_alternative<CancellingCallback>(callback))
				{
					auto& cancellingCallback = std::get<CancellingCallback>(callback);
					if (!cancellingCallback(std::forward<Args>(args)...))
					{
						return;
					}
				}
				else
				{
					auto& nonCancellingCallback = std::get<NonCancellingCallback>(callback);
					nonCancellingCallback(std::forward<Args>(args)...);
				}
			}
			for (auto& [id, callback] : callbacks.at(EventPriority::Normal))
			{
				if (std::holds_alternative<CancellingCallback>(callback))
				{
					auto& cancellingCallback = std::get<CancellingCallback>(callback);
					if (!cancellingCallback(std::forward<Args>(args)...))
					{
						return;
					}
				}
				else
				{
					auto& nonCancellingCallback = std::get<NonCancellingCallback>(callback);
					nonCancellingCallback(std::forward<Args>(args)...);
				}
			}
			for (auto& [id, callback] : callbacks.at(EventPriority::Low))
			{
				if (std::holds_alternative<CancellingCallback>(callback))
				{
					auto& cancellingCallback = std::get<CancellingCallback>(callback);
					if (!cancellingCallback(std::forward<Args>(args)...))
					{
						return;
					}
				}
				else
				{
					auto& nonCancellingCallback = std::get<NonCancellingCallback>(callback);
					nonCancellingCallback(std::forward<Args>(args)...);
				}
			}
		}

		void InvokeAsync(Args&&... args) const noexcept
		{
			std::lock_guard lock{ callbackMutex };

			for (const auto& [priority, callbackList] : callbacks)
			{
				for (const auto& [id, callback] : callbackList)
				{
					auto future = std::async(std::launch::async, [callback, args...]()
					{
						if (std::holds_alternative<CancellingCallback>(callback))
						{
							auto& cancellingCallback = std::get<CancellingCallback>(callback);
							cancellingCallback(std::forward<Args>(args)...);
						}
						else
						{
							auto& nonCancellingCallback = std::get<NonCancellingCallback>(callback);
							nonCancellingCallback(std::forward<Args>(args)...);
						}
					});
				}
			}
		}

		Event() noexcept
		{
			callbacks[EventPriority::Low] = { };
			callbacks[EventPriority::Normal] = { };
			callbacks[EventPriority::High] = { };
		}
		~Event() = default;
		Event(Event&&) = default;
		auto operator=(Event&&) -> Event& = default;
		Event(const Event&) = delete;
		auto operator=(const Event&) -> Event& = delete;

		auto operator+=(const CallbackType<Args...> auto& callback) -> Event&
		{
			AddCallback(callback);
		}

		private:
		CallbackId nextCallbackId = 0;
		mutable std::mutex callbackMutex;
		std::unordered_map<EventPriority, std::vector<CallbackData>> callbacks;
	};

	template <typename ...Args>
	class ScopedCallback final
	{
		public:
		ScopedCallback(Event<Args...>& event, Event<Args...>::CallbackId id) noexcept : 
			event(event), id(id)
		{
		}
		~ScopedCallback() noexcept
		{
			event.RemoveCallback(id);
		}

		ScopedCallback(ScopedCallback&&) = default;
		auto operator=(ScopedCallback&&) -> ScopedCallback& = default;
		ScopedCallback(const ScopedCallback&) = delete;
		auto operator=(const ScopedCallback&) -> ScopedCallback& = delete;

		private:
		Event<Args...>& event;
		Event<Args...>::CallbackId id{ };
	};
}

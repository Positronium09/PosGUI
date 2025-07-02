export module PGUI.Event;

import std;

export namespace PGUI
{
	enum class EventPriority
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

	template <typename... Args>
	class Event
	{
		public:
		using CancellingCallback = std::function<bool(Args...)>;
		using NonCancellingCallback = std::function<void(Args...)>;
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

			std::scoped_lock lock{ callbackMutex };

			CallbackData data{ nextCallbackId, CallbackType{ callback } };
			callbacks[2 - static_cast<int>(priority)].push_back(data);

			return nextCallbackId++;
		}

		auto RemoveCallback(CallbackId id) noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for (auto& callbackList : callbacks)
			{
				auto erased = std::erase_if(
					callbackList,
					[id](const CallbackData& data) { return data.id == id; });
				if (erased > 0)
				{
					break;
				}
			}
		}

		auto ClearCallbacks() noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for (auto& callbackList : callbacks)
			{
				callbackList.clear();
			}
		}

		auto Invoke(Args... args) const noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for (const auto& callbackVector : callbacks)
			{
				for (const auto& [id, callback] : callbackVector)
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
		}

		auto InvokeAsync(Args... args) const noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for (const auto& callbackVector : callbacks)
			{
				for (const auto& [id, callback] : callbackVector)
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

		Event() noexcept = default;

		~Event() = default;

		Event(Event&&) = default;

		auto operator=(Event&&) -> Event & = default;

		Event(const Event&) = delete;

		auto operator=(const Event&) -> Event & = delete;

		auto operator+=(const CallbackType<Args...> auto& callback) -> Event&
		{
			AddCallback(callback);
			return *this;
		}

		private:
		CallbackId nextCallbackId = 0;
		mutable std::mutex callbackMutex;
		std::array<std::vector<CallbackData>, 3> callbacks;
	};

	template <typename... Args>
	class ScopedCallback final
	{
		using EventType = Event<Args...>;

		public:
		ScopedCallback(EventType& event, CallbackId id) noexcept :
			event{ event }, id{ id }
		{
		}

		~ScopedCallback() noexcept
		{
			event.RemoveCallback(id);
		}

		ScopedCallback(ScopedCallback&&) = default;

		auto operator=(ScopedCallback&&) -> ScopedCallback & = default;

		ScopedCallback(const ScopedCallback&) = delete;

		auto operator=(const ScopedCallback&) -> ScopedCallback & = delete;

		private:
		EventType& event;
		CallbackId id{ };
	};
}

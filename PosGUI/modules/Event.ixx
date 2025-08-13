export module PGUI.Event;

import std;

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
			CallbackPriority priority;
			Callback callback;

			auto operator<=>(const CallbackData& other) const noexcept -> std::partial_ordering
			{
				return std::to_underlying(priority) <=> std::to_underlying(other.priority);
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

			CallbackData data{ nextCallbackId, priority, CallbackType{ callback } };
			callbacks.insert(data);

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

		auto InvokeAsync(Args... args) const noexcept -> void
		{
			std::scoped_lock lock{ callbackMutex };

			for ([[maybe_unused]] const auto& [id, priority, callback] : callbacks)
			{
				auto future = std::async(std::launch::async, [callback, args...]
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
		std::set<CallbackData> callbacks;
	};

	template <typename... Args>
	class ScopedCallback final
	{
		using EventType = Event<Args...>;

		public:
		ScopedCallback(EventType& event, const CallbackId id) noexcept :
			event{ event }, id{ id }
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
		EventType& event;
		CallbackId id{ };
	};
}

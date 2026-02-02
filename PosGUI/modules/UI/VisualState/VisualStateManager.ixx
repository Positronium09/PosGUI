export module PGUI.UI.VisualStateManager;

import std;

import PGUI.UI.VisualStateGroup;

import PGUI.Event;
import PGUI.Utils;

export namespace PGUI::UI
{
	template <Enumeration... States>
	class VisualStateManager
	{
		public:
		static constexpr auto StateCount = sizeof...(States);

		explicit VisualStateManager(const States... initialStates) noexcept :
		stateGroups{ VisualStateGroup<States>{ initialStates }... }
		{
		}

		~VisualStateManager() noexcept = default;

		template <std::size_t Index>
		[[nodiscard]] auto& GetStateGroup() noexcept
		{
			return std::get<Index>(stateGroups);
		}
		template <std::size_t Index>
		[[nodiscard]] const auto& GetStateGroup() const noexcept
		{
			return std::get<Index>(stateGroups);
		}
		
		template <std::size_t Index>
		[[nodiscard]] auto& GetStateChangedEvent() noexcept
		{
			return std::get<Index>(stateChangedEvents);
		}
		template <std::size_t Index>
		[[nodiscard]] const auto& GetStateChangedEvent() const noexcept
		{
			return std::get<Index>(stateChangedEvents);
		}

		template <IsInTypeList<States...> State>
		[[nodiscard]] auto& GetStateGroup() noexcept
		{
			return std::get<VisualStateGroup<State>>(stateGroups);
		}
		template <IsInTypeList<States...> State>
		[[nodiscard]] const auto& GetStateGroup() const noexcept
		{
			return std::get<VisualStateGroup<State>>(stateGroups);
		}
		template <IsInTypeList<States...> State>
		[[nodiscard]] auto& GetStateChangedEvent() noexcept
		{
			return std::get<EventSRWM<State>>(stateChangedEvents);
		}
		template <IsInTypeList<States...> State>
		[[nodiscard]] const auto& GetStateChangedEvent() const noexcept
		{
			return std::get<EventSRWM<State>>(stateChangedEvents);
		}

		template <IsInTypeList<States...> State>
		[[nodiscard]] auto GetCurrentState() const noexcept
		{
			return GetStateGroup<State>().GetState();
		}
		template <IsInTypeList<States...> State>
		[[nodiscard]] auto GetCurrentState() noexcept
		{
			return GetStateGroup<State>().GetState();
		}

		template <IsInTypeList<States...> State>
		auto SetCurrentState(const State newState) noexcept
		{
			auto& stateGroup = GetStateGroup<State>();
			const auto oldState = stateGroup.GetState();
			
			if (oldState == newState)
			{
				return;
			}

			if (auto result = stateGroup.TransitionTo(newState);
				result)
			{
				GetStateChangedEvent<State>().Invoke(newState);
			}
		}

		template <IsInTypeList<States...> State>
		auto TransitionTo(const State newState) noexcept
		{
			SetCurrentState<State>(newState);
		}

		private:
		std::tuple<VisualStateGroup<States>...> stateGroups{ };
		std::tuple<EventSRWM<States>...> stateChangedEvents{ };
	};
}

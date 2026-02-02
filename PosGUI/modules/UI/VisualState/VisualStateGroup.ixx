export module PGUI.UI.VisualStateGroup;

import std;

import PGUI.Utils;

export namespace PGUI::UI
{
	template <Enumeration StateT>
	class VisualStateGroup
	{
		public:
		explicit VisualStateGroup(const StateT initialState) noexcept : 
			state{ initialState }
		{
		}
		~VisualStateGroup() noexcept = default;

		auto AllowTransition(const StateT from, StateT to) noexcept -> void
		{
			allowedTransitions[from].emplace_back(to);
		}
		auto DisallowTransition(const StateT from, StateT to) noexcept -> void
		{
			disallowedTransitions[from].emplace_back(to);
		}

		[[nodiscard]] auto IsTransitionAllowed(const StateT from, const StateT to) const noexcept -> bool
		{
			if (disallowedTransitions.contains(from))
			{
				const auto& disallowedVector = disallowedTransitions.at(from);
				if (const auto it = std::ranges::find(disallowedVector, to);
					it != disallowedVector.cend())
				{
					return false;
				}
			}

			if (allowedTransitions.contains(from))
			{
				const auto& allowedVector = allowedTransitions.at(from);
				if (const auto it = std::ranges::find(allowedVector, to);
					it != allowedVector.cend())
				{
					return true;
				}

				return false;
			}

			return true;
		}

		[[nodiscard]] auto IsTransitionDisallowed(const StateT from, const StateT to) const noexcept -> bool
		{
			return !IsTransitionAllowed(from, to);
		}

		[[nodiscard]] auto GetState() const noexcept -> StateT
		{
			return state;
		}

		auto TransitionTo(const StateT newState) noexcept -> bool
		{
			if (IsTransitionAllowed(state, newState))
			{
				state = newState;
				return true;
			}

			return false;
		}

		private:
		std::unordered_map<StateT, std::vector<StateT>> allowedTransitions;
		std::unordered_map<StateT, std::vector<StateT>> disallowedTransitions;
		StateT state;
	};
}

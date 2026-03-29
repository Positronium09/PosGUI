export module PGUI.UI.VisualStateGroup;

import std;

import PGUI.Utils;
import PGUI.ErrorHandling;

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

		auto AllowTransition(const StateT from, StateT to) noexcept -> Error
		{
			try
			{
				allowedTransitions[from].emplace_back(to);
				return Error{ ErrorCode::Success };
			}
			catch (const std::exception& e)
			{
				return Error{ SystemErrorCode::STLFailure }
				.SetCustomMessage(
					StringToWString(e.what()));
			}
		}
		auto DisallowTransition(const StateT from, StateT to) noexcept -> Error
		{
			try
			{
				disallowedTransitions[from].emplace_back(to);
				return Error{ ErrorCode::Success };
			}
			catch (const std::exception& e)
			{
				return Error{ SystemErrorCode::STLFailure }
				.SetCustomMessage(
					StringToWString(e.what()));
			}
		}

		[[nodiscard]] auto IsTransitionAllowed(const StateT from, const StateT to) const noexcept -> bool
		{
			if (const auto disallowedIt = disallowedTransitions.find(from);
				disallowedIt != disallowedTransitions.end())
			{
				const auto& disallowedVector = disallowedIt->second;
				if (const auto it = std::ranges::find(disallowedVector, to);
					it != disallowedVector.cend())
				{
					return false;
				}
			}

			if (const auto allowedIt = allowedTransitions.find(from);
				allowedIt != allowedTransitions.end())
			{
				const auto& allowedVector = allowedIt->second;
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

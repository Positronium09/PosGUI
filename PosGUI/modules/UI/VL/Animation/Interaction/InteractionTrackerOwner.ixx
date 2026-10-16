module;
#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Foundation.Numerics.h>

#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Interaction.InteractionTrackerOwner;

import std;

import PGUI.ComPtr;
import PGUI.Wrapper;
import PGUI.ProjectionHolder;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.Shape;
import PGUI.Mutex;

namespace WUCI = winrt::Windows::UI::Composition::Interactions;

namespace PGUI::UI::VL
{
	class InteractionTrackerOwnerRouter;
}

export namespace PGUI::UI::VL
{
	struct CustomAnimationStateEnteredArgs
	{
		friend class InteractionTrackerOwnerRouter;

		[[nodiscard]] auto IsFromBinding() const noexcept
		{
			return isFromBinding;
		}

		[[nodiscard]] auto GetRequestId() const noexcept
		{
			return requestId;
		}

		private:
		bool isFromBinding;
		std::int32_t requestId;

		CustomAnimationStateEnteredArgs(const bool isFromBinding, const std::int32_t requestId) noexcept : 
			isFromBinding{ isFromBinding },
			requestId{ requestId }
		{ }
	};

	struct IdleStateEnteredArgs
	{
		friend class InteractionTrackerOwnerRouter;

		[[nodiscard]] auto IsFromBinding() const noexcept
		{
			return isFromBinding;
		}

		[[nodiscard]] auto GetRequestId() const noexcept
		{
			return requestId;
		}

		private:
		bool isFromBinding;
		std::int32_t requestId;

		IdleStateEnteredArgs(const bool isFromBinding, const std::int32_t requestId) noexcept :
			isFromBinding{ isFromBinding },
			requestId{ requestId }
		{ }
	};

	struct InertiaStateEnteredArgs
	{
		friend class InteractionTrackerOwnerRouter;

		[[nodiscard]] auto IsFromBinding() const noexcept
		{
			return isFromBinding;
		}
		[[nodiscard]] auto IsInertiaFromImpulse() const noexcept
		{
			return isInertiaFromImpulse;
		}
		[[nodiscard]] auto GetModifiedRestingPosition() const noexcept
		{
			return modifiedRestingPosition;
		}
		[[nodiscard]] auto GetModifiedRestingScale() const noexcept
		{
			return modifiedRestingScale;
		}
		[[nodiscard]] auto GetNaturalRestingPosition() const noexcept
		{
			return naturalRestingPosition;
		}
		[[nodiscard]] auto GetNaturalRestingScale() const noexcept
		{
			return naturalRestingScale;
		}
		[[nodiscard]] auto GetPositionVelocityInPixelsPerSecond() const noexcept
		{
			return positionVelocityInPixelsPerSecond;
		}
		[[nodiscard]] auto GetRequestId() const noexcept
		{
			return requestId;
		}
		[[nodiscard]] auto GetScaleVelocityInPercentPerSecond() const noexcept
		{
			return scaleVelocityInPercentPerSecond;
		}

		private:
		bool isFromBinding;
		bool isInertiaFromImpulse;
		std::optional<Point3F> modifiedRestingPosition;
		std::optional<float> modifiedRestingScale;
		Point3F naturalRestingPosition;
		float naturalRestingScale;
		Point3F positionVelocityInPixelsPerSecond;
		std::int32_t requestId;
		float scaleVelocityInPercentPerSecond;

		InertiaStateEnteredArgs(
			const bool isFromBinding,
			const bool isInertiaFromImpulse,
			const std::optional<Point3F>& modifiedRestingPosition,
			const std::optional<float>& modifiedRestingScale,
			const Point3F& naturalRestingPosition,
			const float naturalRestingScale,
			const Point3F& positionVelocityInPixelsPerSecond,
			const std::int32_t requestId,
			const float scaleVelocityInPercentPerSecond) noexcept : 
			isFromBinding{ isFromBinding },
			isInertiaFromImpulse{ isInertiaFromImpulse },
			modifiedRestingPosition{ modifiedRestingPosition },
			modifiedRestingScale{ modifiedRestingScale },
			naturalRestingPosition{ naturalRestingPosition },
			naturalRestingScale{ naturalRestingScale },
			positionVelocityInPixelsPerSecond{ positionVelocityInPixelsPerSecond },
			requestId{ requestId },
			scaleVelocityInPercentPerSecond{ scaleVelocityInPercentPerSecond }
		{ }
	};

	struct InteractingStateEnteredArgs
	{
		friend class InteractionTrackerOwnerRouter;

		[[nodiscard]] auto IsFromBinding() const noexcept
		{
			return isFromBinding;
		}

		[[nodiscard]] auto GetRequestId() const noexcept
		{
			return requestId;
		}

		private:
		bool isFromBinding;
		std::int32_t requestId;

		InteractingStateEnteredArgs(const bool isFromBinding, const std::int32_t requestId) noexcept :
			isFromBinding{ isFromBinding },
			requestId{ requestId }
		{ }
	};

	struct RequestIgnoredArgs
	{
		friend class InteractionTrackerOwnerRouter;

		[[nodiscard]] auto GetRequestId() const noexcept
		{
			return requestId;
		}

		private:
		std::int32_t requestId;

		explicit RequestIgnoredArgs(const std::int32_t requestId) noexcept :
			requestId{ requestId }
		{ }
	};

	struct ValuesChangedArgs
	{
		friend class InteractionTrackerOwnerRouter;

		[[nodiscard]] auto GetPosition() const noexcept
		{
			return position;
		}
		[[nodiscard]] auto GetRequestId() const noexcept
		{
			return requestId;
		}
		[[nodiscard]] auto GetScale() const noexcept
		{
			return scale;
		}

		private:
		Point3F position;
		std::int32_t requestId;
		float scale;

		ValuesChangedArgs(
			const Point3F& position,
			const std::int32_t requestId,
			const float scale) noexcept :
			position{ position },
			requestId{ requestId },
			scale{ scale }
		{ }
	};
}

namespace PGUI::UI::VL
{
	class InteractionTrackerOwnerRouter : public Implements<InteractionTrackerOwnerRouter, WUCI::IInteractionTrackerOwner>
	{
		using CustomAnimationStateEnteredHandler = std::function<Result<void>(const Wrapper<WUCI::InteractionTracker>&, CustomAnimationStateEnteredArgs)>;
		using IdleStateEnteredHandler = std::function<Result<void>(const Wrapper<WUCI::InteractionTracker>&, IdleStateEnteredArgs)>;
		using InertiaStateEnteredHandler = std::function<Result<void>(const Wrapper<WUCI::InteractionTracker>&, InertiaStateEnteredArgs)>;
		using InteractingStateEnteredHandler = std::function<Result<void>(const Wrapper<WUCI::InteractionTracker>&, InteractingStateEnteredArgs)>;
		using RequestIgnoredHandler = std::function<Result<void>(const Wrapper<WUCI::InteractionTracker>&, RequestIgnoredArgs)>;
		using ValuesChangedHandler = std::function<Result<void>(const Wrapper<WUCI::InteractionTracker>&, ValuesChangedArgs)>;

		public:
		auto CustomAnimationStateEntered(
			const WUCI::InteractionTracker& sender,
			const WUCI::InteractionTrackerCustomAnimationStateEnteredArgs& args) -> void;
		auto IdleStateEntered(
			const WUCI::InteractionTracker& sender,
		    const WUCI::InteractionTrackerIdleStateEnteredArgs& args) -> void;
		auto InertiaStateEntered(
			const WUCI::InteractionTracker& sender,
		    const WUCI::InteractionTrackerInertiaStateEnteredArgs& args) -> void;
		auto InteractingStateEntered(
			const WUCI::InteractionTracker& sender,
		    const WUCI::InteractionTrackerInteractingStateEnteredArgs& args) -> void;
		auto RequestIgnored(
			const WUCI::InteractionTracker& sender,
		    const WUCI::InteractionTrackerRequestIgnoredArgs& args) -> void;
		auto ValuesChanged(
			const WUCI::InteractionTracker& sender,
		    const WUCI::InteractionTrackerValuesChangedArgs& args) -> void;

		auto SetCustomAnimationStateEnteredHandler(const CustomAnimationStateEnteredHandler& handler) noexcept -> void;
		auto SetIdleStateEnteredHandler(const IdleStateEnteredHandler& handler) noexcept -> void;
		auto SetInertiaStateEnteredHandler(const InertiaStateEnteredHandler& handler) noexcept -> void;
		auto SetInteractingStateEnteredHandler(const InteractingStateEnteredHandler& handler) noexcept -> void;
		auto SetRequestIgnoredHandler(const RequestIgnoredHandler& handler) noexcept -> void;
		auto SetValuesChangedHandler(const ValuesChangedHandler& handler) noexcept -> void;

		auto ClearHandlers() noexcept -> void;

		private:
		Mutex::CSMutex customAnimationStateEnteredMutex;
		Mutex::CSMutex idleStateEnteredMutex;
		Mutex::CSMutex inertiaStateEnteredMutex;
		Mutex::CSMutex interactingStateEnteredMutex;
		Mutex::CSMutex requestIgnoredMutex;
		Mutex::CSMutex valuesChangedMutex;
		CustomAnimationStateEnteredHandler customAnimationStateEnteredHandler{ };
		IdleStateEnteredHandler idleStateEnteredHandler{ };
		InertiaStateEnteredHandler inertiaStateEnteredHandler{ };
		InteractingStateEnteredHandler interactingStateEnteredHandler{ };
		RequestIgnoredHandler requestIgnoredHandler{ };
		ValuesChangedHandler valuesChangedHandler{ };
	};
}

export namespace PGUI::UI::VL
{
	class InteractionTrackerOwner
	{
		public:
		template <std::derived_from<InteractionTrackerOwner> T, typename... Args>
		[[nodiscard]] static auto Create(Args&&... args) noexcept -> Result<T>
		{
			auto ownerResult = WrapThrowing([&args...]
			{
				return T{ std::forward<Args>(args)... };
			});
			if (!ownerResult.has_value())
			{
				return Unexpected{ ownerResult.error() };
			}

			if (const auto wireResult = ownerResult.value().WireRouter();
				!wireResult.has_value())
			{
				return Unexpected{ wireResult.error() };
			}

			return MoveChecked(ownerResult.value());
		}

		virtual ~InteractionTrackerOwner() noexcept;

		InteractionTrackerOwner(const InteractionTrackerOwner&) = delete;
		auto operator=(const InteractionTrackerOwner&) -> InteractionTrackerOwner& = delete;

		InteractionTrackerOwner(InteractionTrackerOwner&& other) noexcept;
		auto operator=(InteractionTrackerOwner&& other) noexcept -> InteractionTrackerOwner&;

		[[nodiscard]] auto& GetRouter() const noexcept { return router; }

		virtual auto OnCustomAnimationStateEntered(const Wrapper<WUCI::InteractionTracker>&, CustomAnimationStateEnteredArgs) noexcept -> Result<void>
		{
			return EmptyResult;
		}
		virtual auto OnIdleStateEntered(const Wrapper<WUCI::InteractionTracker>&, IdleStateEnteredArgs) noexcept -> Result<void>
		{
			return EmptyResult;
		}
		virtual auto OnInertiaStateEntered(const Wrapper<WUCI::InteractionTracker>&, InertiaStateEnteredArgs) noexcept -> Result<void>
		{
			return EmptyResult;
		}
		virtual auto OnInteractingStateEntered(const Wrapper<WUCI::InteractionTracker>&, InteractingStateEnteredArgs) noexcept -> Result<void>
		{
			return EmptyResult;
		}
		virtual auto OnRequestIgnored(const Wrapper<WUCI::InteractionTracker>&, RequestIgnoredArgs) noexcept -> Result<void>
		{
			return EmptyResult;
		}
		virtual auto OnValuesChanged(const Wrapper<WUCI::InteractionTracker>&, ValuesChangedArgs) noexcept -> Result<void>
		{
			return EmptyResult;
		}

		protected:
		InteractionTrackerOwner() noexcept = default;

		private:
		winrt::com_ptr<InteractionTrackerOwnerRouter> router;

		auto InstallHandlers() noexcept -> void;

		[[nodiscard]] auto WireRouter() noexcept -> Result<void>;

		auto CallOnCustomAnimationStateEntered(const Wrapper<WUCI::InteractionTracker>& tracker, CustomAnimationStateEnteredArgs args) noexcept -> Result<void>;
		auto CallOnIdleStateEntered(const Wrapper<WUCI::InteractionTracker>& tracker, IdleStateEnteredArgs args) noexcept -> Result<void>;
		auto CallOnInertiaStateEntered(const Wrapper<WUCI::InteractionTracker>& tracker, InertiaStateEnteredArgs args) noexcept -> Result<void>;
		auto CallOnInteractingStateEntered(const Wrapper<WUCI::InteractionTracker>& tracker, InteractingStateEnteredArgs args) noexcept -> Result<void>;
		auto CallOnRequestIgnored(const Wrapper<WUCI::InteractionTracker>& tracker, RequestIgnoredArgs args) noexcept -> Result<void>;
		auto CallOnValuesChanged(const Wrapper<WUCI::InteractionTracker>& tracker, ValuesChangedArgs args) noexcept -> Result<void>;
	};
}

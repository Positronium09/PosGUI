module;
#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Foundation.Numerics.h>

#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

#include <winrt/Windows.Foundation.Collections.h>

module PGUI.UI.VL.Interaction.InteractionTrackerOwner;

import std;

import PGUI.ComPtr;
import PGUI.ProjectionHolder;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.Shape;
import PGUI.Mutex;

namespace WFN = winrt::Windows::Foundation::Numerics;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;

namespace PGUI::UI::VL
{
	auto InteractionTrackerOwnerRouter::CustomAnimationStateEntered(
		const WUCI::InteractionTracker& sender,
		const WUCI::InteractionTrackerCustomAnimationStateEnteredArgs& args) -> void
	{
		std::scoped_lock lock{ customAnimationStateEnteredMutex };
		if (!customAnimationStateEnteredHandler)
		{
			Logger::Trace(L"Custom animation state entered without a handler.");
			return;
		}

		const auto result = CallProjected([this, &sender, &args]
		{
			return customAnimationStateEnteredHandler(
				Wrapper<WUCI::InteractionTracker>{ sender },
				CustomAnimationStateEnteredArgs{
					args.IsFromBinding(),
					args.RequestId()
				});
		}).and_then(std::identity{ });

		if (!result.has_value())
		{
			Logger::Trace(result.error(), L"Custom animation state entered handler failed.");
		}
	}

	auto InteractionTrackerOwnerRouter::IdleStateEntered(
		const WUCI::InteractionTracker& sender,
		const WUCI::InteractionTrackerIdleStateEnteredArgs& args) -> void
	{
		std::scoped_lock lock{ idleStateEnteredMutex };
		if (!idleStateEnteredHandler)
		{
			Logger::Trace(L"Idle state entered without a handler.");
			return;
		}

		const auto result = CallProjected([this, &sender, &args]
		{
			return idleStateEnteredHandler(
				Wrapper<WUCI::InteractionTracker>{ sender },
				IdleStateEnteredArgs{
					args.IsFromBinding(),
					args.RequestId()
				});
		}).and_then(std::identity{ });

		if (!result.has_value())
		{
			Logger::Trace(result.error(), L"Idle state entered handler failed.");
		}
	}

	auto InteractionTrackerOwnerRouter::InertiaStateEntered(
		const WUCI::InteractionTracker& sender,
		const WUCI::InteractionTrackerInertiaStateEnteredArgs& args) -> void
	{
		std::scoped_lock lock{ inertiaStateEnteredMutex };
		if (!inertiaStateEnteredHandler)
		{
			Logger::Trace(L"Inertia state entered without a handler.");
			return;
		}

		const auto result = CallProjected([this, &sender, &args]
		{
			return inertiaStateEnteredHandler(
				Wrapper<WUCI::InteractionTracker>{ sender },
				InertiaStateEnteredArgs{
					args.IsFromBinding(),
					args.IsInertiaFromImpulse(),
					std::optional<WFN::float3>{ args.ModifiedRestingPosition() },
					args.ModifiedRestingScale(),
					args.NaturalRestingPosition(),
					args.NaturalRestingScale(),
					args.PositionVelocityInPixelsPerSecond(),
					args.RequestId(),
					args.ScaleVelocityInPercentPerSecond()
				});
		}).and_then(std::identity{ });

		if (!result.has_value())
		{
			Logger::Trace(result.error(), L"Inertia state entered handler failed.");
		}
	}

	auto InteractionTrackerOwnerRouter::InteractingStateEntered(
		const WUCI::InteractionTracker& sender,
		const WUCI::InteractionTrackerInteractingStateEnteredArgs& args) -> void
	{
		std::scoped_lock lock{ interactingStateEnteredMutex };
		if (!interactingStateEnteredHandler)
		{
			Logger::Trace(L"Interacting state entered without a handler.");
			return;
		}

		const auto result = CallProjected([this, &sender, &args]
		{
			return interactingStateEnteredHandler(
				Wrapper<WUCI::InteractionTracker>{ sender },
				InteractingStateEnteredArgs{
					args.IsFromBinding(),
					args.RequestId()
				});
		}).and_then(std::identity{ });

		if (!result.has_value())
		{
			Logger::Trace(result.error(), L"Interacting state entered handler failed.");
		}
	}

	auto InteractionTrackerOwnerRouter::RequestIgnored(
		const WUCI::InteractionTracker& sender,
		const WUCI::InteractionTrackerRequestIgnoredArgs& args) -> void
	{
		std::scoped_lock lock{ requestIgnoredMutex };
		if (!requestIgnoredHandler)
		{
			Logger::Trace(L"Request ignored without a handler.");
			return;
		}

		const auto result = CallProjected([this, &sender, &args]
		{
			return requestIgnoredHandler(
				Wrapper<WUCI::InteractionTracker>{ sender },
				RequestIgnoredArgs{
					args.RequestId()
				});
		}).and_then(std::identity{ });

		if (!result.has_value())
		{
			Logger::Trace(result.error(), L"Request ignored handler failed.");
		}
	}

	auto InteractionTrackerOwnerRouter::ValuesChanged(
		const WUCI::InteractionTracker& sender,
		const WUCI::InteractionTrackerValuesChangedArgs& args) -> void
	{
		std::scoped_lock lock{ valuesChangedMutex };
		if (!valuesChangedHandler)
		{
			Logger::Trace(L"Values changed without a handler.");
			return;
		}

		const auto result = CallProjected([this, &sender, &args]
		{
			return valuesChangedHandler(
				Wrapper<WUCI::InteractionTracker>{ sender },
				ValuesChangedArgs{
					args.Position(),
					args.RequestId(),
					args.Scale()
				});
		}).and_then(std::identity{ });

		if (!result.has_value())
		{
			Logger::Trace(result.error(), L"Values changed handler failed.");
		}
	}

	auto InteractionTrackerOwnerRouter::SetCustomAnimationStateEnteredHandler(
		const CustomAnimationStateEnteredHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ customAnimationStateEnteredMutex };
		customAnimationStateEnteredHandler = handler;
	}

	auto InteractionTrackerOwnerRouter::SetIdleStateEnteredHandler(
		const IdleStateEnteredHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ idleStateEnteredMutex };
		idleStateEnteredHandler = handler;
	}

	auto InteractionTrackerOwnerRouter::SetInertiaStateEnteredHandler(
		const InertiaStateEnteredHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ inertiaStateEnteredMutex };
		inertiaStateEnteredHandler = handler;
	}

	auto InteractionTrackerOwnerRouter::SetInteractingStateEnteredHandler(
		const InteractingStateEnteredHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ interactingStateEnteredMutex };
		interactingStateEnteredHandler = handler;
	}

	auto InteractionTrackerOwnerRouter::SetRequestIgnoredHandler(const RequestIgnoredHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ requestIgnoredMutex };
		requestIgnoredHandler = handler;
	}

	auto InteractionTrackerOwnerRouter::SetValuesChangedHandler(const ValuesChangedHandler& handler) noexcept -> void
	{
		std::scoped_lock lock{ valuesChangedMutex };
		valuesChangedHandler = handler;
	}

	auto InteractionTrackerOwnerRouter::ClearHandlers() noexcept -> void
	{
		SetCustomAnimationStateEnteredHandler(nullptr);
		SetIdleStateEnteredHandler(nullptr);
		SetInertiaStateEnteredHandler(nullptr);
		SetInteractingStateEnteredHandler(nullptr);
		SetRequestIgnoredHandler(nullptr);
		SetValuesChangedHandler(nullptr);
	}

	auto InteractionTrackerOwner::InstallHandlers() noexcept -> void
	{
		router->SetCustomAnimationStateEnteredHandler(
			std::bind_front(&InteractionTrackerOwner::CallOnCustomAnimationStateEntered, this));
		router->SetIdleStateEnteredHandler(std::bind_front(&InteractionTrackerOwner::CallOnIdleStateEntered, this));
		router->SetInertiaStateEnteredHandler(
			std::bind_front(&InteractionTrackerOwner::CallOnInertiaStateEntered, this));
		router->SetInteractingStateEnteredHandler(
			std::bind_front(&InteractionTrackerOwner::CallOnInteractingStateEntered, this));
		router->SetRequestIgnoredHandler(std::bind_front(&InteractionTrackerOwner::CallOnRequestIgnored, this));
		router->SetValuesChangedHandler(std::bind_front(&InteractionTrackerOwner::CallOnValuesChanged, this));
	}

	auto InteractionTrackerOwner::WireRouter() noexcept -> Result<void>
	{
		auto routerResult = MakeSelf<InteractionTrackerOwnerRouter>();
		if (!routerResult.has_value())
		{
			return Unexpected{ routerResult.error() };
		}

		router = MoveChecked(routerResult.value());
		InstallHandlers();

		return EmptyResult;
	}

	InteractionTrackerOwner::~InteractionTrackerOwner() noexcept
	{
		if (router)
		{
			router->ClearHandlers();
		}
	}

	InteractionTrackerOwner::InteractionTrackerOwner(InteractionTrackerOwner&& other) noexcept :
		router{ MoveChecked(other.router) }
	{
		if (router)
		{
			InstallHandlers();
		}
	}

	auto InteractionTrackerOwner::operator=(InteractionTrackerOwner&& other) noexcept -> InteractionTrackerOwner&
	{
		if (this == &other)
		{
			return *this;
		}

		if (router)
		{
			router->ClearHandlers();
		}

		router = MoveChecked(other.router);
		if (router)
		{
			InstallHandlers();
		}

		return *this;
	}

	auto InteractionTrackerOwner::CallOnCustomAnimationStateEntered(
		const Wrapper<WUCI::InteractionTracker>& tracker,
		const CustomAnimationStateEnteredArgs args) noexcept -> Result<void>
	{
		return OnCustomAnimationStateEntered(tracker, args);
	}

	auto InteractionTrackerOwner::CallOnIdleStateEntered(
		const Wrapper<WUCI::InteractionTracker>& tracker,
		const IdleStateEnteredArgs args) noexcept -> Result<void>
	{
		return OnIdleStateEntered(tracker, args);
	}

	auto InteractionTrackerOwner::CallOnInertiaStateEntered(
		const Wrapper<WUCI::InteractionTracker>& tracker,
		const InertiaStateEnteredArgs args) noexcept -> Result<void>
	{
		return OnInertiaStateEntered(tracker, args);
	}

	auto InteractionTrackerOwner::CallOnInteractingStateEntered(
		const Wrapper<WUCI::InteractionTracker>& tracker,
		const InteractingStateEnteredArgs args) noexcept -> Result<void>
	{
		return OnInteractingStateEntered(tracker, args);
	}

	auto InteractionTrackerOwner::CallOnRequestIgnored(
		const Wrapper<WUCI::InteractionTracker>& tracker,
		const RequestIgnoredArgs args) noexcept -> Result<void>
	{
		return OnRequestIgnored(tracker, args);
	}

	auto InteractionTrackerOwner::CallOnValuesChanged(
		const Wrapper<WUCI::InteractionTracker>& tracker,
		const ValuesChangedArgs args) noexcept -> Result<void>
	{
		return OnValuesChanged(tracker, args);
	}
}

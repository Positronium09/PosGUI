module;
#include <Windows.h>

#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

#include <winrt/Windows.Foundation.Collections.h>

#include <windows.ui.composition.interop.h>

export module PGUI.UI.VL.Interaction.VisualInteractionSource;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.Interaction.ConditionalValue;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.Visual.Visual;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;
namespace AWUCI = ABI::Windows::UI::Composition::Interactions;

export namespace PGUI::UI::VL
{
	class InteractionSourceConfiguration : public CompositionObjectT<WUCI::InteractionSourceConfiguration>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] auto GetPositionXSourceMode() const noexcept -> Result<InteractionSourceRedirectionMode>
		{
			return CallProjected([this]
			{
				return Get().PositionXSourceMode();
			});
		}
		[[nodiscard]] auto SetPositionXSourceMode(const InteractionSourceRedirectionMode positionXSourceMode) const noexcept -> Result<void>
		{
			return CallProjected([this, positionXSourceMode]
			{
				Get().PositionXSourceMode(positionXSourceMode);
			});
		}

		[[nodiscard]] auto GetPositionYSourceMode() const noexcept -> Result<InteractionSourceRedirectionMode>
		{
			return CallProjected([this]
			{
				return Get().PositionYSourceMode();
			});
		}
		[[nodiscard]] auto SetPositionYSourceMode(const InteractionSourceRedirectionMode positionYSourceMode) const noexcept -> Result<void>
		{
			return CallProjected([this, positionYSourceMode]
			{
				Get().PositionYSourceMode(positionYSourceMode);
			});
		}

		[[nodiscard]] auto GetScaleSourceMode() const noexcept -> Result<InteractionSourceRedirectionMode>
		{
			return CallProjected([this]
			{
				return Get().ScaleSourceMode();
			});
		}
		[[nodiscard]] auto SetScaleSourceMode(const InteractionSourceRedirectionMode scaleSourceMode) const noexcept -> Result<void>
		{
			return CallProjected([this, scaleSourceMode]
			{
				Get().ScaleSourceMode(scaleSourceMode);
			});
		}
	};

	class VisualInteractionSource : public CompositionObjectT<WUCI::VisualInteractionSource>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] static auto Create(const Visual& source) noexcept -> Result<VisualInteractionSource>
		{
			return CallProjected([&source]
			{
				return VisualInteractionSource{ WUCI::VisualInteractionSource::Create(source.Get()) };
			});
		}

		[[nodiscard]] auto GetDeltaPosition() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().DeltaPosition() };
			});
		}

		[[nodiscard]] auto GetDeltaScale() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().DeltaScale();
			});
		}

		[[nodiscard]] auto EnablePositionXRails(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				Get().IsPositionXRailsEnabled(enabled);
			});
		}
		[[nodiscard]] auto DisablePositionXRails() const noexcept -> Result<void> { return EnablePositionXRails(false); }
		[[nodiscard]] auto IsPositionXRailsEnabled() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return Get().IsPositionXRailsEnabled();
			});
		}

		[[nodiscard]] auto EnablePositionYRails(const bool enabled = true) const noexcept -> Result<void>
		{
			return CallProjected([this, enabled]
			{
				Get().IsPositionYRailsEnabled(enabled);
			});
		}
		[[nodiscard]] auto DisablePositionYRails() const noexcept -> Result<void> { return EnablePositionYRails(false); }
		[[nodiscard]] auto IsPositionYRailsEnabled() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return Get().IsPositionYRailsEnabled();
			});
		}

		[[nodiscard]] auto GetManipulationRedirectionMode() const noexcept -> Result<ManipulationRedirectionMode>
		{
			return CallProjected([this]
			{
				return Get().ManipulationRedirectionMode();
			});
		}
		[[nodiscard]] auto SetManipulationRedirectionMode(const ManipulationRedirectionMode manipulationRedirectionMode) const noexcept -> Result<void>
		{
			return CallProjected([this, manipulationRedirectionMode]
			{
				Get().ManipulationRedirectionMode(manipulationRedirectionMode);
			});
		}

		[[nodiscard]] auto GetPointerWheelConfig() const noexcept -> Result<InteractionSourceConfiguration>
		{
			return CallProjected([this]
			{
				return InteractionSourceConfiguration{ Get().PointerWheelConfig() };
			});
		}

		[[nodiscard]] auto GetPosition() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().Position() };
			});
		}

		[[nodiscard]] auto GetPositionVelocity() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().PositionVelocity() };
			});
		}

		[[nodiscard]] auto GetPositionXChainingMode() const noexcept -> Result<ChainingMode>
		{
			return CallProjected([this]
			{
				return Get().PositionXChainingMode();
			});
		}
		[[nodiscard]] auto SetPositionXChainingMode(const ChainingMode positionXChainingMode) const noexcept -> Result<void>
		{
			return CallProjected([this, positionXChainingMode]
			{
				Get().PositionXChainingMode(positionXChainingMode);
			});
		}

		[[nodiscard]] auto GetPositionXSourceMode() const noexcept -> Result<InteractionSourceMode>
		{
			return CallProjected([this]
			{
				return Get().PositionXSourceMode();
			});
		}
		[[nodiscard]] auto SetPositionXSourceMode(const InteractionSourceMode positionXSourceMode) const noexcept -> Result<void>
		{
			return CallProjected([this, positionXSourceMode]
			{
				Get().PositionXSourceMode(positionXSourceMode);
			});
		}

		[[nodiscard]] auto GetPositionYChainingMode() const noexcept -> Result<ChainingMode>
		{
			return CallProjected([this]
			{
				return Get().PositionYChainingMode();
			});
		}
		[[nodiscard]] auto SetPositionYChainingMode(const ChainingMode positionYChainingMode) const noexcept -> Result<void>
		{
			return CallProjected([this, positionYChainingMode]
			{
				Get().PositionYChainingMode(positionYChainingMode);
			});
		}

		[[nodiscard]] auto GetPositionYSourceMode() const noexcept -> Result<InteractionSourceMode>
		{
			return CallProjected([this]
			{
				return Get().PositionYSourceMode();
			});
		}
		[[nodiscard]] auto SetPositionYSourceMode(const InteractionSourceMode positionYSourceMode) const noexcept -> Result<void>
		{
			return CallProjected([this, positionYSourceMode]
			{
				Get().PositionYSourceMode(positionYSourceMode);
			});
		}

		[[nodiscard]] auto GetScale() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Scale();
			});
		}

		[[nodiscard]] auto GetScaleChainingMode() const noexcept -> Result<ChainingMode>
		{
			return CallProjected([this]
			{
				return Get().ScaleChainingMode();
			});
		}
		[[nodiscard]] auto SetScaleChainingMode(const ChainingMode scaleChainingMode) const noexcept -> Result<void>
		{
			return CallProjected([this, scaleChainingMode]
			{
				Get().ScaleChainingMode(scaleChainingMode);
			});
		}

		[[nodiscard]] auto GetScaleSourceMode() const noexcept -> Result<InteractionSourceMode>
		{
			return CallProjected([this]
			{
				return Get().ScaleSourceMode();
			});
		}
		[[nodiscard]] auto SetScaleSourceMode(const InteractionSourceMode scaleSourceMode) const noexcept -> Result<void>
		{
			return CallProjected([this, scaleSourceMode]
			{
				Get().ScaleSourceMode(scaleSourceMode);
			});
		}

		[[nodiscard]] auto GetScaleVelocity() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().ScaleVelocity();
			});
		}

		[[nodiscard]] auto GetSource() const noexcept -> Result<Visual>
		{
			return CallProjected([this]
			{
				return Visual{ Get().Source() };
			});
		}

		[[nodiscard]] auto ConfigureCenterPointXModifiers(const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, conditionalValues]
			{
				Get().ConfigureCenterPointXModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}

		[[nodiscard]] auto ConfigureCenterPointYModifiers(const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, conditionalValues]
			{
				Get().ConfigureCenterPointYModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}

		[[nodiscard]] auto ConfigureDeltaPositionXModifiers(const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, conditionalValues]
			{
				Get().ConfigureDeltaPositionXModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}

		[[nodiscard]] auto ConfigureDeltaPositionYModifiers(const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, conditionalValues]
			{
				Get().ConfigureDeltaPositionYModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}

		[[nodiscard]] auto ConfigureDeltaScaleModifiers(const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, conditionalValues]
			{
				Get().ConfigureDeltaScaleModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}

		[[nodiscard]] auto TryRedirectForManipulation(const POINTER_INFO& pointerInfo) const noexcept -> Result<void>
		{
			const auto interop = Get().try_as<AWUCI::IVisualInteractionSourceInterop>();
			if (!interop)
			{
				return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
			}

			if (const auto error = Error{
					interop->TryRedirectForManipulation(pointerInfo)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
		[[nodiscard]] auto TryRedirectForManipulation(const std::uint32_t pointerId) const noexcept -> Result<void>
		{
			POINTER_INFO pointerInfo;
			if (const auto result = GetPointerInfo(pointerId, &pointerInfo);
				result == 0)
			{
				return Unexpected{ Error{
					GetLastError()
				}.AddDetail(L"pointerId", std::to_wstring(pointerId)) };
			}

			return TryRedirectForManipulation(pointerInfo);
		}
	};
}

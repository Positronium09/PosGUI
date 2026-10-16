module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.UI.Composition.Interactions.h>

#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Foundation.h>

export module PGUI.UI.VL.Interaction.InteractionTracker;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionAnimation;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.Interaction.ConditionalValue;
import PGUI.UI.VL.Interaction.InteractionTrackerOwner;
import PGUI.UI.VL.Interaction.VisualInteractionSource;
import PGUI.UI.VL.Interaction.InteractionTrackerInertiaModifier;
import PGUI.UI.VL.Interaction.InteractionTrackerVector2InertiaModifier;

namespace WUC = winrt::Windows::UI::Composition;
namespace WUCI = winrt::Windows::UI::Composition::Interactions;
namespace WFN = winrt::Windows::Foundation::Numerics;

export namespace PGUI::UI::VL
{
	class InteractionSourceCollection : public CompositionObjectT<WUCI::CompositionInteractionSourceCollection>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] auto GetCount() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().Count();
			});
		}

		[[nodiscard]] auto Add(const VisualInteractionSource& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				Get().Add(value.Get());
			});
		}

		[[nodiscard]] auto Remove(const VisualInteractionSource& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				Get().Remove(value.Get());
			});
		}

		[[nodiscard]] auto RemoveAll() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				Get().RemoveAll();
			});
		}

		[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<VisualInteractionSource>>
		{
			return CallProjected([this]
			{
				auto elements = std::vector<VisualInteractionSource>{ };
				elements.reserve(static_cast<std::size_t>(Get().Count()));

				for (const auto& element : Get())
				{
					elements.emplace_back(element.as<WUCI::VisualInteractionSource>());
				}

				return elements;
			});
		}

		template <Callable<Result<void>(VisualInteractionSource)> F>
		[[nodiscard]] auto ForEach(F&& f) const noexcept -> Result<void>
		{
			return CallProjected([this, &f] -> Result<void>
			{
				for (const auto& [i, element] : Get() | std::views::enumerate)
				{
					if (auto result = std::invoke(f, VisualInteractionSource{ element.as<WUCI::VisualInteractionSource>() });
						!result.has_value())
					{
						return Unexpected{ result.error().AddDetail(L"iteration", std::to_wstring(i)) };
					}
				}

				return EmptyResult;
			}).and_then(std::identity{ });
		}
	};

	class InteractionTracker : public CompositionObjectT<WUCI::InteractionTracker>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] static auto Create() noexcept -> Result<InteractionTracker>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return InteractionTracker{ WUCI::InteractionTracker::Create(compositor) };
			});
		}

		[[nodiscard]] static auto Create(const InteractionTrackerOwner& owner) noexcept -> Result<InteractionTracker>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();
			if (owner.GetRouter() == nullptr)
			{
				return Unexpected{ Error{ ErrorCode::NullPointer }.AddDetail(L"owner", L"nullptr") };
			}

			return CallProjected([&compositor, &owner]
			{
				InteractionTracker tracker{ WUCI::InteractionTracker::CreateWithOwner(compositor, *owner.GetRouter().get()) };
				return tracker;
			});
		}
		[[nodiscard]] static auto Create(InteractionTrackerOwner&&) noexcept -> Result<InteractionTracker> = delete;

		[[nodiscard]] auto GetInteractionSources() const noexcept -> Result<InteractionSourceCollection>
		{
			return CallProjected([this]
			{
				return InteractionSourceCollection{ Get().InteractionSources() };
			});
		}

		[[nodiscard]] auto IsInertiaFromImpulse() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return Get().IsInertiaFromImpulse();
			});
		}

		[[nodiscard]] auto IsPositionRoundingSuggested() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return Get().IsPositionRoundingSuggested();
			});
		}

		[[nodiscard]] auto GetMaxPosition() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().MaxPosition() };
			});
		}
		[[nodiscard]] auto SetMaxPosition(const Point3F& maxPosition) const noexcept -> Result<void>
		{
			return CallProjected([this, &maxPosition]
			{
				Get().MaxPosition(maxPosition);
			});
		}

		[[nodiscard]] auto GetMaxScale() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().MaxScale();
			});
		}
		[[nodiscard]] auto SetMaxScale(const float maxScale) const noexcept -> Result<void>
		{
			return CallProjected([this, maxScale]
			{
				Get().MaxScale(maxScale);
			});
		}

		[[nodiscard]] auto GetMinPosition() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().MinPosition() };
			});
		}
		[[nodiscard]] auto SetMinPosition(const Point3F& minPosition) const noexcept -> Result<void>
		{
			return CallProjected([this, &minPosition]
			{
				Get().MinPosition(minPosition);
			});
		}

		[[nodiscard]] auto GetMinScale() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().MinScale();
			});
		}
		[[nodiscard]] auto SetMinScale(const float minScale) const noexcept -> Result<void>
		{
			return CallProjected([this, minScale]
			{
				Get().MinScale(minScale);
			});
		}

		[[nodiscard]] auto GetNaturalRestingPosition() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().NaturalRestingPosition() };
			});
		}

		[[nodiscard]] auto GetNaturalRestingScale() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().NaturalRestingScale();
			});
		}

		[[nodiscard]] auto GetPosition() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().Position() };
			});
		}

		[[nodiscard]] auto GetPositionInertiaDecayRate() const noexcept -> Result<std::optional<Point3F>>
		{
			return CallProjected([this]
			{
				return std::optional<WFN::float3>{ Get().PositionInertiaDecayRate() }.transform([](const auto& value)
				{
					return Point3F{ value };
				});
			});
		}
		[[nodiscard]] auto SetPositionInertiaDecayRate(const std::optional<Point3F>& positionInertiaDecayRate) const noexcept -> Result<void>
		{
			return CallProjected([this, &positionInertiaDecayRate]
			{
				Get().PositionInertiaDecayRate(std::optional<WFN::float3>{ positionInertiaDecayRate });
			});
		}

		[[nodiscard]] auto GetPositionVelocityInPixelsPerSecond() const noexcept -> Result<Point3F>
		{
			return CallProjected([this]
			{
				return Point3F{ Get().PositionVelocityInPixelsPerSecond() };
			});
		}

		[[nodiscard]] auto GetScale() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Scale();
			});
		}

		[[nodiscard]] auto GetScaleInertiaDecayRate() const noexcept -> Result<std::optional<float>>
		{
			return CallProjected([this]
			{
				return std::optional<float>{ Get().ScaleInertiaDecayRate() };
			});
		}
		[[nodiscard]] auto SetScaleInertiaDecayRate(const std::optional<float> scaleInertiaDecayRate) const noexcept -> Result<void>
		{
			return CallProjected([this, scaleInertiaDecayRate]
			{
				Get().ScaleInertiaDecayRate(scaleInertiaDecayRate);
			});
		}

		[[nodiscard]] auto GetScaleVelocityInPercentPerSecond() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().ScaleVelocityInPercentPerSecond();
			});
		}

		[[nodiscard]] auto AdjustPositionXIfGreaterThanThreshold(const float adjustment, const float positionThreshold) const noexcept -> Result<void>
		{
			return CallProjected([this, adjustment, positionThreshold]
			{
				Get().AdjustPositionXIfGreaterThanThreshold(adjustment, positionThreshold);
			});
		}

		[[nodiscard]] auto AdjustPositionYIfGreaterThanThreshold(const float adjustment, const float positionThreshold) const noexcept -> Result<void>
		{
			return CallProjected([this, adjustment, positionThreshold]
			{
				Get().AdjustPositionYIfGreaterThanThreshold(adjustment, positionThreshold);
			});
		}

		[[nodiscard]] auto ConfigureCenterPointXInertiaModifiers(
			const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, &conditionalValues]
			{
				Get().ConfigureCenterPointXInertiaModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}
		[[nodiscard]] auto ConfigureCenterPointYInertiaModifiers(
			const std::span<const ScalarConditionalValue> conditionalValues) const noexcept -> Result<void>
		{
			return CallProjected([this, &conditionalValues]
			{
				Get().ConfigureCenterPointYInertiaModifiers(
					ProjectRangeUnchecked<WUCI::CompositionConditionalValue>(conditionalValues));
			});
		}

		[[nodiscard]] auto ConfigurePositionXInertiaModifiers(
			const std::span<const InteractionTrackerInertiaModifier> modifiers) const noexcept -> Result<void>
		{
			return CallProjected([this, &modifiers]
			{
				Get().ConfigurePositionXInertiaModifiers(
					ProjectRangeUnchecked<WUCI::InteractionTrackerInertiaModifier>(modifiers));
			});
		}
		[[nodiscard]] auto ConfigurePositionYInertiaModifiers(
			const std::span<const InteractionTrackerInertiaModifier> modifiers) const noexcept -> Result<void>
		{
			return CallProjected([this, &modifiers]
			{
				Get().ConfigurePositionYInertiaModifiers(
					ProjectRangeUnchecked<WUCI::InteractionTrackerInertiaModifier>(modifiers));
			});
		}
		[[nodiscard]] auto ConfigureScaleInertiaModifiers(
			const std::span<const InteractionTrackerInertiaModifier> modifiers) const noexcept -> Result<void>
		{
			return CallProjected([this, &modifiers]
			{
				Get().ConfigureScaleInertiaModifiers(
					ProjectRangeUnchecked<WUCI::InteractionTrackerInertiaModifier>(modifiers));
			});
		}

		[[nodiscard]] auto ConfigureVector2PositionInertiaModifiers(
			const std::span<const InteractionTrackerVector2InertiaModifier> modifiers) const noexcept -> Result<void>
		{
			return CallProjected([this, &modifiers]
			{
				Get().ConfigureVector2PositionInertiaModifiers(
					ProjectRangeUnchecked<WUCI::InteractionTrackerVector2InertiaModifier>(modifiers));
			});
		}

		[[nodiscard]] auto TryUpdatePosition(const Point3F& value) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &value]
			{
				return Get().TryUpdatePosition(value);
			});
		}

		[[nodiscard]] auto TryUpdatePosition(const Point3F& value, const ClampingOption option) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &value, option]
			{
				return Get().TryUpdatePosition(value, option);
			});
		}

		[[nodiscard]] auto TryUpdatePosition(
			const Point3F& value, 
			const ClampingOption option, const PositionUpdateOption posUpdateOption) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &value, option, posUpdateOption]
			{
				return Get().TryUpdatePosition(value, option, posUpdateOption);
			});
		}

		[[nodiscard]] auto TryUpdatePositionBy(const Point3F& amount) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &amount]
			{
				return Get().TryUpdatePositionBy(amount);
			});
		}

		[[nodiscard]] auto TryUpdatePositionBy(const Point3F& amount, const ClampingOption option) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &amount, option]
			{
				return Get().TryUpdatePositionBy(amount, option);
			});
		}

		[[nodiscard]] auto TryUpdatePositionWithAdditionalVelocity(const Point3F& velocityInPixelsPerSecond) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &velocityInPixelsPerSecond]
			{
				return Get().TryUpdatePositionWithAdditionalVelocity(velocityInPixelsPerSecond);
			});
		}

		[[nodiscard]] auto TryUpdatePositionWithAnimation(const CompositionAnimation& animation) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &animation]
			{
				return Get().TryUpdatePositionWithAnimation(animation.Get());
			});
		}

		[[nodiscard]] auto TryUpdateScale(const float value, const Point3F& centerPoint) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, value, &centerPoint]
			{
				return Get().TryUpdateScale(value, centerPoint);
			});
		}

		[[nodiscard]] auto TryUpdateScaleWithAdditionalVelocity(
			const float velocityInPercentPerSecond, 
			const Point3F& centerPoint) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, velocityInPercentPerSecond, &centerPoint]
			{
				return Get().TryUpdateScaleWithAdditionalVelocity(velocityInPercentPerSecond, centerPoint);
			});
		}

		[[nodiscard]] auto TryUpdateScaleWithAnimation(
			const CompositionAnimation& animation, 
			const Point3F& centerPoint) const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this, &animation, &centerPoint]
			{
				return Get().TryUpdateScaleWithAnimation(animation.Get(), centerPoint);
			});
		}
	};
}
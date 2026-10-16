module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.AnimationGroup;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.UI.VL.Properties;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionAnimation;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <WrapperDerivedFrom<WUC::CompositionObject> Owner>
	class AnimationGroup : public CompositionObjectT<WUC::CompositionAnimationGroup>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		using OwnerType = Owner;

		[[nodiscard]] static auto Create() noexcept -> Result<AnimationGroup>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return AnimationGroup{ compositor.CreateAnimationGroup() };
			});
		}

		[[nodiscard]] auto GetCount() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return this->Get().Count();
			});
		}

		template <
			Properties::IsProperty Target,
			WrapperDerivedFrom<WUC::CompositionAnimation> A> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Target::OwnerHandleType> &&
			requires { typename A::AnimationTag; } &&
			std::same_as<typename A::AnimationTag, typename Target::ValueTag>
		[[nodiscard]] auto Add(const A& animation) const noexcept -> Result<void>
		{
			if (const auto result = animation.template SetTarget<Target>();
				!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			return CallProjected([this, &animation]
			{
				return this->Get().Add(animation.Get());
			});
		}

		//TODO First() -> IIterator<CompositionAnimation>

		[[nodiscard]] auto Remove(const CompositionAnimation& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				this->Get().Remove(value.Get());
			});
		}

		[[nodiscard]] auto RemoveAll() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().RemoveAll();
			});
		}

		[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<CompositionAnimation>>
		{
			return CallProjected([this]
			{
				auto elements = std::vector<CompositionAnimation>{ };
				elements.reserve(static_cast<std::size_t>(this->Get().Count()));

				for (const auto& element : this->Get())
				{
					elements.emplace_back(element);
				}

				return elements;
			});
		}

		template <Callable<Result<void>(CompositionAnimation)> F>
		[[nodiscard]] auto ForEach(F&& f) const noexcept -> Result<void>
		{
			return CallProjected([this, &f] -> Result<void>
			{
				for (const auto& [i, element] : this->Get() | std::views::enumerate)
				{
					if (auto result = std::invoke(f, CompositionAnimation{ element });
						!result.has_value())
					{
						return Unexpected{ result.error().AddDetail(L"iteration", std::to_wstring(i)) };
					}
				}

				return EmptyResult;
			}).and_then(std::identity{ });
		}
	};

	template <WrapperDerivedFrom<WUC::CompositionObject> Owner>
	[[nodiscard]] auto CreateAnimationGroup() noexcept -> Result<AnimationGroup<Owner>>
	{
		return AnimationGroup<Owner>::Create();
	}

	template <WrapperDerivedFrom<WUC::CompositionObject> O>
	[[nodiscard]] auto CreateAnimationGroup(const O& object) noexcept -> Result<AnimationGroup<O>>
	{
		return CallProjected([&object]
		{
			return AnimationGroup<O>{ object.Get().Compositor().CreateAnimationGroup() };
		});
	}
}

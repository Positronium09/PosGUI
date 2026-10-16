module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.ImplicitAnimationCollection;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.UI.VL.Properties;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.CompositionAnimation;
import PGUI.UI.VL.AnimationGroup;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <WrapperDerivedFrom<WUC::CompositionObject> Owner>
	class ImplicitAnimationCollection : public CompositionObjectT<WUC::ImplicitAnimationCollection>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		using OwnerType = Owner;

		[[nodiscard]] static auto Create() noexcept -> Result<ImplicitAnimationCollection>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor]
			{
				return ImplicitAnimationCollection{ compositor.CreateImplicitAnimationCollection() };
			});
		}

		[[nodiscard]] auto GetSize() const noexcept -> Result<std::uint32_t>
		{
			return CallProjected([this]
			{
				return this->Get().Size();
			});
		}

		[[nodiscard]] auto Clear() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().Clear();
			});
		}

		//TODO First() -> IIterator<IKeyValuePair<string, ICompositionAnimationBase>>

		[[nodiscard]] auto GetView() const noexcept -> Result<std::map<std::wstring, Wrapper<WUC::ICompositionAnimationBase>>>
		{
			return CallProjected([this]
			{
				return this->Get().GetView() | std::views::transform([](const auto& element)
				{
					return std::pair{ std::wstring{ element.Key() }, Wrapper<WUC::ICompositionAnimationBase>{ element.Value() } };
				}) | std::ranges::to<std::map>();
			});
		}

		template <Properties::IsProperty Trigger> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType>
		[[nodiscard]] auto HasKey() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return this->Get().HasKey(Trigger::name);
			});
		}

		template <
			Properties::IsProperty Trigger, 
			Properties::IsProperty Target = Trigger,
			WrapperDerivedFrom<WUC::CompositionAnimation> A> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType> &&
			HandleDerivedFrom<typename Owner::HandleType, typename Target::OwnerHandleType> &&
			requires { typename A::AnimationTag; } &&
			std::same_as<typename A::AnimationTag, typename Target::ValueTag>
		[[nodiscard]] auto Insert(const A& animation) const noexcept -> Result<bool>
		{
			if (const auto result = animation.template SetTarget<Target>();
				!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			return CallProjected([this, &animation]
			{
				return this->Get().Insert(Trigger::name, animation.Get());
			});
		}
		template <Properties::IsProperty Trigger, typename GroupOwner> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType> &&
			HandleDerivedFrom<typename Owner::HandleType, typename GroupOwner::HandleType>
		[[nodiscard]] auto Insert(const AnimationGroup<GroupOwner>& group) const noexcept -> Result<bool>
		{
			return CallProjected([this, &group]
			{
				return this->Get().Insert(Trigger::name, group.Get());
			});
		}

		template <Properties::IsProperty Trigger> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType>
		[[nodiscard]] auto Lookup() const noexcept -> Result<Wrapper<WUC::ICompositionAnimationBase>>
		{
			return CallProjected([this]
			{
				return Wrapper<WUC::ICompositionAnimationBase>{ this->Get().Lookup(Trigger::name) };
			});
		}
		template <Properties::IsProperty Trigger> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType>
		[[nodiscard]] auto TryLookup() const noexcept -> Result<std::optional<Wrapper<WUC::ICompositionAnimationBase>>>
		{
			return CallProjected([this] -> std::optional<Wrapper<WUC::ICompositionAnimationBase>>
			{
				const auto value = this->Get().TryLookup(Trigger::name);
				if (!value)
				{
					return std::nullopt;
				}

				return Wrapper<WUC::ICompositionAnimationBase>{ value };
			});
		}

		template <Properties::IsProperty Trigger> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType>
		[[nodiscard]] auto Remove() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().Remove(Trigger::name);
			});
		}
		template <Properties::IsProperty Trigger> requires
			HandleDerivedFrom<typename Owner::HandleType, typename Trigger::OwnerHandleType>
		[[nodiscard]] auto TryRemove() const noexcept -> Result<bool>
		{
			return CallProjected([this]
			{
				return this->Get().TryRemove(Trigger::name);
			});
		}

		[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<std::pair<std::wstring, Wrapper<WUC::ICompositionAnimationBase>>>>
		{
			return CallProjected([this]
			{
				auto elements = std::vector<std::pair<std::wstring, Wrapper<WUC::ICompositionAnimationBase>>>{ };
				elements.reserve(this->Get().Size());

				for (const auto& element : this->Get())
				{
					elements.emplace_back(std::wstring{ element.Key() }, Wrapper<WUC::ICompositionAnimationBase>{ element.Value() });
				}

				return elements;
			});
		}

		template <Callable<Result<void>(std::wstring, Wrapper<WUC::ICompositionAnimationBase>)> F>
		[[nodiscard]] auto ForEach(F&& f) const noexcept -> Result<void>
		{
			return CallProjected([this, &f] -> Result<void>
			{
				for (const auto& [i, element] : this->Get() | std::views::enumerate)
				{
					if (auto result = std::invoke(f, std::wstring{ element.Key() }, Wrapper<WUC::ICompositionAnimationBase>{ element.Value() });
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
	[[nodiscard]] auto CreateImplicitAnimationCollection() noexcept -> Result<ImplicitAnimationCollection<Owner>>
	{
		return ImplicitAnimationCollection<Owner>::Create();
	}

	template <WrapperDerivedFrom<WUC::CompositionObject> O>
	[[nodiscard]] auto CreateImplicitAnimationCollection(const O& object) noexcept -> Result<ImplicitAnimationCollection<O>>
	{
		return CallProjected([&object]
		{
			return ImplicitAnimationCollection<O>{ object.Get().Compositor().CreateImplicitAnimationCollection() };
		});
	}
}

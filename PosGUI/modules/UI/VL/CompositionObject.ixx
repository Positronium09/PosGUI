module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionObject;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.UI.VL.Properties;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionObject> Projected = WUC::CompositionObject>
	class CompositionObjectT : public Wrapper<Projected>
	{
		public:
		using Base = Wrapper<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetComment() const noexcept -> Result<std::wstring>
		{
			return CallProjected([this]
			{
				return std::wstring{ this->Get().Comment() };
			});
		}
		[[nodiscard]] auto SetComment(const std::wstring_view comment) const noexcept -> Result<void>
		{
			return CallProjected([this, &comment]
			{
				this->Get().Comment(comment);
			});
		}

		//TODO Compositor Compositor

		//TODO Dispatcher CoreDispatcher

		//TODO DispatcherQueue DispatcherQueue

		template <WrapperDerivedFrom<WUC::ImplicitAnimationCollection> C> requires
			requires { typename C::OwnerType; } &&
			HandleDerivedFrom<Projected, typename C::OwnerType::HandleType>
			[[nodiscard]] auto SetImplicitAnimations(const C& collection) const noexcept -> Result<void>
		{
			return CallProjected([this, &collection]
			{
				this->Get().ImplicitAnimations(collection.Get());
			});
		}

		[[nodiscard]] auto GetProperties() const noexcept -> Result<Wrapper<WUC::CompositionPropertySet>>
		{
			return CallProjected([this]
			{
				return Wrapper<WUC::CompositionPropertySet>{ this->Get().Properties() };
			});
		}

		[[nodiscard]] auto Close() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().Close();
			});
		}

		[[nodiscard]] auto PopulatePropertyInfo(
			const std::wstring_view propertyName, 
			const WrapperDerivedFrom<WUC::AnimationPropertyInfo> auto& propertyInfo) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName, &propertyInfo]
			{
				this->Get().PopulatePropertyInfo(propertyName, propertyInfo.Get());
			});
		}

		[[nodiscard]] auto StartAnimation(
			const std::wstring_view propertyName, 
			const WrapperDerivedFrom<WUC::CompositionAnimation> auto& animation) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName, &animation]
			{
				this->Get().StartAnimation(propertyName, animation.Get());
			});
		}

		[[nodiscard]] auto StartAnimation(
			const std::wstring_view propertyName, 
			const WrapperDerivedFrom<WUC::CompositionAnimation> auto& animation, 
			const WrapperDerivedFrom<WUC::AnimationController> auto& animationController) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName, &animation, &animationController]
			{
				this->Get().StartAnimation(propertyName, animation.Get(), animationController.Get());
			});
		}

		template <Properties::IsProperty P, WrapperDerivedFrom<WUC::CompositionAnimation> A>
			requires HandleDerivedFrom<Projected, typename P::OwnerHandleType> &&
			std::same_as<typename A::AnimationTag, typename P::ValueTag>
			[[nodiscard]] auto StartAnimation(const A& animation) const noexcept -> Result<void>
		{
			return StartAnimation(P::name, animation);
		}

		template <WrapperDerivedFrom<WUC::CompositionAnimationGroup> G> requires
			requires { typename G::OwnerType; } &&
			HandleDerivedFrom<Projected, typename G::OwnerType::HandleType>
		[[nodiscard]] auto StartAnimationGroup(const G& group) const noexcept -> Result<void>
		{
			return CallProjected([this, &group]
			{
				this->Get().StartAnimationGroup(group.Get());
			});
		}
		template <WrapperDerivedFrom<WUC::ICompositionAnimationBase> V> requires
			(!requires { typename V::OwnerType; })
		[[nodiscard]] auto StartAnimationGroup(const V& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				this->Get().StartAnimationGroup(value.Get());
			});
		}

		template <Properties::IsProperty P> requires
			HandleDerivedFrom<Projected, typename P::OwnerHandleType>
		[[nodiscard]] auto StopAnimation() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().StopAnimation(P::name);
			});
		}
		[[nodiscard]] auto StopAnimation(const std::wstring_view propertyName) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName]
			{
				this->Get().StopAnimation(propertyName);
			});
		}

		template <WrapperDerivedFrom<WUC::CompositionAnimationGroup> G> requires
			requires { typename G::OwnerType; } &&
			HandleDerivedFrom<Projected, typename G::OwnerType::HandleType>
		[[nodiscard]] auto StopAnimationGroup(const G& group) const noexcept -> Result<void>
		{
			return CallProjected([this, &group]
			{
				this->Get().StopAnimationGroup(group.Get());
			});
		}
		template <WrapperDerivedFrom<WUC::ICompositionAnimationBase> V> requires
			(!requires { typename V::OwnerType; })
		[[nodiscard]] auto StopAnimationGroup(const V& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				this->Get().StopAnimationGroup(value.Get());
			});
		}

		//TODO TryGetAnimationController(string propertyName) -> AnimationController
	};
	using CompositionObject = CompositionObjectT<>;
}

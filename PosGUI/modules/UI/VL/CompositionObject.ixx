module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionObject;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;

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
		[[nodiscard]] auto SetComment(const std::wstring& comment) const noexcept -> Result<void>
		{
			return CallProjected([this, &comment]
			{
				this->Get().Comment(comment);
			});
		}

		//TODO Compositor Compositor

		//TODO Dispatcher CoreDispatcher

		//TODO DispatcherQueue DispatcherQueue

		//TODO ImplicitAnimations ImplicitAnimationCollection

		//TODO Properties CompositionPropertySet

		[[nodiscard]] auto Close() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().Close();
			});
		}

		[[nodiscard]] auto PopulatePropertyInfo(const std::wstring& propertyName, const WrapperDerivedFrom<WUC::AnimationPropertyInfo> auto& propertyInfo) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName, &propertyInfo]
			{
				this->Get().PopulatePropertyInfo(propertyName, propertyInfo.Get());
			});
		}

		[[nodiscard]] auto StartAnimation(const std::wstring& propertyName, const WrapperDerivedFrom<WUC::CompositionAnimation> auto& animation) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName, &animation]
			{
				this->Get().StartAnimation(propertyName, animation.Get());
			});
		}

		[[nodiscard]] auto StartAnimation(const std::wstring& propertyName, const WrapperDerivedFrom<WUC::CompositionAnimation> auto& animation, const WrapperDerivedFrom<WUC::AnimationController> auto& animationController) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName, &animation, &animationController]
			{
				this->Get().StartAnimation(propertyName, animation.Get(), animationController.Get());
			});
		}

		[[nodiscard]] auto StartAnimationGroup(const WrapperDerivedFrom<WUC::ICompositionAnimationBase> auto& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &value]
			{
				this->Get().StartAnimationGroup(value.Get());
			});
		}

		[[nodiscard]] auto StopAnimation(const std::wstring& propertyName) const noexcept -> Result<void>
		{
			return CallProjected([this, &propertyName]
			{
				this->Get().StopAnimation(propertyName);
			});
		}

		[[nodiscard]] auto StopAnimationGroup(const WrapperDerivedFrom<WUC::ICompositionAnimationBase> auto& value) const noexcept -> Result<void>
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

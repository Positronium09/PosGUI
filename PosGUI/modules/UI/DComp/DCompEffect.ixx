module;
#include <dcomp.h>

export module PGUI.UI.DComp.Effect;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.DComp.Animation;

export namespace PGUI::UI::DComp
{
	template <typename Interface = IDCompositionEffect>
	class Effect : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionEffect>);

		public:
		Effect() noexcept = default;
		explicit(false) Effect(const ComPtr<Interface>& ptr) noexcept :
			ComPtrHolder<Interface>{ ptr }
		{ }

		explicit(false) operator Effect<>() const noexcept
		{
			return Effect<>{ this->Get() };
		}
	};

	template <typename Interface = IDCompositionTransform3D>
	class Transform3D : public Effect<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionTransform3D>);
		public:
		Transform3D() noexcept = default;
		explicit(false) Transform3D(const ComPtr<Interface>& ptr) noexcept :
			Effect<Interface>{ ptr }
		{ }
		
		explicit(false) operator Transform3D<>() const noexcept
		{
			return Transform3D<>{ this->Get() };
		}
	};

	template <typename Interface = IDCompositionEffectGroup>
	class EffectGroup : public Effect<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionEffectGroup>);

		public:
		EffectGroup() noexcept = default;
		explicit(false) EffectGroup(const ComPtr<Interface>& ptr) noexcept :
			Effect<Interface>{ ptr }
		{ }

		[[nodiscard]] auto SetOpacity(const float opacity) const noexcept -> Result<void>
		{
			if (const auto error = Error{ this->Get()->SetOpacity(opacity) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetOpacity(const Animation& animation) const noexcept -> Result<void>
		{
			if (const auto error = Error{ this->Get()->SetOpacity(animation.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto SetTransform3D(const Transform3D<>& transform) const noexcept -> Result<void>
		{
			if (const auto error = Error{ this->Get()->SetTransform3D(transform.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		explicit(false) operator EffectGroup<>() const noexcept
		{
			return EffectGroup<>{ this->Get() };
		}
	};

	template <typename Interface = IDCompositionFilterEffect>
	class FilterEffect : public Effect<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionFilterEffect>);

		public:
		FilterEffect() noexcept = default;
		explicit(false) FilterEffect(const ComPtr<Interface>& ptr) noexcept :
			Effect<Interface>{ ptr }
		{ }

		[[nodiscard]] auto SetInput(const UINT index, const ComPtr<IUnknown>& input, const UINT flags = 0) const noexcept -> Result<void>
		{
			if (const auto error = Error{ this->Get()->SetInput(index, input.get(), flags) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		explicit(false) operator FilterEffect<>() const noexcept
		{
			return FilterEffect<>{ this->Get() };
		}
	};
}

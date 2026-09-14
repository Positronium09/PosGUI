module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Geometry.CompositionGeometry;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionGeometry> Projected = WUC::CompositionGeometry>
	class CompositionGeometryT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetTrimEnd() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().TrimEnd();
			});
		}
		[[nodiscard]] auto SetTrimEnd(const float trimEnd) const noexcept -> Result<void>
		{
			return CallProjected([this, trimEnd]
			{
				this->Get().TrimEnd(trimEnd);
			});
		}

		[[nodiscard]] auto GetTrimOffset() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().TrimOffset();
			});
		}
		[[nodiscard]] auto SetTrimOffset(const float trimOffset) const noexcept -> Result<void>
		{
			return CallProjected([this, trimOffset]
			{
				this->Get().TrimOffset(trimOffset);
			});
		}

		[[nodiscard]] auto GetTrimStart() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return this->Get().TrimStart();
			});
		}
		[[nodiscard]] auto SetTrimStart(const float trimStart) const noexcept -> Result<void>
		{
			return CallProjected([this, trimStart]
			{
				this->Get().TrimStart(trimStart);
			});
		}
	};
	using CompositionGeometry = CompositionGeometryT<>;
}

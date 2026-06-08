module;
#include <dcomp.h>

export module PGUI.UI.DComp.Target;

import PGUI.ComPtr;
import PGUI.UI.DComp.Visual;
import PGUI.ErrorHandling;

export namespace PGUI::UI::DComp
{
	class Target : public ComPtrHolder<IDCompositionTarget>
	{
		public:
		Target() noexcept = default;
		explicit(false) Target(const ComPtr<IDCompositionTarget>& ptr) noexcept :
			ComPtrHolder{ ptr }
		{ }

		[[nodiscard]] auto SetRoot(const Visual& visual) const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRoot(visual.GetRaw()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto ClearRoot() const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get()->SetRoot(nullptr) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
	};
}

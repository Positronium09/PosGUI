module;
#include <wrl.h>
#include <d2d1_1.h>

export module PGUI.UI.D2D.Effect;

import PGUI.ComPtr;
import PGUI.UI.D2D.D2DImage;
import PGUI.UI.D2D.D2DProperties;

export namespace PGUI::UI::D2D
{
	class Effect : public ComPtrHolder<ID2D1Effect>
	{
		public:
		Effect() noexcept = default;
		Effect(ComPtr<ID2D1Effect> effect) noexcept;

		[[nodiscard]] auto GetInput(UINT32 index) const noexcept -> D2DImage<>;
		[[nodiscard]] auto GetInputCount() const noexcept { return Get()->GetInputCount(); }
		[[nodiscard]] auto GetOutput() const noexcept -> D2DImage<>;
		void SetInput(UINT32 index, D2DImage<> image, bool invalidate) noexcept;
		void SetInputEffect(UINT32 index, Effect effect, bool invalidate) noexcept;
		void SetInputCount(UINT32 count);

		auto GetProperties() const noexcept { return D2DProperties{ Get() }; }
	};
}

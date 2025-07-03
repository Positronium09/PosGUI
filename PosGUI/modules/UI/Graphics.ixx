module;
#include <d2d1_3.h>

export module PGUI.UI.Graphics;

import PGUI.ComPtr;
import PGUI.UI.D2D.DeviceContext;

export namespace PGUI::UI
{
	class Graphics : public D2D::DeviceContext<ID2D1DeviceContext7>
	{
		public:
		explicit(false) Graphics(const ComPtr<ID2D1DeviceContext7>& deviceContext) noexcept;
	};
}

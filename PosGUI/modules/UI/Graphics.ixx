module;
#include <wrl.h>
#include <d2d1_3.h>

export module PGUI.UI.Graphics;

import PGUI.ComPtr;
import PGUI.UI.D2D.RenderTarget;

export namespace PGUI::UI
{
	class Graphics : public D2D::RenderTarget<ID2D1DeviceContext7>
	{
		public:
		Graphics(ComPtr<ID2D1DeviceContext7> deviceContext) noexcept;
	};
}

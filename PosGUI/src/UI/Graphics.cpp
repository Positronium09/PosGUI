module;
#include <wrl.h>
#include <d2d1_3.h>

module PGUI.UI.Graphics;

import PGUI.ComPtr;

namespace PGUI::UI
{
	Graphics::Graphics(ComPtr<ID2D1DeviceContext7> deviceContext) noexcept : 
		RenderTarget<ID2D1DeviceContext7>{ deviceContext }
	{
	}
}

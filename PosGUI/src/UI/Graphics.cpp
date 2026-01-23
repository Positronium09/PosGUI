module;
#include <d2d1_3.h>

module PGUI.UI.Graphics;

import PGUI.ComPtr;
import PGUI.UI.D2D.DeviceContext;

namespace PGUI::UI
{
	Graphics::Graphics(const ComPtr<ID2D1DeviceContext7>& deviceContext) noexcept :
		DeviceContext{ deviceContext }
	{
	}
}

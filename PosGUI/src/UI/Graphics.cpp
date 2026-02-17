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
		transformStack.reserve(8);
	}

	auto Graphics::PushTransform(const D2D::Matrix3x2& transform) const -> void
	{
		const auto currentTransform = GetTransform();
		transformStack.push_back(currentTransform);

		SetTransform(transform * currentTransform);
	}

	auto Graphics::PopTransform() const -> void
	{
		if (transformStack.empty())
		{
			return;
		}

		const auto& transform = transformStack.back();
		SetTransform(transform);
		transformStack.pop_back();
	}
}

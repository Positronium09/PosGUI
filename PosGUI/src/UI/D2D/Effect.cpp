module;
#include <wrl.h>
#include <d2d1_1.h>

module PGUI.UI.D2D.Effect;

import PGUI.ComPtr;
import PGUI.Exceptions;

namespace PGUI::UI::D2D
{
	Effect::Effect(ComPtr<ID2D1Effect> effect) noexcept : 
		ComPtrHolder{ effect }
	{
	}

	auto Effect::GetInput(UINT32 index) const noexcept -> D2DImage<>
	{
		D2DImage<> image;
		Get()->GetInput(index, image.GetAddress());

		return image;
	}

	auto Effect::GetOutput() const noexcept -> D2DImage<>
	{
		D2DImage<> image;
		Get()->GetOutput(image.GetAddress());

		return image;
	}

	void Effect::SetInput(UINT32 index, D2DImage<> image, bool invalidate) noexcept
	{
		Get()->SetInput(index, image.GetRaw(), invalidate);
	}

	void Effect::SetInputEffect(UINT32 index, Effect effect, bool invalidate) noexcept
	{
		Get()->SetInputEffect(index, effect.GetRaw(), invalidate);
	}

	void Effect::SetInputCount(UINT32 count)
	{
		auto hr = Get()->SetInputCount(count); ThrowFailed(hr);
	}


}

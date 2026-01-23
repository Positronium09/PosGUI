module;
#include <d2d1_1.h>

module PGUI.UI.D2D.Effect;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::UI::D2D
{
	Effect::Effect(const ComPtr<ID2D1Effect>& effect) noexcept :
		ComPtrHolder{ effect }
	{ }

	auto Effect::GetInput(const UINT32 index) const noexcept -> D2DImage<>
	{
		D2DImage<> image;
		Get()->GetInput(index, image.Put());

		return image;
	}

	auto Effect::GetOutput() const noexcept -> D2DImage<>
	{
		D2DImage<> image;
		Get()->GetOutput(image.Put());

		return image;
	}

	auto Effect::SetInput(
		const UINT32 index,
		D2DImage<> image,
		const bool invalidate) noexcept -> void
	{
		Get()->SetInput(index, image.GetRaw(), invalidate);
	}

	auto Effect::SetInputEffect(
		const UINT32 index,
		Effect effect,
		const bool invalidate) noexcept -> void
	{
		Get()->SetInputEffect(index, effect.GetRaw(), invalidate);
	}

	auto Effect::SetInputCount(const UINT32 count) noexcept -> Error
	{
		Error error{ Get()->SetInputCount(count) };
		error
			.AddDetail(L"Count", std::to_wstring(count));
		LogIfFailed(error, L"Failed to set input count");
		return error;
	}
}

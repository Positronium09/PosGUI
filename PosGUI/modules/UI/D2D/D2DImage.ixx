module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.D2DImage;

import std;

import PGUI.ComPtr;

export namespace PGUI::UI::D2D
{
	template <std::derived_from<ID2D1Image> Interface = ID2D1Image>
	class D2DImage : public ComPtrHolder<Interface>
	{
		public:
		D2DImage() noexcept = default;

		explicit(false) D2DImage(ComPtr<Interface> ptr) :
			ComPtrHolder<Interface>{ ptr }
		{ }

		explicit(false) operator D2DImage<>() const noexcept
		{
			return D2DImage<>{ this->Get() };
		}
	};
}

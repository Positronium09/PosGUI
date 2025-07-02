module;
#include <wrl.h>
#include <d2d1_1.h>

export module PGUI.UI.D2D.D2DImage;

import std;

import PGUI.ComPtr;

export namespace PGUI::UI::D2D
{
	template <typename Interface = ID2D1Image>
	class D2DImage : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, ID2D1Image>, "Interface must be derived from ID2D1Image");

		public:
		D2DImage() noexcept = default;
		D2DImage(ComPtr<Interface> ptr) :
			ComPtrHolder<Interface>{ ptr }
		{
		}
	};
}

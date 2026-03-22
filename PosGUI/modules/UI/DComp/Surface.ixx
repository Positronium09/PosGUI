module;
#include <dcomp.h>

export module PGUI.UI.DComp.Surface;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;

export namespace PGUI::DComp
{
	template <typename Interface = IDCompositionSurface>
	class Surface : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionSurface>);
		public:
		Surface() noexcept = default;
		explicit(false) Surface(Interface* ptr) noexcept : 
			ComPtrHolder<Interface>{ ptr }
		{ }

		//TODO BeginDraw

		auto EndDraw() const noexcept -> Error
		{
			return Error{ this->Get()->EndDraw() };
		}

		auto ResumeDraw() const noexcept -> Error
		{
			return Error{ this->Get()->ResumeDraw() };
		}

		auto SuspendDraw() const noexcept
		{
			return Error{ this->Get()->SuspendDraw() };
		}

		auto Scroll(RectL scrollRect, SizeI offset, std::optional<RectL> clipRect) const noexcept
		{
			const RECT* rect = nullptr;
			if (clipRect.has_value())
			{
				rect = std::bit_cast<RECT*>(&clipRect.value());
			}

			return Error{
				this->Get()->Scroll(
				std::bit_cast<const RECT*>(&scrollRect),
				rect, offset.cx, offset.cy)
			};
		}

		explicit(false) operator Surface<>() const noexcept
		{
			return Surface<>{ this->Get() };
		}
	};

	class VirtualSurface : public Surface<IDCompositionVirtualSurface>
	{
		public:
		VirtualSurface() noexcept = default;
		explicit(false) VirtualSurface(IDCompositionVirtualSurface* ptr) noexcept :
			Surface{ ptr }
		{ }

		auto Resize(const SizeU size) const noexcept
		{
			return Error{ Get()->Resize(size.cx, size.cy) };
		}

		auto Trim(std::span<RectL> rects) const noexcept
		{
			const RECT* rectArray = nullptr;
			if (!rects.empty())
			{
				rectArray = std::bit_cast<const RECT*>(rects.data());
			}
			return Error{ 
				Get()->Trim(
					rectArray, 
					static_cast<UINT>(rects.size())
				)
			};
		}
	};
}

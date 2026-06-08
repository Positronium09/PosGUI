module;
#include <d2d1_3.h>
#include <dcomp.h>

export module PGUI.UI.DComp.Surface;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Utils;
import PGUI.UI.Graphics;
import PGUI.ErrorHandling;

export namespace PGUI::UI::DComp
{
	template <typename Interface = IDCompositionSurface>
	class Surface : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, IDCompositionSurface>);

		public:
		Surface() noexcept = default;
		explicit(false) Surface(const ComPtr<Interface>& ptr) noexcept :
			ComPtrHolder<Interface>{ ptr }
		{ }

		[[nodiscard]] auto BeginDraw(std::optional<RectL> updateRect = std::nullopt) const noexcept -> Result<UI::Graphics>
		{
			const RECT* rect = nullptr;
			if (updateRect.has_value())
			{
				rect = std::bit_cast<const RECT*>(&updateRect.value());
			}

			ComPtr<ID2D1DeviceContext7> dc{ nullptr };
			POINT updateOffset{ };
			if (Error error{ this->Get()->BeginDraw(rect, GetIID<ID2D1DeviceContext7>(), dc.put_void(), &updateOffset) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			UI::Graphics graphics{ dc };
			graphics.PushTranslation(PointL{ updateOffset });

			return graphics;
		}

		[[nodiscard]] auto EndDraw() const noexcept -> Result<void>
		{
			if (Error error{ this->Get()->EndDraw() };
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto ResumeDraw() const noexcept -> Result<void>
		{
			if (Error error{ this->Get()->ResumeDraw() };
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto SuspendDraw() const noexcept -> Result<void>
		{
			if (Error error{ this->Get()->SuspendDraw() };
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto Scroll(RectL scrollRect, SizeI offset, std::optional<RectL> clipRect) const noexcept -> Result<void>
		{
			const RECT* rect = nullptr;
			if (clipRect.has_value())
			{
				rect = std::bit_cast<RECT*>(&clipRect.value());
			}

			if (const Error error{
					this->Get()->Scroll(
					std::bit_cast<const RECT*>(&scrollRect),
					rect, offset.cx, offset.cy)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
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
		explicit(false) VirtualSurface(const ComPtr<IDCompositionVirtualSurface>& ptr) noexcept :
			Surface{ ptr }
		{ }

		[[nodiscard]] auto Resize(const SizeU size) const noexcept -> Result<void>
		{
			if (Error error{ Get()->Resize(size.cx, size.cy) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto Trim(std::span<RectL> rects) const noexcept -> Result<void>
		{
			const RECT* rectArray = nullptr;
			if (!rects.empty())
			{
				rectArray = std::bit_cast<const RECT*>(rects.data());
			}
			if (const Error error{
					Get()->Trim(
						rectArray,
						static_cast<UINT>(rects.size())
					)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}
			return EmptyResult;
		}
	};
}

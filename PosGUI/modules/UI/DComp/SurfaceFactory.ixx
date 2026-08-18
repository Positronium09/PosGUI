module;
#include <dcomp.h>

export module PGUI.UI.DComp.SurfaceFactory;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.UI.DComp.Surface;
import PGUI.ErrorHandling;

export namespace PGUI::UI::DComp
{
	class SurfaceFactory : public ComPtrHolder<IDCompositionSurfaceFactory>
	{
		public:
		SurfaceFactory() noexcept = default;
		explicit(false) SurfaceFactory(const ComPtr<IDCompositionSurfaceFactory>& ptr) noexcept :
			ComPtrHolder{ ptr }
		{ }

		[[nodiscard]] auto CreateSurface(const SizeU size,
			const DXGI_FORMAT pixelFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
			const DXGI_ALPHA_MODE alphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED) const noexcept -> Result<Surface<>>
		{
			ComPtr<IDCompositionSurface> surface{ nullptr };
			if (const Error error{
					Get()->CreateSurface(size.cx, size.cy, pixelFormat, alphaMode, surface.put())
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return Surface{ surface };
		}

		[[nodiscard]] auto CreateVirtualSurface(const SizeU initialSize,
			const DXGI_FORMAT pixelFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
			const DXGI_ALPHA_MODE alphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED) const noexcept -> Result<VirtualSurface>
		{
			ComPtr<IDCompositionVirtualSurface> surface{ nullptr };
			if (const Error error{
					Get()->CreateVirtualSurface(initialSize.cx, initialSize.cy, pixelFormat, alphaMode, surface.put())
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return VirtualSurface{ surface };
		}
	};
}

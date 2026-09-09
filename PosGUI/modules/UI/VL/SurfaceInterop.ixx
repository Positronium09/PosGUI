module;
#include <winrt/Windows.UI.Composition.h>

#include <windows.ui.composition.interop.h>

#include <d2d1_3.h>

export module PGUI.UI.VL.SurfaceInterop;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Wrapper;
import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.ProjectionHolder;
import PGUI.UI.DXDevices;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.CompositionGraphicsDevice;
import PGUI.UI.VL.CompositionSurface;
import PGUI.UI.Graphics;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.Visual;

namespace WUC = winrt::Windows::UI::Composition;
namespace AWUC = ABI::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class SurfaceInterop : public CompositionObjectT<WUC::CompositionDrawingSurface>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] static auto Create(const SizeI size) noexcept -> Result<SurfaceInterop>
		{
			const auto surfaceResult = CallProjected([&size]
			{
				const auto graphicsDeviceResult = CompositionGraphicsDevice::GetGraphicsDevice();
				if (!graphicsDeviceResult.has_value())
				{
					throw Exception{ graphicsDeviceResult.error() };
				}
				const auto& graphicsDevice = graphicsDeviceResult.value();

				return graphicsDevice.CreateDrawingSurface2(
					size,
					winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
					winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied);
			});
			if (!surfaceResult.has_value())
			{
				return Unexpected{ surfaceResult.error() };
			}
			const auto& surface = surfaceResult.value();

			SurfaceInterop si{ surface };

			const auto interop = surface.try_as<AWUC::ICompositionDrawingSurfaceInterop2>();
			if (!interop)
			{
				return Unexpected{ Error{
					SystemErrorCode::InterfaceNotSupported
				}.SetCustomMessage(L"Failed to get AWUC::ICompositionDrawingSurfaceInterop2") };
			}

			si.drawingSurfaceInterop = WinrtToComPtr(interop);

			return si;
		}

		[[nodiscard]] auto BeginDraw() const noexcept -> Result<Graphics>
		{
			ComPtr<ID2D1DeviceContext> dc;
			POINT offset;
			if (const auto error = Error{
					drawingSurfaceInterop->BeginDraw(nullptr, GetIID(dc), dc.put_void(), &offset) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			const Graphics graphics{ dc.try_query<ID2D1DeviceContext7>() };
			if (graphics.Get().get() == nullptr)
			{
				return Unexpected{ Error{
					SystemErrorCode::InterfaceNotSupported
				}.SetCustomMessage(L"Failed to get ID2D1DeviceContext7") };
			}

			graphics.PushTranslation(offset);
			
			return graphics;
		}
		[[nodiscard]] auto EndDraw() const noexcept -> Result<void>
		{
			if (const auto error = Error{
					drawingSurfaceInterop->EndDraw()
				}; 
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
		[[nodiscard]] auto SuspendDraw() const noexcept -> Result<void>
		{
			if (const auto error = Error{
					drawingSurfaceInterop->SuspendDraw()
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}
		[[nodiscard]] auto ResumeDraw() const noexcept -> Result<void>
		{
			if (const auto error = Error{
					drawingSurfaceInterop->ResumeDraw()
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto Resize(const SizeL size) const noexcept -> Result<void>
		{
			if (const auto error = Error{
					drawingSurfaceInterop->Resize(size)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto Scroll(const PointI offset,
			const std::optional<RectL>& scrollRect = std::nullopt,
			const std::optional<RectL>& clipRect = std::nullopt) const noexcept -> Result<void>
		{
			const RECT* scrollRectPtr = nullptr;
			if (scrollRect.has_value())
			{
				scrollRectPtr = reinterpret_cast<const RECT*>(&scrollRect.value());
			}

			const RECT* clipRectPtr = nullptr;
			if (clipRect.has_value())
			{
				clipRectPtr = reinterpret_cast<const RECT*>(&clipRect.value());
			}

			if (const auto error = Error{
					drawingSurfaceInterop->Scroll(scrollRectPtr, clipRectPtr, offset.x, offset.y)
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		private:
		ComPtr<AWUC::ICompositionDrawingSurfaceInterop2> drawingSurfaceInterop;
	};
}

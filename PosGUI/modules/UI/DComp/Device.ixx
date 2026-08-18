module;
#include <dcomp.h>

export module PGUI.UI.DComp.Device;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.ErrorHandling;
import PGUI.UI.DComp.Target;
import PGUI.UI.DComp.Visual;
import PGUI.UI.DComp.Surface;
import PGUI.UI.DComp.SurfaceFactory;
import PGUI.UI.DComp.Effect;
import PGUI.UI.DComp.Transform2D;
import PGUI.UI.DComp.Transform3D;
import PGUI.UI.DComp.FilterEffects;
import PGUI.UI.DComp.Animation;

export namespace PGUI::UI::DComp
{
	class Device : public ComPtrHolder<IDCompositionDevice5, IDCompositionDesktopDevice>
	{
		public:
		Device() noexcept = default;
		explicit(false) Device(const ComPtr<IDCompositionDevice5>& ptr) noexcept :
			ComPtrHolder{ ptr, ptr.try_query<IDCompositionDesktopDevice>() }
		{ }

		[[nodiscard]] auto Commit() const noexcept -> Result<void>
		{
			if (const auto error = Error{ Get<IDCompositionDevice5>()->Commit() };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EmptyResult;
		}

		[[nodiscard]] auto CreateTargetForHwnd(const HWND hwnd, const bool topmost = false) const noexcept -> Result<Target>
		{
			if (!IsInitialized<IDCompositionDesktopDevice>())
			{
				const auto error = Error{ SystemErrorCode::InterfaceNotSupported }
					.AddDetail(L"Interface", L"IDCompositionDesktopDevice")
					.SuggestFix(L"If IDCompositionDevice5 is set with ComPtrHolder::Set also set the IDCompositionDesktopDevice interface");;
				
				return Unexpected{ error };
			}

			ComPtr<IDCompositionTarget> target{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDesktopDevice>()->CreateTargetForHwnd(hwnd, topmost, target.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return Target{ target };
		}

		[[nodiscard]] auto CreateVisual() const noexcept -> Result<Visual>
		{
			ComPtr<IDCompositionVisual2> visual{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateVisual(visual.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return Visual{ visual.try_query<IDCompositionVisual3>() };
		}

		[[nodiscard]] auto CreateDebugVisual() const noexcept -> Result<DebugVisual>
		{
			ComPtr<IDCompositionVisual2> visual{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateVisual(visual.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return DebugVisual{ visual.try_query<IDCompositionVisual3>() };
		}

		[[nodiscard]] auto CreateSurfaceFactory(const ComPtr<IUnknown>& renderingDevice) const noexcept -> Result<SurfaceFactory>
		{
			ComPtr<IDCompositionSurfaceFactory> surfaceFactory{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateSurfaceFactory(renderingDevice.get(), surfaceFactory.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return SurfaceFactory{ surfaceFactory };
		}

		[[nodiscard]] auto CreateSurface(const SizeU size,
			const DXGI_FORMAT pixelFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
			const DXGI_ALPHA_MODE alphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED) const noexcept -> Result<Surface<>>
		{
			ComPtr<IDCompositionSurface> surface{ nullptr };
			if (const auto error = Error{
					Get<IDCompositionDevice5>()->CreateSurface(size.cx, size.cy, pixelFormat, alphaMode, surface.put())
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
			if (const auto error = Error{
					Get<IDCompositionDevice5>()->CreateVirtualSurface(initialSize.cx, initialSize.cy, pixelFormat, alphaMode, surface.put())
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return VirtualSurface{ surface };
		}

		[[nodiscard]] auto CreateEffectGroup() const noexcept -> Result<EffectGroup<>>
		{
			ComPtr<IDCompositionEffectGroup> effectGroup{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateEffectGroup(effectGroup.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return EffectGroup{ effectGroup };
		}

		[[nodiscard]] auto CreateAnimation() const noexcept -> Result<Animation>
		{
			ComPtr<IDCompositionAnimation> animation{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateAnimation(animation.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return Animation{ animation };
		}

		[[nodiscard]] auto CreateTranslateTransform() const noexcept -> Result<TranslateTransform>
		{
			ComPtr<IDCompositionTranslateTransform> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateTranslateTransform(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return TranslateTransform{ transform };
		}

		[[nodiscard]] auto CreateScaleTransform() const noexcept -> Result<ScaleTransform>
		{
			ComPtr<IDCompositionScaleTransform> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateScaleTransform(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return ScaleTransform{ transform };
		}

		[[nodiscard]] auto CreateRotateTransform() const noexcept -> Result<RotateTransform>
		{
			ComPtr<IDCompositionRotateTransform> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateRotateTransform(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return RotateTransform{ transform };
		}

		[[nodiscard]] auto CreateSkewTransform() const noexcept -> Result<SkewTransform>
		{
			ComPtr<IDCompositionSkewTransform> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateSkewTransform(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return SkewTransform{ transform };
		}

		[[nodiscard]] auto CreateMatrixTransform() const noexcept -> Result<MatrixTransform>
		{
			ComPtr<IDCompositionMatrixTransform> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateMatrixTransform(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return MatrixTransform{ transform };
		}

		[[nodiscard]] auto CreateTransformGroup(const std::span<const Transform<>> transforms) const noexcept -> Result<Transform<>>
		{
			std::vector<IDCompositionTransform*> raw(transforms.size());
			std::ranges::transform(transforms, raw.begin(),
				[](const Transform<>& transform) noexcept { return transform.GetRaw(); });

			ComPtr<IDCompositionTransform> transformGroup{ nullptr };
			if (const auto error = Error{
					Get<IDCompositionDevice5>()->CreateTransformGroup(raw.data(), static_cast<UINT>(raw.size()), transformGroup.put())
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return Transform{ transformGroup };
		}

		[[nodiscard]] auto CreateTranslateTransform3D() const noexcept -> Result<TranslateTransform3D>
		{
			ComPtr<IDCompositionTranslateTransform3D> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateTranslateTransform3D(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return TranslateTransform3D{ transform };
		}

		[[nodiscard]] auto CreateScaleTransform3D() const noexcept -> Result<ScaleTransform3D>
		{
			ComPtr<IDCompositionScaleTransform3D> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateScaleTransform3D(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return ScaleTransform3D{ transform };
		}

		[[nodiscard]] auto CreateRotateTransform3D() const noexcept -> Result<RotateTransform3D>
		{
			ComPtr<IDCompositionRotateTransform3D> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateRotateTransform3D(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return RotateTransform3D{ transform };
		}

		[[nodiscard]] auto CreateMatrixTransform3D() const noexcept -> Result<MatrixTransform3D>
		{
			ComPtr<IDCompositionMatrixTransform3D> transform{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateMatrixTransform3D(transform.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return MatrixTransform3D{ transform };
		}

		[[nodiscard]] auto CreateTransform3DGroup(const std::span<const Transform3D<>> transforms) const noexcept -> Result<Transform3D<>>
		{
			std::vector<IDCompositionTransform3D*> raw(transforms.size());
			std::ranges::transform(transforms, raw.begin(),
				[](const Transform3D<>& transform) noexcept { return transform.GetRaw(); });

			ComPtr<IDCompositionTransform3D> transformGroup{ nullptr };
			if (const auto error = Error{
					Get<IDCompositionDevice5>()->CreateTransform3DGroup(raw.data(), static_cast<UINT>(raw.size()), transformGroup.put())
				};
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return Transform3D{ transformGroup };
		}

		[[nodiscard]] auto CreateGaussianBlurEffect() const noexcept -> Result<GaussianBlurEffect>
		{
			ComPtr<IDCompositionGaussianBlurEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateGaussianBlurEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return GaussianBlurEffect{ effect };
		}

		[[nodiscard]] auto CreateBrightnessEffect() const noexcept -> Result<BrightnessEffect>
		{
			ComPtr<IDCompositionBrightnessEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateBrightnessEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return BrightnessEffect{ effect };
		}

		[[nodiscard]] auto CreateColorMatrixEffect() const noexcept -> Result<ColorMatrixEffect>
		{
			ComPtr<IDCompositionColorMatrixEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateColorMatrixEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return ColorMatrixEffect{ effect };
		}

		[[nodiscard]] auto CreateShadowEffect() const noexcept -> Result<ShadowEffect>
		{
			ComPtr<IDCompositionShadowEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateShadowEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return ShadowEffect{ effect };
		}

		[[nodiscard]] auto CreateHueRotationEffect() const noexcept -> Result<HueRotationEffect>
		{
			ComPtr<IDCompositionHueRotationEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateHueRotationEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return HueRotationEffect{ effect };
		}

		[[nodiscard]] auto CreateSaturationEffect() const noexcept -> Result<SaturationEffect>
		{
			ComPtr<IDCompositionSaturationEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateSaturationEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return SaturationEffect{ effect };
		}

		[[nodiscard]] auto CreateTurbulenceEffect() const noexcept -> Result<TurbulenceEffect>
		{
			ComPtr<IDCompositionTurbulenceEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateTurbulenceEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return TurbulenceEffect{ effect };
		}

		[[nodiscard]] auto CreateLinearTransferEffect() const noexcept -> Result<LinearTransferEffect>
		{
			ComPtr<IDCompositionLinearTransferEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateLinearTransferEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return LinearTransferEffect{ effect };
		}

		[[nodiscard]] auto CreateTableTransferEffect() const noexcept -> Result<TableTransferEffect>
		{
			ComPtr<IDCompositionTableTransferEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateTableTransferEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return TableTransferEffect{ effect };
		}

		[[nodiscard]] auto CreateCompositeEffect() const noexcept -> Result<CompositeEffect>
		{
			ComPtr<IDCompositionCompositeEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateCompositeEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return CompositeEffect{ effect };
		}

		[[nodiscard]] auto CreateBlendEffect() const noexcept -> Result<BlendEffect>
		{
			ComPtr<IDCompositionBlendEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateBlendEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return BlendEffect{ effect };
		}

		[[nodiscard]] auto CreateArithmeticCompositeEffect() const noexcept -> Result<ArithmeticCompositeEffect>
		{
			ComPtr<IDCompositionArithmeticCompositeEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateArithmeticCompositeEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return ArithmeticCompositeEffect{ effect };
		}

		[[nodiscard]] auto CreateAffineTransform2DEffect() const noexcept -> Result<AffineTransform2DEffect>
		{
			ComPtr<IDCompositionAffineTransform2DEffect> effect{ nullptr };
			if (const auto error = Error{ Get<IDCompositionDevice5>()->CreateAffineTransform2DEffect(effect.put()) };
				error.IsFailure())
			{
				return Unexpected{ error };
			}

			return AffineTransform2DEffect{ effect };
		}
	};
}

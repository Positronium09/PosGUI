module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DLayer;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.Brush;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.D2D.D2DGeometry;

export namespace PGUI::UI::D2D
{
	struct LayerParameters : public D2D1_LAYER_PARAMETERS1
	{
		constexpr LayerParameters() noexcept = default;
		LayerParameters(RectF contentBounds, 
			AntiAliasingMode antiAliasingMode = AntiAliasingMode::PerPrimitive,
			LayerOptions layerOptions = LayerOptions::None,
			D2DGeometry<> geometryMask = D2DGeometry<>{ },
			Matrix3x2 maskTransform = Matrix3x2::Identity(),
			Brush opacityBrush = Brush{ EmptyBrush{ } }, float opacity = 1.0F) noexcept :
			D2D1_LAYER_PARAMETERS1{ .contentBounds = contentBounds, .geometricMask = geometryMask.GetRaw(),
			.maskAntialiasMode = static_cast<D2D1_ANTIALIAS_MODE>(antiAliasingMode),
			.maskTransform = maskTransform, 
			.opacity = opacity, .opacityBrush = opacityBrush,
			.layerOptions = static_cast<D2D1_LAYER_OPTIONS1>(layerOptions) }
		{
		}

		explicit(false) operator D2D1_LAYER_PARAMETERS1() const noexcept
		{
			return D2D1_LAYER_PARAMETERS1{ .contentBounds = contentBounds, .geometricMask = geometricMask,
				.maskAntialiasMode = static_cast<D2D1_ANTIALIAS_MODE>(maskAntialiasMode),
				.maskTransform = maskTransform, 
				.opacity = opacity, .opacityBrush = opacityBrush,
				.layerOptions = static_cast<D2D1_LAYER_OPTIONS1>(layerOptions) };
		}
		
		[[nodiscard]] auto GetContentBounds() const noexcept -> RectF { return contentBounds; }
		[[nodiscard]] auto GetGeometricMask() const noexcept { return D2DGeometry<ID2D1Geometry>{ geometricMask }; }
		[[nodiscard]] auto GetMaskAntialiasMode() const noexcept
		{
			return static_cast<AntiAliasingMode>(maskAntialiasMode);
		}
		[[nodiscard]] auto GetMaskTransform() const noexcept -> Matrix3x2
		{
			return D2D1MatrixToMatrix3x2(maskTransform);
		}
		[[nodiscard]] auto GetOpacity() const noexcept { return opacity; }
		[[nodiscard]] auto GetOpacityBrush() const noexcept { return Brush{ opacityBrush }; }
		[[nodiscard]] auto GetLayerOptions() const noexcept
		{
			return static_cast<LayerOptions>(layerOptions);
		}
	};

	class D2DLayer : public ComPtrHolder<ID2D1Layer>
	{
		public:
		D2DLayer() noexcept = default;
		D2DLayer(ComPtr<ID2D1Layer> layer) noexcept :
			ComPtrHolder<ID2D1Layer>{ layer }
		{
		}

		[[nodiscard]] auto GetSize() const noexcept -> SizeF
		{
			return this->Get()->GetSize();
		}
	};
}

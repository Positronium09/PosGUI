module;
#include <d2d1_3.h>
#include <dcomp.h>

export module PGUI.UI.DComp.Visual;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.Utils;
import PGUI.UI.Color;
import PGUI.UI.Graphics;
import PGUI.UI.DComp.DCompEnums;
import PGUI.UI.DComp.Surface;
import PGUI.UI.DComp.Effect;
import PGUI.UI.DComp.Clip;
import PGUI.UI.DComp.Transform2D;
import PGUI.UI.DComp.Transform3D;
import PGUI.UI.DComp.Animation;
import PGUI.UI.D2D.D2DStructs;
import PGUI.ErrorHandling;

export namespace PGUI::UI::DComp
{
	class Visual : public ComPtrHolder<IDCompositionVisual3>
	{
		public:
		Visual() noexcept = default;
		explicit(false) Visual(const ComPtr<IDCompositionVisual3>& ptr) noexcept :
			ComPtrHolder{ ptr }
		{ }

		[[nodiscard]] auto AddVisual(const Visual& visual, 
			bool insertAbove = false, 
			const Visual& referenceVisual = Visual{ }) const noexcept -> Result<void>;
		[[nodiscard]] auto RemoveAllVisuals() const noexcept -> Result<void>;
		[[nodiscard]] auto RemoveVisual(const Visual& visual) const noexcept -> Result<void>;

		[[nodiscard]] auto SetBitmapInterpolationMode(BitmapInterpolationMode interpolationMode) const noexcept -> Result<void>;
		[[nodiscard]] auto SetBorderMode(BorderMode borderMode) const noexcept -> Result<void>;

		[[nodiscard]] auto SetClip(const RectF& rc) const noexcept -> Result<void>;
		[[nodiscard]] auto SetClip(const Clip<>& clip) const noexcept -> Result<void>;

		[[nodiscard]] auto SetCompositeMode(CompositeMode compositeMode) const noexcept -> Result<void>;

		[[nodiscard]] auto SetContent(const ComPtr<IUnknown>& content) const noexcept -> Result<void>;
		[[nodiscard]] auto SetContent(const ComPtr<IDCompositionSurface>& content) const noexcept -> Result<void>;
		[[nodiscard]] auto SetContent(const ComPtr<IDXGISwapChain1>& content) const noexcept -> Result<void>;
		[[nodiscard]] auto SetContent(const Surface<>& content) const noexcept -> Result<void>;

		[[nodiscard]] auto SetEffect(const Effect<>& effect) const noexcept -> Result<void>;

		[[nodiscard]] auto SetOffset(const PointF& offset) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffset(const Animation& animationX, const Animation& animationY) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffsetX(float offset) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffsetX(const Animation& animation) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffsetY(float offset) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffsetY(const Animation& animation) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffsetZ(float offset) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOffsetZ(const Animation& animation) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOpacity(float opacity) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOpacity(const Animation& animation) const noexcept -> Result<void>;

		[[nodiscard]] auto SetVisible(bool isVisible) const noexcept -> Result<void>;

		[[nodiscard]] auto SetTransform(const Transform3D<>& transform) const noexcept -> Result<void>;
		[[nodiscard]] auto SetTransform(const Transform<>& transform) const noexcept -> Result<void>
		{
			return SetTransform(static_cast<Transform3D<>>(transform));
		}
		[[nodiscard]] auto SetTransform(const Matrix3x2& transform) const noexcept -> Result<void>;
		[[nodiscard]] auto SetTransform(const Matrix4x4& transform) const noexcept -> Result<void>;

		[[nodiscard]] auto SetTransformParent(const Visual& parent) const noexcept -> Result<void>;

		[[nodiscard]] auto SetBackFaceVisibility(BackFaceVisibility visibility) const noexcept -> Result<void>;
		[[nodiscard]] auto SetOpacityMode(OpacityMode opacityMode) const noexcept -> Result<void>;

		[[nodiscard]] auto SetDepthMode(DepthMode depthMode) const noexcept -> Result<void>;
	};

	class DebugVisual : public Visual
	{
		public:
		DebugVisual() noexcept = default;

		explicit(false) DebugVisual(const ComPtr<IDCompositionVisual3>& ptr) noexcept :
			Visual{ ptr }, debug{ ptr.try_query<IDCompositionVisualDebug>() }
		{ }

		[[nodiscard]] auto EnableHeatMap(const RGBA& color) const noexcept -> Result<void>;
		[[nodiscard]] auto DisableHeatMap() const noexcept -> Result<void>;
		[[nodiscard]] auto EnableRedrawRegions() const noexcept -> Result<void>;
		[[nodiscard]] auto DisableRedrawRegions() const noexcept -> Result<void>;

		private:
		ComPtr<IDCompositionVisualDebug> debug;
	};
}

module;
#include <d2d1_3.h>
#include <dcomp.h>

module PGUI.UI.DComp.Visual;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.Utils;
import PGUI.UI.Graphics;
import PGUI.UI.DComp.Animation;
import PGUI.ErrorHandling;

namespace PGUI::UI::DComp
{
	auto Visual::AddVisual(const Visual& visual, const bool insertAbove,
	                       const Visual& referenceVisual) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->AddVisual(visual.GetRaw(), insertAbove, referenceVisual.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::RemoveAllVisuals() const noexcept -> Result<void>
	{
		if (const auto error = Error{ Get()->RemoveAllVisuals() };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::RemoveVisual(const Visual& visual) const noexcept -> Result<void>
	{
		if (const auto error = Error{ Get()->RemoveVisual(visual.GetRaw()) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetBitmapInterpolationMode(
		const BitmapInterpolationMode interpolationMode) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetBitmapInterpolationMode(
					ToUnderlying<DCOMPOSITION_BITMAP_INTERPOLATION_MODE>(interpolationMode))
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetBorderMode(const BorderMode borderMode) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetBorderMode(ToUnderlying<DCOMPOSITION_BORDER_MODE>(borderMode))
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetClip(const RectF& rc) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetClip(rc)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetClip(const Clip<>& clip) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetClip(clip.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetCompositeMode(const CompositeMode compositeMode) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetCompositeMode(ToUnderlying<DCOMPOSITION_COMPOSITE_MODE>(compositeMode))
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetContent(const ComPtr<IUnknown>& content) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetContent(content.get())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetContent(const ComPtr<IDCompositionSurface>& content) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetContent(content.get())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetContent(const ComPtr<IDXGISwapChain1>& content) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetContent(content.get())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetContent(const Surface<>& content) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetContent(content.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetEffect(const Effect<>& effect) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetEffect(effect.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffset(const PointF& offset) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetX(offset.x)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		if (const auto error = Error{
				Get()->SetOffsetY(offset.y)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffset(const Animation& animationX, const Animation& animationY) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetX(animationX.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		if (const auto error = Error{
				Get()->SetOffsetY(animationY.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffsetX(const float offset) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetX(offset)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffsetX(const Animation& animation) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetX(animation.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffsetY(const float offset) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetY(offset)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffsetY(const Animation& animation) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetY(animation.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffsetZ(const float offset) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetZ(offset)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOffsetZ(const Animation& animation) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOffsetZ(animation.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOpacity(const float opacity) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOpacity(opacity)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOpacity(const Animation& animation) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOpacity(animation.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetVisible(const bool isVisible) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetVisible(isVisible)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetTransform(const Transform3D<>& transform) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetTransform(transform.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}
		return EmptyResult;
	}

	auto Visual::SetTransform(const Matrix3x2& transform) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				GetUpCast<IDCompositionVisual>()->SetTransform(transform)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetTransform(const Matrix4x4& transform) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetTransform(transform)
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetTransformParent(const Visual& parent) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetTransformParent(parent.GetRaw())
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetBackFaceVisibility(const BackFaceVisibility visibility) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetBackFaceVisibility(
					ToUnderlying<DCOMPOSITION_BACKFACE_VISIBILITY>(visibility))
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetOpacityMode(const OpacityMode opacityMode) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetOpacityMode(ToUnderlying<DCOMPOSITION_OPACITY_MODE>(opacityMode))
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto Visual::SetDepthMode(const DepthMode depthMode) const noexcept -> Result<void>
	{
		if (const auto error = Error{
				Get()->SetDepthMode(ToUnderlying<DCOMPOSITION_DEPTH_MODE>(depthMode))
			};
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto DebugVisual::EnableHeatMap(const RGBA& color) const noexcept -> Result<void>
	{
		if (!debug)
		{
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		if (const auto error = Error{ debug->EnableHeatMap(color) };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto DebugVisual::DisableHeatMap() const noexcept -> Result<void>
	{
		if (!debug)
		{
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		if (const auto error = Error{ debug->DisableHeatMap() };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto DebugVisual::EnableRedrawRegions() const noexcept -> Result<void>
	{
		if (!debug)
		{
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		if (const auto error = Error{ debug->EnableRedrawRegions() };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}

	auto DebugVisual::DisableRedrawRegions() const noexcept -> Result<void>
	{
		if (!debug)
		{
			return Unexpected{ Error{ SystemErrorCode::InterfaceNotSupported } };
		}

		if (const auto error = Error{ debug->DisableRedrawRegions() };
			error.IsFailure())
		{
			return Unexpected{ error };
		}

		return EmptyResult;
	}
}

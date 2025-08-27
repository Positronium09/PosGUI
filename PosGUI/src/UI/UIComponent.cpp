module PGUI.UI.UIComponent;

import std;

import PGUI.Shape2D;
import PGUI.WindowClass;
import PGUI.UI.Colors;
import PGUI.UI.Clip;
import PGUI.UI.D2D;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIComponent::UIComponent(const RectF bounds) :
		bounds{ bounds }
	{ }

	auto UIComponent::SetClip(const ClipParameters& params) noexcept -> void
	{
		clip.SetParameters(params);
		AdjustClip();
		clipChanged.Invoke();
	}

	auto UIComponent::ClearClip() noexcept -> void
	{
		clip.Clear();
		clipChanged.Invoke();
	}

	auto UIComponent::SetBounds(const RectF& rect) noexcept -> void
	{
		if (bounds == rect)
		{
			return;
		}

		const auto oldBounds = bounds;
		bounds = rect;
		if (adjustClipOnSize && oldBounds.Size() != bounds.Size())
		{
			AdjustClip();
		}
		boundsChanged.Invoke(oldBounds, bounds);
	}

	auto UIComponent::AdjustClip() noexcept -> void
	{
		if (auto& clipParams = clip.GetParameters();
			std::holds_alternative<EllipseClipParameters>(clipParams))
		{
			EllipseClipParameters& params = std::get<EllipseClipParameters>(clipParams);
			params.ellipse.center = bounds.Center();
		}
		else if (std::holds_alternative<RectangleClipParameters>(clipParams))
		{
			RectangleClipParameters& params = std::get<RectangleClipParameters>(clipParams);
			params.rect = bounds;
		}
		else if (std::holds_alternative<RoundedRectangleClipParameters>(clipParams))
		{
			RoundedRectangleClipParameters& params = std::get<RoundedRectangleClipParameters>(clipParams);
			params.rect.left = bounds.left;
			params.rect.top = bounds.top;
			params.rect.right = bounds.right;
			params.rect.bottom = bounds.bottom;
		}
		else if (std::holds_alternative<RoundCornerClipParameters>(clipParams))
		{
			RoundCornerClipParameters& params = std::get<RoundCornerClipParameters>(clipParams);
			params.rect = bounds;
		}

		clip.CreateClip();
	}
}

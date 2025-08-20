module;
#include <d2d1_3.h>
#include <Windows.h>

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
	UIComponent::UIComponent() :
		UIComponent{ WindowClass::Create(L"PGUI_UIComponent") }
	{ }

	UIComponent::UIComponent(const WindowClassPtr& wndClass) :
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_CREATE, &UIComponent::OnCreate);
		RegisterHandler(WM_NCHITTEST, &UIComponent::OnNcHitTest);
		RegisterHandler(WM_SIZE, &UIComponent::OnSize);
	}

	auto UIComponent::SetClip(const ClipParameters& params) noexcept -> void
	{
		clip.SetParameters(params);
		AdjustClip();
		Invalidate();
		OnClipChanged();
	}

	auto UIComponent::ClearClip() noexcept -> void
	{
		clip.Clear();
		OnClipChanged();
	}

	auto UIComponent::BeginDraw() -> void
	{
		DirectXCompositionWindow::BeginDraw();

		auto graphics = GetGraphics();

		graphics.Clear(Colors::Transparent);

		graphics.PushLayer(D2D::LayerParameters{
			InfiniteRect<float>(),
			D2D::AntiAliasingMode::PerPrimitive,
			D2D::LayerOptions::None,
			D2D::D2DGeometry<>{ clip }
		});
	}

	auto UIComponent::EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG>
	{
		auto graphics = GetGraphics();
		graphics.PopLayer();

		return DirectXCompositionWindow::EndDraw();
	}

	auto UIComponent::AdjustClip() noexcept -> void
	{
		if (!adjustClipOnSize)
		{
			return;
		}

		auto& clipParams = clip.GetParameters();

		const RectF clientRect = GetClientRect();

		if (std::holds_alternative<EllipseClipParameters>(clipParams))
		{
			EllipseClipParameters& params = std::get<EllipseClipParameters>(clipParams);
			params.ellipse.center = clientRect.Center();
		}
		else if (std::holds_alternative<RectangleClipParameters>(clipParams))
		{
			RectangleClipParameters& params = std::get<RectangleClipParameters>(clipParams);
			params.rect = clientRect;
		}
		else if (std::holds_alternative<RoundedRectangleClipParameters>(clipParams))
		{
			RoundedRectangleClipParameters& params = std::get<RoundedRectangleClipParameters>(clipParams);
			params.rect.left = clientRect.left;
			params.rect.top = clientRect.top;
			params.rect.right = clientRect.right;
			params.rect.bottom = clientRect.bottom;
		}
		else if (std::holds_alternative<RoundCornerClipParameters>(clipParams))
		{
			RoundCornerClipParameters& params = std::get<RoundCornerClipParameters>(clipParams);
			params.rect = clientRect;
		}

		clip.CreateClip();
	}

	auto UIComponent::OnCreate(
		UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) const noexcept -> MessageHandlerResult
	{
		DisableInput();

		return 0;
	}

	auto UIComponent::OnNcHitTest(
		const UINT msg, const WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		const auto defResult = DefWindowProcW(Hwnd(), msg, wParam, lParam);

		if (!hitTestClip || defResult != HTCLIENT || !clip.GetGeometry().IsInitialized())
		{
			return defResult;
		}

		const PointL point = ScreenToClient(MAKEPOINTS(lParam));

		if (const auto contains = clip.GetGeometry().FillContainsPoint(point);
			!contains)
		{
			return { HTTRANSPARENT, HandlerReturnFlags::ForceThisResult };
		}

		return defResult;
	}

	auto UIComponent::OnSize(
		UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) noexcept -> MessageHandlerResult
	{
		AdjustClip();

		return 0;
	}
}

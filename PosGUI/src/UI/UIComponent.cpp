module;
#include <variant>
#include <d2d1_3.h>
#include <Windows.h>

module PGUI.UI.UIComponent;

import PGUI.Shape2D;
import PGUI.WindowClass;
import PGUI.UI.Colors;
import PGUI.UI.Clip;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.DirectXCompositionWindow;

namespace PGUI::UI
{
	UIComponent::UIComponent() : 
		UIComponent{ WindowClass::Create(L"PGUI_UIComponent") }
	{
	}
	UIComponent::UIComponent(const WindowClassPtr& wndClass) : 
		DirectXCompositionWindow{ wndClass }
	{
		RegisterHandler(WM_CREATE, &UIComponent::OnCreate);
		RegisterHandler(WM_NCHITTEST, &UIComponent::OnNcHitTest);
		RegisterHandler(WM_SIZE, &UIComponent::OnSize);
	}

	void UIComponent::SetClip(const ClipParameters& params) noexcept
	{
		clip.SetParameters(params);
		AdjustClip();
		Invalidate();
		OnClipChanged();
	}
	void UIComponent::ClearClip() noexcept
	{
		clip.Clear();
		OnClipChanged();
	}

	void UIComponent::BeginDraw()
	{
		DirectXCompositionWindow::BeginDraw();

		auto graphics = GetGraphics();

		if (autoDpiScaledDrawing)
		{
			RectF clientRect = GetClientRect();
			auto dpi = static_cast<float>(GetDPI()) / DEFAULT_SCREEN_DPI;

			auto scaleMatrix = D2D::Matrix3x2::Scale(dpi, dpi, clientRect.Center());
			auto prevTransform = graphics.GetTransform();
			graphics.SetTransform(scaleMatrix * prevTransform);
		}

		graphics.Clear(Colors::Transparent);

		graphics.PushLayer(D2D::LayerParameters{ 
			InfiniteRect<float>(), D2D::AntiAliasingMode::PerPrimitive,
			D2D::LayerOptions::None, D2D::D2DGeometry<>{ clip } });
	}

	auto UIComponent::EndDraw() -> std::pair<D2D1_TAG, D2D1_TAG>
	{
		auto graphics = GetGraphics();
		graphics.PopLayer();

		if (autoDpiScaledDrawing)
		{
			RectF clientRect = GetClientRect();
			auto dpi = static_cast<float>(GetDPI()) / DEFAULT_SCREEN_DPI;

			auto scaleMatrix = D2D::Matrix3x2::Scale(dpi, dpi, clientRect.Center());
			scaleMatrix.Invert();

			graphics.SetTransform(graphics.GetTransform() * scaleMatrix);
		}

		return DirectXCompositionWindow::EndDraw();
	}

	void UIComponent::AdjustClip() noexcept
	{
		if (!adjustClipOnSize)
		{
			return;
		}

		auto& clipParams = clip.GetParameters();

		RectF clientRect = GetClientRect();

		if (std::holds_alternative<EllipseClipParameters>(clipParams))
		{
			auto& params = std::get<EllipseClipParameters>(clipParams);
			params.ellipse.center = clientRect.Center();
		}
		else if (std::holds_alternative<RectangleClipParameters>(clipParams))
		{
			auto& params = std::get<RectangleClipParameters>(clipParams);
			params.rect = clientRect;
		}
		else if (std::holds_alternative<RoundedRectangleClipParameters>(clipParams))
		{
			auto& params = std::get<RoundedRectangleClipParameters>(clipParams);
			params.rect.left = clientRect.left;
			params.rect.top = clientRect.top;
			params.rect.right = clientRect.right;
			params.rect.bottom = clientRect.bottom;
		}
		else if (std::holds_alternative<RoundCornerClipParameters>(clipParams))
		{
			auto& params = std::get<RoundCornerClipParameters>(clipParams);
			params.rect = clientRect;
		}

		clip.CreateClip();
	}

	auto UIComponent::OnCreate(UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) const noexcept -> MessageHandlerResult
	{
		DisableInput();

		return 0;
	}
	auto UIComponent::OnNcHitTest(UINT msg, WPARAM wParam, LPARAM lParam) const noexcept -> MessageHandlerResult
	{
		auto defResult = DefWindowProcW(Hwnd(), msg, wParam, lParam);

		if (!hitTestClip || defResult != HTCLIENT || !clip.GetGeometry().IsInitialized())
		{
			return defResult;
		}

		PointL point = ScreenToClient(MAKEPOINTS(lParam));

		auto contains = clip.GetGeometry().FillContainsPoint(point);

		if (!contains)
		{
			return { HTTRANSPARENT, ReturnFlags::ForceThisResult };
		}

		return defResult;
	}
	auto UIComponent::OnSize(UINT /* unused */, WPARAM /* unused */, LPARAM /* unused */) noexcept -> MessageHandlerResult
	{
		AdjustClip();

		return 0;
	}
}

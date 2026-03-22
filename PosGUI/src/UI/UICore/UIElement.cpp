module;
#include <dxgi1_6.h>

module PGUI.UI.UICore:UIElement;

import :Interface;
import :UIEvent;
import :UIHost;

import PGUI.UI.D2D.D2DEnums;

namespace PGUI::UI
{
	auto UIElement::SetParent(const RawUIElementPtr<> newParent) noexcept
	{
		parent = newParent;
	}

	auto UIElement::DisallowFocus() noexcept
	{
		canHaveFocus = false;
		if (HasFocus())
		{
			RemoveFocus();
		}
	}

	auto UIElement::RequestRedraw() noexcept -> void
	{
		if (parent)
		{
			parent->RedrawRequestedEvent().Invoke(this);
		}
		else if (host)
		{
			host->RedrawRequestedEvent().Invoke(this);
		}
	}

	auto UIElement::CompositionModeChanged(const bool enabled) noexcept -> Error
	{
		if (IsComposed() == enabled)
		{
			return Error{ ErrorCode::Success };
		}

		const auto& rootVisual = host->GetDCompositionVisual();

		if (!enabled)
		{
			if (const auto hr = rootVisual->RemoveVisual(compositionVisual.get());
				FAILED(hr))
			{
				return Error{ hr }.SetCustomMessage(L"Failed to remove the visual");
			}
			(void)compositionVisual->SetContent(nullptr);

			compositionVisual.reset();
			compositionSurface.reset();

			return Error{ ErrorCode::Success };
		}

		const auto& device = host->DCompositionDevice();
		const auto size = GetSize();

		ComPtr<IDCompositionVisual2> visualBase;
		auto hr = device->CreateVisual(&visualBase);

		if (FAILED(hr))
		{
			return Error{ hr }.SetCustomMessage(L"Cannot create DComposition visual");
		}

		compositionVisual = visualBase.try_query<IDCompositionVisual3>();

		if (compositionVisual.get() == nullptr)
		{
			return Error{ E_NOINTERFACE }.SetCustomMessage(L"Cannot query IDCompositionVisual3 interface");
		}

		hr = device->CreateVirtualSurface(
			static_cast<UINT>(size.cx), static_cast<UINT>(size.cy),
			DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ALPHA_MODE_PREMULTIPLIED,
			compositionSurface.put());

		if (FAILED(hr))
		{
			return Error{ hr }.SetCustomMessage(L"Failed to create the virtual surface");
		}

		hr = compositionVisual->SetContent(compositionSurface.get());
		if (FAILED(hr))
		{
			return Error{ hr }.SetCustomMessage(L"Failed to set the visual content");
		}

		hr = rootVisual->AddVisual(compositionVisual.get(), TRUE, nullptr);
		if (FAILED(hr))
		{
			return Error{ hr }.SetCustomMessage(L"Failed to add the visual to the root visual");
		}

		return Error{ ErrorCode::Success };
	}
}

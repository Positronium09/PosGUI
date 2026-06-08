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
}

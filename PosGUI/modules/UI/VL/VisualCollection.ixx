module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.VisualCollection;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.VLEnums;
import PGUI.UI.VL.Visual;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class VisualCollection : public CompositionObjectT<WUC::VisualCollection>
	{
		public:
		using Base = CompositionObjectT;
		using Base::Base;

		[[nodiscard]] auto GetCount() const noexcept -> Result<std::int32_t>
		{
			return CallProjected([this]
			{
				return Get().Count();
			});
		}

		auto InsertAbove(const Visual& newChild, const Visual& sibling) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild, &sibling]
			{
				Get().InsertAbove(newChild.Get(), sibling.Get());
			});
		}

		auto InsertAtBottom(const Visual& newChild) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild]
			{
				Get().InsertAtBottom(newChild.Get());
			});
		}

		auto InsertAtTop(const Visual& newChild) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild]
			{
				Get().InsertAtTop(newChild.Get());
			});
		}

		auto InsertBelow(const Visual& newChild, const Visual& sibling) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild, &sibling]
			{
				Get().InsertBelow(newChild.Get(), sibling.Get());
			});
		}

		auto Remove(const Visual& child) const noexcept -> Result<void>
		{
			return CallProjected([this, &child]
			{
				Get().Remove(child.Get());
			});
		}

		auto RemoveAll() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				Get().RemoveAll();
			});
		}
	};
}

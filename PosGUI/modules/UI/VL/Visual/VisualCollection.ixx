module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.Visual.VisualCollection;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.Visual.Visual;

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

		[[nodiscard]] auto InsertAbove(const Visual& newChild, const Visual& sibling) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild, &sibling]
			{
				Get().InsertAbove(newChild.Get(), sibling.Get());
			});
		}

		[[nodiscard]] auto InsertAtBottom(const Visual& newChild) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild]
			{
				Get().InsertAtBottom(newChild.Get());
			});
		}

		[[nodiscard]] auto InsertAtTop(const Visual& newChild) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild]
			{
				Get().InsertAtTop(newChild.Get());
			});
		}

		[[nodiscard]] auto InsertBelow(const Visual& newChild, const Visual& sibling) const noexcept -> Result<void>
		{
			return CallProjected([this, &newChild, &sibling]
			{
				Get().InsertBelow(newChild.Get(), sibling.Get());
			});
		}

		[[nodiscard]] auto Remove(const Visual& child) const noexcept -> Result<void>
		{
			return CallProjected([this, &child]
			{
				Get().Remove(child.Get());
			});
		}

		[[nodiscard]] auto RemoveAll() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				Get().RemoveAll();
			});
		}

		[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<Visual>>
		{
			return CallProjected([this]
			{
				auto visuals = std::vector<Visual>{ };
				visuals.reserve(static_cast<std::size_t>(Get().Count()));

				for (const auto& v : Get())
				{
					visuals.emplace_back(v);
				}

				return visuals;
			});
		}

		template <Callable<Result<void>(Visual)> F>
		[[nodiscard]] auto ForEach(F&& f) const noexcept -> Result<void> 
		{
			return CallProjected([this, &f] -> Result<void>
			{
				for (const auto& [i, v] : Get() | std::views::enumerate)
				{
					if (auto res = std::invoke(f, Visual{ v });
						!res.has_value())
					{
						return Unexpected{ 
							res.error()
							.AddDetail(L"Iteration", std::to_wstring(i))
							.AddDetail(L"Visual", std::format(L"{}", winrt::get_unknown(v)))
						};
					}
				}

				return EmptyResult;
			}).and_then(std::identity{ });
		}
	};
}

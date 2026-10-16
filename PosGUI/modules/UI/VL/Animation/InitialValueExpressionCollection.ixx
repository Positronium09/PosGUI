module;
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Windows.Foundation.Collections.h>

export module PGUI.UI.VL.InitialValueExpressionCollection;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Utils;
import PGUI.Wrapper;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class InitialValueExpressionCollection : public CompositionObjectT<WUC::InitialValueExpressionCollection>
	{
		public:
		using CompositionObjectT::CompositionObjectT;

		[[nodiscard]] auto GetSize() const noexcept -> Result<std::uint32_t>
		{
			return CallProjected([this]
			{
				return Get().Size();
			});
		}

		[[nodiscard]] auto GetView() const noexcept -> Result<std::map<std::wstring, std::wstring>>
		{
			return CallProjected([this]
			{
				return Get().GetView() | std::views::transform([](const auto& element)
				{
					return std::pair{ std::wstring{ element.Key() }, std::wstring{ element.Value() } };
				}) | std::ranges::to<std::map>();
			});
		}

		[[nodiscard]] auto Clear() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				Get().Clear();
			});
		}

		[[nodiscard]] auto HasKey(const std::wstring_view key) const noexcept -> Result<bool>
		{
			return CallProjected([this, &key]
			{
				return Get().HasKey(key);
			});
		}

		[[nodiscard]] auto Insert(const std::wstring_view key, const std::wstring_view value) const noexcept -> Result<bool>
		{
			return CallProjected([this, &key, &value]
			{
				return Get().Insert(key, value);
			});
		}

		[[nodiscard]] auto Lookup(const std::wstring_view key) const noexcept -> Result<std::wstring>
		{
			return CallProjected([this, &key]
			{
				return std::wstring{ Get().Lookup(key) };
			});
		}
		[[nodiscard]] auto TryLookup(const std::wstring_view key) const noexcept -> Result<std::optional<std::wstring>>
		{
			return CallProjected([this, &key]
			{
				return Get().TryLookup(key).transform([](const auto& value)
				{
					return std::wstring{ value };
				});
			});
		}

		[[nodiscard]] auto Remove(const std::wstring_view key) const noexcept -> Result<void>
		{
			return CallProjected([this, &key]
			{
				Get().Remove(key);
			});
		}
		[[nodiscard]] auto TryRemove(const std::wstring_view key) const noexcept -> Result<bool>
		{
			return CallProjected([this, &key]
			{
				return Get().TryRemove(key);
			});
		}

		[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<std::pair<std::wstring, std::wstring>>>
		{
			return CallProjected([this]
			{
				auto elements = std::vector<std::pair<std::wstring, std::wstring>>{ };
				elements.reserve(Get().Size());

				for (const auto& element : Get())
				{
					elements.emplace_back(std::wstring{ element.Key() }, std::wstring{ element.Value() });
				}

				return elements;
			});
		}

		template <Callable<Result<void>(std::wstring, std::wstring)> F>
		[[nodiscard]] auto ForEach(F&& f) const noexcept -> Result<void>
		{
			return CallProjected([this, &f] -> Result<void>
			{
				for (const auto& [i, element] : Get() | std::views::enumerate)
				{
					if (auto result = std::invoke(f, std::wstring{ element.Key() }, std::wstring{ element.Value() });
						!result.has_value())
					{
						return Unexpected{ result.error().AddDetail(L"iteration", std::to_wstring(i)) };
					}
				}

				return EmptyResult;
			}).and_then(std::identity{ });
		}
	};
}

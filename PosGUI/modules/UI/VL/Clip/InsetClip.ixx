module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Clip.InsetClip;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Clip.CompositionClip;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class InsetClip : public CompositionClipT<WUC::InsetClip>
	{
		public:
		using CompositionClipT::CompositionClipT;

		[[nodiscard]] static auto Create(
			const float left, const float top,
			const float right, const float bottom) noexcept -> Result<InsetClip>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();

			return CallProjected([&compositor, left, top, right, bottom]
			{
				return InsetClip{ compositor.CreateInsetClip(left, top, right, bottom) };
			});
		}

		[[nodiscard]] static auto Create(const RectF& rect) noexcept -> Result<InsetClip>
		{
			return Create(rect.left, rect.top, rect.right, rect.bottom);
		}

		[[nodiscard]] auto GetInsets() const noexcept -> Result<RectF>
		{
			std::array results{
				GetLeftInset(),
				GetTopInset(),
				GetRightInset(),
				GetBottomInset()
			};

			if (const auto iter = std::ranges::find_if(results, [](const auto& res) { return !res.has_value(); });
				iter != results.end())
			{
				return Unexpected{ iter->error() };
			}

			return RectF{
				results[0].value(),
				results[1].value(),
				results[2].value(),
				results[3].value()
			};
		}
		[[nodiscard]] auto SetInsets(
			const float left, const float top,
			const float right, const float bottom) const noexcept -> Result<void>
		{
			if (auto res = SetLeftInset(left);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Left") };
			}
			if (auto res = SetTopInset(top);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Top") };
			}
			if (auto res = SetRightInset(right);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Right") };
			}
			if (auto res = SetBottomInset(bottom);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Bottom") };
			}

			return EmptyResult;
		}
		[[nodiscard]] auto SetInsets(const RectF& rc) const noexcept -> Result<void>
		{
			return SetInsets(rc.left, rc.top, rc.right, rc.bottom);
		}

		[[nodiscard]] auto GetLeftInset() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().LeftInset();
			});
		}
		[[nodiscard]] auto SetLeftInset(const float leftInset) const noexcept -> Result<void>
		{
			return CallProjected([this, leftInset]
			{
				Get().LeftInset(leftInset);
			});
		}

		[[nodiscard]] auto GetTopInset() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().TopInset();
			});
		}
		[[nodiscard]] auto SetTopInset(const float topInset) const noexcept -> Result<void>
		{
			return CallProjected([this, topInset]
			{
				Get().TopInset(topInset);
			});
		}

		[[nodiscard]] auto GetRightInset() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().RightInset();
			});
		}
		[[nodiscard]] auto SetRightInset(const float rightInset) const noexcept -> Result<void>
		{
			return CallProjected([this, rightInset]
			{
				Get().RightInset(rightInset);
			});
		}

		[[nodiscard]] auto GetBottomInset() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().BottomInset();
			});
		}
		[[nodiscard]] auto SetBottomInset(const float bottomInset) const noexcept -> Result<void>
		{
			return CallProjected([this, bottomInset]
			{
				Get().BottomInset(bottomInset);
			});
		}
	};
}

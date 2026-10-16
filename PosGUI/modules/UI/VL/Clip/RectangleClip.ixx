module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.Clip.RectangleClip;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.Clip;
import PGUI.UI.VL.Compositor;
import PGUI.UI.VL.Clip.CompositionClip;
import PGUI.UI.VL.VLEnums;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	class RectangleClip : public CompositionClipT<WUC::RectangleClip>
	{
		public:
		using CompositionClipT::CompositionClipT;

		[[nodiscard]] static auto Create(const RectF& rc) noexcept -> Result<RectangleClip>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();
			return CallProjected([&]
			{
				return RectangleClip{ compositor.CreateRectangleClip(rc.left, rc.top, rc.right, rc.bottom) };
			});
		}
		[[nodiscard]] static auto Create(
			const RectF& rc,
			const Point2F& radius
			) noexcept -> Result<RectangleClip>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();
			return CallProjected([&]
			{
				return RectangleClip{
					compositor.CreateRectangleClip(
						rc.left, rc.top, rc.right, rc.bottom,
						radius, radius, radius, radius
					)
				};
			});
		}
		[[nodiscard]] static auto Create(
			const RectF& rc,
			const Point2F& topLeftRadius, const Point2F& topRightRadius, 
			const Point2F& bottomLeftRadius, const Point2F& bottomRightRadius
		) noexcept -> Result<RectangleClip>
		{
			const auto result = Compositor::GetCompositor();
			if (!result.has_value())
			{
				return Unexpected{ result.error() };
			}
			const auto& compositor = result.value();
			return CallProjected([&]
			{
				return RectangleClip{
					compositor.CreateRectangleClip(
						rc.left, rc.top, rc.right, rc.bottom,
						topLeftRadius, topRightRadius, bottomLeftRadius, bottomRightRadius
					)
				};
			});
		}
		[[nodiscard]] static auto Create(const RoundedRect& roundedRect) noexcept -> Result<RectangleClip>
		{
			return Create(roundedRect, roundedRect.GetRadii());
		}

		[[nodiscard]] static auto CreateFrom(const Clip& clip) noexcept -> Result<RectangleClip>
		{
			if (const auto& parameters = clip.GetParameters();
				std::holds_alternative<RectangleClipParameters>(parameters))
			{
				const auto& params = std::get<RectangleClipParameters>(parameters);
				return Create(params.rect);
			}
			else if (std::holds_alternative<RoundedRectangleClipParameters>(parameters))
			{
				const auto& params = std::get<RoundedRectangleClipParameters>(parameters);
				return Create(params.rect);
			}
			else if (std::holds_alternative<RoundCornerClipParameters>(parameters))
			{
				const auto& params = std::get<RoundCornerClipParameters>(parameters);
				return Create(
					params.rect, 
					{ params.topLeftRadius, params.topLeftRadius },
					{ params.topRightRadius, params.topRightRadius },
					{ params.bottomLeftRadius, params.bottomLeftRadius },
					{ params.bottomRightRadius, params.bottomRightRadius }
				);
			}

			return Unexpected{ Error{ ErrorCode::InvalidArgument } };
		}

		[[nodiscard]] auto GetRect() const noexcept -> Result<RectF>
		{
			std::array results{
				GetLeft(),
				GetTop(),
				GetRight(),
				GetBottom()
			};

			if (std::ranges::any_of(results, [](const auto& res) { return !res.has_value(); }))
			{
				return Unexpected{
					std::ranges::find_if(results, [](const auto& res) { return !res.has_value(); })->error()
				};
			}

			return RectF{
				results.at(0).value(),
				results.at(1).value(),
				results.at(2).value(),
				results.at(3).value()
			};
		}
		[[nodiscard]] auto SetRect(const RectF& rc) const noexcept -> Result<void>
		{
			if (auto res = SetLeft(rc.left);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Left") };
			}
			if (auto res = SetTop(rc.top);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Top") };
			}
			if (auto res = SetRight(rc.right);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Right") };
			}
			if (auto res = SetBottom(rc.bottom);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"side", L"Bottom") };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto SetLeftRadius(const Point2F& radius) const noexcept -> Result<void>
		{
			if (auto res = SetTopLeftRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"TopLeft") };
			}
			if (auto res = SetBottomLeftRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"BottomLeft") };
			}
			return EmptyResult;
		}
		[[nodiscard]] auto SetTopRadius(const Point2F& radius) const noexcept -> Result<void>
		{
			if (auto res = SetTopLeftRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"TopLeft") };
			}
			if (auto res = SetTopRightRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"TopRight") };
			}
			return EmptyResult;
		}
		[[nodiscard]] auto SetRightRadius(const Point2F& radius) const noexcept -> Result<void>
		{
			if (auto res = SetTopRightRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"TopRight") };
			}
			if (auto res = SetBottomRightRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"BottomRight") };
			}
			return EmptyResult;
		}
		[[nodiscard]] auto SetBottomRadius(const Point2F& radius) const noexcept -> Result<void>
		{
			if (auto res = SetBottomLeftRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"BottomLeft") };
			}
			if (auto res = SetBottomRightRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"BottomRight") };
			}
			return EmptyResult;
		}
		[[nodiscard]] auto SetRadius(const Point2F& radius) const noexcept -> Result<void>
		{
			if (auto res = SetTopLeftRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"TopLeft") };
			}
			if (auto res = SetTopRightRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"TopRight") };
			}
			if (auto res = SetBottomLeftRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"BottomLeft") };
			}
			if (auto res = SetBottomRightRadius(radius);
				!res.has_value())
			{
				return Unexpected{ res.error().AddDetail(L"corner", L"BottomRight") };
			}
			return EmptyResult;
		}

		[[nodiscard]] auto GetBottom() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Bottom();
			});
		}
		[[nodiscard]] auto SetBottom(const float bottom) const noexcept -> Result<void>
		{
			return CallProjected([this, bottom]
			{
				Get().Bottom(bottom);
			});
		}

		[[nodiscard]] auto GetBottomLeftRadius() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().BottomLeftRadius() };
			});
		}
		[[nodiscard]] auto SetBottomLeftRadius(const Point2F& bottomLeftRadius) const noexcept -> Result<void>
		{
			return CallProjected([this, &bottomLeftRadius]
			{
				Get().BottomLeftRadius(bottomLeftRadius);
			});
		}

		[[nodiscard]] auto GetBottomRightRadius() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().BottomRightRadius() };
			});
		}
		[[nodiscard]] auto SetBottomRightRadius(const Point2F& bottomRightRadius) const noexcept -> Result<void>
		{
			return CallProjected([this, &bottomRightRadius]
			{
				Get().BottomRightRadius(bottomRightRadius);
			});
		}

		[[nodiscard]] auto GetLeft() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Left();
			});
		}
		[[nodiscard]] auto SetLeft(const float left) const noexcept -> Result<void>
		{
			return CallProjected([this, left]
			{
				Get().Left(left);
			});
		}

		[[nodiscard]] auto GetRight() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Right();
			});
		}
		[[nodiscard]] auto SetRight(const float right) const noexcept -> Result<void>
		{
			return CallProjected([this, right]
			{
				Get().Right(right);
			});
		}

		[[nodiscard]] auto GetTop() const noexcept -> Result<float>
		{
			return CallProjected([this]
			{
				return Get().Top();
			});
		}
		[[nodiscard]] auto SetTop(const float top) const noexcept -> Result<void>
		{
			return CallProjected([this, top]
			{
				Get().Top(top);
			});
		}

		[[nodiscard]] auto GetTopLeftRadius() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().TopLeftRadius() };
			});
		}
		[[nodiscard]] auto SetTopLeftRadius(const Point2F& topLeftRadius) const noexcept -> Result<void>
		{
			return CallProjected([this, &topLeftRadius]
			{
				Get().TopLeftRadius(topLeftRadius);
			});
		}

		[[nodiscard]] auto GetTopRightRadius() const noexcept -> Result<Point2F>
		{
			return CallProjected([this]
			{
				return Point2F{ Get().TopRightRadius() };
			});
		}
		[[nodiscard]] auto SetTopRightRadius(const Point2F& topRightRadius) const noexcept -> Result<void>
		{
			return CallProjected([this, &topRightRadius]
			{
				Get().TopRightRadius(topRightRadius);
			});
		}
	};
	using RoundedRectangleClip = RectangleClip;
}

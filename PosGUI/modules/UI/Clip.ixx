module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.Clip;

import std;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.D2DPathGeometry;

export namespace PGUI::UI
{
	struct RectangleClip : ComPtrHolder<ID2D1RectangleGeometry>
	{
		explicit RectangleClip(const ComPtr<ID2D1RectangleGeometry>& geometry) noexcept;

		explicit RectangleClip(RectF rect) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct RoundedRectangleClip : ComPtrHolder<ID2D1RoundedRectangleGeometry>
	{
		explicit RoundedRectangleClip(const ComPtr<ID2D1RoundedRectangleGeometry>& geometry) noexcept;

		explicit RoundedRectangleClip(const RoundedRect& rect) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct EllipseClip : ComPtrHolder<ID2D1EllipseGeometry>
	{
		explicit EllipseClip(const ComPtr<ID2D1EllipseGeometry>& geometry) noexcept;

		explicit EllipseClip(Ellipse ellipse) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct PathClip : ComPtrHolder<ID2D1PathGeometry1>
	{
		explicit PathClip(const ComPtr<ID2D1PathGeometry1>& geometry) noexcept;

		explicit PathClip(const D2D::D2DPathGeometry& geometry) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct RoundCornerClip : PathClip
	{
		explicit RoundCornerClip(const ComPtr<ID2D1PathGeometry1>& geometry) noexcept;

		explicit RoundCornerClip(const D2D::D2DPathGeometry& geometry) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct RectangleClipParameters
	{
		RectF rect;

		explicit(false) constexpr RectangleClipParameters(const RectF rect) noexcept :
			rect{ rect }
		{
		}
	};

	struct RoundedRectangleClipParameters
	{
		RoundedRect rect;

		explicit(false) constexpr RoundedRectangleClipParameters(const RoundedRect& rect) noexcept :
			rect{ rect }
		{
		}
	};

	struct EllipseClipParameters
	{
		Ellipse ellipse;

		explicit(false) constexpr EllipseClipParameters(const Ellipse ellipse) noexcept :
			ellipse{ ellipse }
		{
		}
	};

	struct PathClipParameters
	{
		D2D::D2DPathGeometry path;

		explicit(false) PathClipParameters(const D2D::D2DPathGeometry& path) noexcept :
			path{ path }
		{
		}
	};

	struct RoundCornerClipParameters
	{
		RectF rect;
		float topLeftRadius;
		float topRightRadius;
		float bottomLeftRadius;
		float bottomRightRadius;

		RoundCornerClipParameters(
			const RectF rect, const float topLeftRadius,
			const float topRightRadius, const float bottomLeftRadius,
			const float bottomRightRadius) noexcept :
			rect{ rect }, topLeftRadius{ topLeftRadius },
			topRightRadius{ topRightRadius },
			bottomLeftRadius{ bottomLeftRadius },
			bottomRightRadius{ bottomRightRadius }
		{
		}
	};

	using EmptyClipParameters = std::monostate;
	using EmptyClip = std::monostate;

	using ClipParameters = std::variant<EmptyClipParameters,
		RectangleClipParameters, RoundedRectangleClipParameters,
		EllipseClipParameters, PathClipParameters, RoundCornerClipParameters>;

	class Clip final
	{
		using ClipVariant = std::variant<
			EmptyClip,
			RectangleClip, RoundedRectangleClip,
			EllipseClip, PathClip, RoundCornerClip>;

		public:
		explicit Clip() noexcept = default;

		explicit Clip(ClipParameters parameters) noexcept;

		auto CreateClip() -> void;

		auto ReleaseClip() noexcept -> void;

		auto Clear() noexcept -> void;

		[[nodiscard]] const auto& GetParameters() const noexcept { return parameters; }
		[[nodiscard]] auto& GetParameters() noexcept { return parameters; }

		auto SetParameters(const ClipParameters& parameters) noexcept -> void;

		[[nodiscard]] auto GetGeometry() const noexcept -> D2D::D2DGeometry<>;

		explicit(false) operator ID2D1Geometry*() const noexcept;

		explicit(false) operator D2D::D2DGeometry<>() const noexcept { return GetGeometry(); }
		explicit(false) operator bool() const noexcept { return !std::holds_alternative<EmptyClip>(clip); }

		private:
		ClipVariant clip;
		ClipParameters parameters = EmptyClipParameters{ };
	};
}

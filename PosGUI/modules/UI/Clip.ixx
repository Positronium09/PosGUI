module;
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.UI.Clip;

import std;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.D2DPathGeometry;

namespace PGUI
{
	class Window;
	template <typename T>
	using WindowPtr = std::shared_ptr<T>;
}

export namespace PGUI::UI
{
	struct RectangleClip : public ComPtrHolder<ID2D1RectangleGeometry>
	{
		explicit RectangleClip(ComPtr<ID2D1RectangleGeometry> geometry) noexcept;

		explicit RectangleClip(RectF rect) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct RoundedRectangleClip : public ComPtrHolder<ID2D1RoundedRectangleGeometry>
	{
		explicit RoundedRectangleClip(ComPtr<ID2D1RoundedRectangleGeometry> geometry) noexcept;

		explicit RoundedRectangleClip(RoundedRect rect) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct EllipseClip : public ComPtrHolder<ID2D1EllipseGeometry>
	{
		explicit EllipseClip(ComPtr<ID2D1EllipseGeometry> geometry) noexcept;

		explicit EllipseClip(Ellipse ellipse) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct PathClip : public ComPtrHolder<ID2D1PathGeometry1>
	{
		explicit PathClip(ComPtr<ID2D1PathGeometry1> geometry) noexcept;

		explicit PathClip(const D2D::D2DPathGeometry& geometry) noexcept;

		explicit(false) operator ID2D1Geometry*() const noexcept { return Get().Get(); }
	};

	struct RoundCornerClip : public PathClip
	{
		explicit RoundCornerClip(ComPtr<ID2D1PathGeometry1> geometry) noexcept;

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

		auto SetParameters(ClipParameters parameters) noexcept -> void;

		[[nodiscard]] auto GetGeometry() const noexcept -> D2D::D2DGeometry<ID2D1Geometry>;

		explicit(false) operator ID2D1Geometry*() const noexcept;

		explicit(false) operator D2D::D2DGeometry<>() const noexcept { return GetGeometry(); }
		explicit(false) operator bool() const noexcept { return !std::holds_alternative<EmptyClip>(clip); }

		private:
		ClipVariant clip;
		ClipParameters parameters = EmptyClipParameters{ };
	};
}

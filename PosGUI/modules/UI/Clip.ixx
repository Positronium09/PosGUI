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

		explicit(false) operator ID2D1Geometry* () const noexcept { return Get().Get(); }
	};

	struct EllipseClip : public ComPtrHolder<ID2D1EllipseGeometry>
	{
		explicit EllipseClip(ComPtr<ID2D1EllipseGeometry> geometry) noexcept;
		explicit EllipseClip(Ellipse ellipse) noexcept;

		explicit(false) operator ID2D1Geometry* () const noexcept { return Get().Get(); }
	};

	struct PathClip : public ComPtrHolder<ID2D1PathGeometry1>
	{
		explicit PathClip(ComPtr<ID2D1PathGeometry1> geometry) noexcept;
		explicit PathClip(const D2D::D2DPathGeometry& geometry) noexcept;
		
		explicit(false) operator ID2D1Geometry* () const noexcept { return Get().Get(); }
	};

	struct RoundCornerClip : public PathClip
	{
		explicit RoundCornerClip(ComPtr<ID2D1PathGeometry1> geometry) noexcept;
		explicit RoundCornerClip(const D2D::D2DPathGeometry& geometry) noexcept;

		explicit(false) operator ID2D1Geometry* () const noexcept { return Get().Get(); }
	};

	struct RectangleClipParameters
	{
		RectF rect;
		RectangleClipParameters(RectF rect) noexcept :
			rect{ rect }
		{
		}
	};

	struct RoundedRectangleClipParameters
	{
		RoundedRect rect;
		RoundedRectangleClipParameters(RoundedRect rect) noexcept :
			rect{ rect }
		{
		}
	};

	struct EllipseClipParameters
	{
		Ellipse ellipse;
		EllipseClipParameters(Ellipse ellipse) noexcept :
			ellipse{ ellipse }
		{
		}
	};

	struct PathClipParameters
	{
		D2D::D2DPathGeometry path;
		PathClipParameters(D2D::D2DPathGeometry path) noexcept :
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

		RoundCornerClipParameters(RectF rect, float topLeftRadius,
			float topRightRadius, float bottomLeftRadius,
			float bottomRightRadius) noexcept :
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
		using ClipVariant = std::variant<EmptyClip, 
			RectangleClip, RoundedRectangleClip, 
			EllipseClip, PathClip, RoundCornerClip>;

		public:
		explicit Clip() noexcept = default;
		explicit Clip(ClipParameters parameters) noexcept;

		void CreateClip();
		void ReleaseClip() noexcept;
		void Clear() noexcept;

		[[nodiscard]] const auto& GetParameters() const noexcept { return parameters; }
		[[nodiscard]] auto& GetParameters() noexcept { return parameters; }
		void SetParameters(ClipParameters parameters) noexcept;

		[[nodiscard]] auto GetGeometry() const noexcept -> D2D::D2DGeometry<ID2D1Geometry>;

		explicit(false) operator ID2D1Geometry*() const noexcept;
		explicit(false) operator D2D::D2DGeometry<>() const noexcept { return GetGeometry(); }
		explicit(false) operator bool() const noexcept { return !std::holds_alternative<EmptyClip>(clip); }

		private:
		ClipVariant clip;
		ClipParameters parameters = EmptyClipParameters{ };
	};
}

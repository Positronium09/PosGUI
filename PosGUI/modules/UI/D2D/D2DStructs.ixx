module;
#include <d2d1_1.h>

export module PGUI.UI.D2D.D2DStructs;

import std;

import PGUI.Shape2D;
import PGUI.UI.D2D.D2DEnums;

export namespace PGUI::UI::D2D
{
	using Matrix3x2 = D2D1::Matrix3x2F;

	[[nodiscard]] auto D2D1MatrixToMatrix3x2(const D2D1_MATRIX_3X2_F& matrix) noexcept
	{
		return Matrix3x2{
			matrix.m[0][0],
			matrix.m[0][1],
			matrix.m[1][0],
			matrix.m[1][1],
			matrix.m[2][0],
			matrix.m[2][1]
		};
	}

	class MappedRect
	{
		public:
		constexpr MappedRect(const D2D1_MAPPED_RECT mappedRect, const UINT32 height) noexcept :
			pitch{ mappedRect.pitch },
			bytes{ mappedRect.bits, height * mappedRect.pitch }
		{ }

		[[nodiscard]] constexpr auto Pitch() const noexcept { return pitch; }
		[[nodiscard]] constexpr auto Bytes() const noexcept { return bytes; }
		[[nodiscard]] constexpr auto operator[](size_t index) const noexcept { return bytes[index]; }
		[[nodiscard]] constexpr auto begin() const noexcept { return bytes.begin(); }
		[[nodiscard]] constexpr auto end() const noexcept { return bytes.end(); }
		[[nodiscard]] constexpr auto cbegin() const noexcept { return bytes.cbegin(); }
		[[nodiscard]] constexpr auto cend() const noexcept { return bytes.cend(); }
		[[nodiscard]] constexpr auto rbegin() const noexcept { return bytes.rbegin(); }
		[[nodiscard]] constexpr auto rend() const noexcept { return bytes.rend(); }
		[[nodiscard]] constexpr auto crbegin() const noexcept { return bytes.crbegin(); }
		[[nodiscard]] constexpr auto crend() const noexcept { return bytes.crend(); }

		private:
		UINT32 pitch;
		std::span<BYTE> bytes;
	};

	struct BitmapProperties : D2D1_BITMAP_PROPERTIES1
	{
		constexpr BitmapProperties() noexcept = default;

		constexpr BitmapProperties(const D2D1_PIXEL_FORMAT pixelFormat, const std::pair<float, float> dpi,
		                           BitmapOptions bitmapOptions = BitmapOptions::None) noexcept :
			D2D1_BITMAP_PROPERTIES1{
				.pixelFormat = pixelFormat,
				.dpiX = dpi.first,
				.dpiY = dpi.second,
				.bitmapOptions = static_cast<D2D1_BITMAP_OPTIONS>(bitmapOptions)
			}
		{ }

		explicit(false) constexpr BitmapProperties(const D2D1_BITMAP_PROPERTIES1& bitmapProperties) noexcept :
			D2D1_BITMAP_PROPERTIES1{ bitmapProperties }
		{ }

		explicit(false) constexpr BitmapProperties(const D2D1_BITMAP_PROPERTIES& bitmapProperties) noexcept :
			D2D1_BITMAP_PROPERTIES1{
				.pixelFormat = bitmapProperties.pixelFormat,
				.dpiX = bitmapProperties.dpiX,
				.dpiY = bitmapProperties.dpiY,
				.bitmapOptions = D2D1_BITMAP_OPTIONS_NONE
			}
		{ }

		explicit(false) constexpr operator D2D1_BITMAP_PROPERTIES() const noexcept
		{
			return D2D1_BITMAP_PROPERTIES{ .pixelFormat = pixelFormat, .dpiX = dpiX, .dpiY = dpiY };
		}

		explicit(false) constexpr operator D2D1_BITMAP_PROPERTIES1() const noexcept
		{
			return D2D1_BITMAP_PROPERTIES1{
				.pixelFormat = pixelFormat,
				.dpiX = dpiX,
				.dpiY = dpiY,
				.bitmapOptions = bitmapOptions
			};
		}

		[[nodiscard]] constexpr auto GetPixelFormat() const noexcept { return pixelFormat; }
		[[nodiscard]] constexpr auto GetDpi() const noexcept { return std::pair{ dpiX, dpiY }; }

		[[nodiscard]] constexpr auto GetBitmapOptions() const noexcept
		{
			return static_cast<BitmapOptions>(bitmapOptions);
		}
	};

	struct BitmapBrushProperties : D2D1_BITMAP_BRUSH_PROPERTIES
	{
		constexpr BitmapBrushProperties() noexcept :
			D2D1_BITMAP_BRUSH_PROPERTIES{
				.extendModeX = D2D1_EXTEND_MODE_CLAMP,
				.extendModeY = D2D1_EXTEND_MODE_CLAMP,
				.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
			}
		{ }

		constexpr BitmapBrushProperties(ExtendMode extendModeX, ExtendMode extendModeY,
		                                BitmapInterpolationMode interpolationMode) noexcept :
			D2D1_BITMAP_BRUSH_PROPERTIES{
				.extendModeX = static_cast<D2D1_EXTEND_MODE>(extendModeX),
				.extendModeY = static_cast<D2D1_EXTEND_MODE>(extendModeY),
				.interpolationMode = static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationMode)
			}
		{ }

		explicit(false) constexpr operator D2D1_BITMAP_BRUSH_PROPERTIES() const noexcept
		{
			return D2D1_BITMAP_BRUSH_PROPERTIES{
				.extendModeX = extendModeX,
				.extendModeY = extendModeY,
				.interpolationMode = interpolationMode
			};
		}
	};

	struct BezierSegment : D2D1_BEZIER_SEGMENT
	{
		constexpr BezierSegment() noexcept = default;

		constexpr BezierSegment(const PointF point1, const PointF point2, const PointF point3) noexcept :
			D2D1_BEZIER_SEGMENT{ .point1 = point1, .point2 = point2, .point3 = point3 }
		{ }

		explicit(false) constexpr operator D2D1_BEZIER_SEGMENT() const noexcept
		{
			return D2D1_BEZIER_SEGMENT{ .point1 = point1, .point2 = point2, .point3 = point3 };
		}

		[[nodiscard]] constexpr auto GetPoint1() const noexcept -> PointF { return point1; }
		[[nodiscard]] constexpr auto GetPoint2() const noexcept -> PointF { return point2; }
		[[nodiscard]] constexpr auto GetPoint3() const noexcept -> PointF { return point3; }
	};

	struct QuadraticBezierSegment : D2D1_QUADRATIC_BEZIER_SEGMENT
	{
		constexpr QuadraticBezierSegment() noexcept = default;

		constexpr QuadraticBezierSegment(PointF point1, PointF point2) noexcept :
			D2D1_QUADRATIC_BEZIER_SEGMENT{ .point1 = point1, .point2 = point2 }
		{ }

		explicit(false) constexpr operator D2D1_QUADRATIC_BEZIER_SEGMENT() const noexcept
		{
			return D2D1_QUADRATIC_BEZIER_SEGMENT{ .point1 = point1, .point2 = point2 };
		}

		[[nodiscard]] constexpr auto GetPoint1() const noexcept -> PointF { return point1; }
		[[nodiscard]] constexpr auto GetPoint2() const noexcept -> PointF { return point2; }
	};

	struct ArcSegment : D2D1_ARC_SEGMENT
	{
		constexpr ArcSegment() noexcept = default;

		ArcSegment(const PointF point, const SizeF size, const float rotationAngle,
		           ArcSize arcSize, SweepDirection sweepDirection) noexcept :
			D2D1_ARC_SEGMENT{
				.point = point,
				.size = size,
				.rotationAngle = rotationAngle,
				.sweepDirection = static_cast<D2D1_SWEEP_DIRECTION>(sweepDirection),
				.arcSize = static_cast<D2D1_ARC_SIZE>(arcSize)
			}
		{ }

		explicit(false) constexpr operator D2D1_ARC_SEGMENT() const noexcept
		{
			return D2D1_ARC_SEGMENT{
				.point = point,
				.size = size,
				.rotationAngle = rotationAngle,
				.sweepDirection = sweepDirection,
				.arcSize = arcSize
			};
		}

		[[nodiscard]] constexpr auto GetPoint() const noexcept -> PointF { return point; }
		[[nodiscard]] constexpr auto GetSize() const noexcept -> SizeF { return size; }
		[[nodiscard]] constexpr auto GetRotationAngle() const noexcept { return rotationAngle; }

		[[nodiscard]] constexpr auto GetSweepDirection() const noexcept
		{
			return static_cast<SweepDirection>(sweepDirection);
		}

		[[nodiscard]] constexpr auto GetArcSize() const noexcept { return static_cast<ArcSize>(arcSize); }
	};
}

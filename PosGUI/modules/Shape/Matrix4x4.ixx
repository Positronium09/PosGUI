module;
#include <d2d1_1.h>
#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.Shape:Matrix4x4;

import std;

import :Point2;
import :Point3;
import :Size;

namespace WFN = winrt::Windows::Foundation::Numerics;

namespace
{
	[[nodiscard]] constexpr auto Abs(const float x) noexcept
	{
		if (x == 0)
		{
			return 0.0F;
		}
		return std::bit_cast<float>(std::bit_cast<std::int32_t>(x) & ~(1 << 31));
	}
}

export namespace PGUI
{
	// ReSharper disable once CppInconsistentNaming

	struct Matrix4x4
	{
		float m11{ 1.0F }, m12{ 0.0F }, m13{ 0.0F }, m14{ 0.0F };
		float m21{ 0.0F }, m22{ 1.0F }, m23{ 0.0F }, m24{ 0.0F };
		float m31{ 0.0F }, m32{ 0.0F }, m33{ 1.0F }, m34{ 0.0F };
		float m41{ 0.0F }, m42{ 0.0F }, m43{ 0.0F }, m44{ 1.0F };

		[[nodiscard]] static constexpr auto Product(const Matrix4x4& a, const Matrix4x4& b) noexcept -> Matrix4x4
		{
			return Matrix4x4{
				a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41,
				a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42,
				a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43,
				a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44,
				a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41,
				a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42,
				a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43,
				a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44,
				a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41,
				a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42,
				a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43,
				a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44,
				a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41,
				a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42,
				a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43,
				a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44
			};
		}

		[[nodiscard]] static constexpr auto Identity() noexcept -> Matrix4x4
		{
			return Matrix4x4{ };
		}

		[[nodiscard]] static constexpr auto Translation(const Point3F& translation) noexcept -> Matrix4x4
		{
			return Matrix4x4{
				1.0F, 0.0F, 0.0F, 0.0F,
				0.0F, 1.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				translation.x, translation.y, translation.z, 1.0F
			};
		}
		[[nodiscard]] static constexpr auto Translation(const float x, const float y, const float z) noexcept -> Matrix4x4
		{
			return Translation(Point3F{ x, y, z });
		}

		[[nodiscard]] static constexpr auto Scale(
			const float cx, const float cy, const float cz,
			const Point3F& center = { }) noexcept -> Matrix4x4
		{
			return Matrix4x4{
				cx, 0.0F, 0.0F, 0.0F,
				0.0F, cy, 0.0F, 0.0F,
				0.0F, 0.0F, cz, 0.0F,
				center.x - cx * center.x,
				center.y - cy * center.y,
				center.z - cz * center.z,
				1.0F
			};
		}
		[[nodiscard]] static constexpr auto Scale(const SizeF& scale, const Point3F& center = { }) noexcept -> Matrix4x4
		{
			return Scale(scale.cx, scale.cy, 1.0F, center);
		}

		[[nodiscard]] static auto RotateX(const float angle, const Point3F& center = { }) noexcept -> Matrix4x4
		{
			return std::bit_cast<Matrix4x4>(WFN::make_float4x4_rotation_x(angle, center));
		}
		[[nodiscard]] static auto RotateY(const float angle, const Point3F& center = { }) noexcept -> Matrix4x4
		{
			return std::bit_cast<Matrix4x4>(WFN::make_float4x4_rotation_y(angle, center));
		}
		[[nodiscard]] static auto RotateZ(const float angle, const Point3F& center = { }) noexcept -> Matrix4x4
		{
			return std::bit_cast<Matrix4x4>(WFN::make_float4x4_rotation_z(angle, center));
		}
		[[nodiscard]] static auto RotateYPR(
			const float yaw, const float pitch, const float roll) noexcept -> Matrix4x4
		{
			return std::bit_cast<Matrix4x4>(WFN::make_float4x4_from_yaw_pitch_roll(yaw, pitch, roll));
		}
		[[nodiscard]] static auto RotateAroundAxis(
			const Point3F& axis, const float angle) noexcept -> Matrix4x4
		{
			return std::bit_cast<Matrix4x4>(WFN::make_float4x4_from_axis_angle(axis, angle));
		}

		[[nodiscard]] static auto Shear(
			const float xyAngle, const float yxAngle,
			const float xzAngle, const float zxAngle,
			const float yzAngle, const float zyAngle) noexcept -> Matrix4x4
		{
			const auto xy = std::tan(xyAngle);
			const auto yx = std::tan(yxAngle);
			const auto xz = std::tan(xzAngle);
			const auto zx = std::tan(zxAngle);
			const auto yz = std::tan(yzAngle);
			const auto zy = std::tan(zyAngle);

			return Matrix4x4{
				1.0F, xy, xz, 0.0F,
				yx, 1.0F, yz, 0.0F,
				zx, zy, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F
			};
		}
		[[nodiscard]] static auto Shear(const float xAngle, const float yAngle) noexcept -> Matrix4x4
		{
			return Shear(xAngle, yAngle, 0.0F, 0.0F, 0.0F, 0.0F);
		}
		[[nodiscard]] static auto Skew(
			const float xyAngle, const float yxAngle,
			const float xzAngle, const float zxAngle,
			const float yzAngle, const float zyAngle) noexcept -> Matrix4x4
		{
			return Shear(xyAngle, yxAngle, xzAngle, zxAngle, yzAngle, zyAngle);
		}
		[[nodiscard]] static auto Skew(const float xAngle, const float yAngle) noexcept -> Matrix4x4
		{
			return Shear(xAngle, yAngle);
		}

		constexpr Matrix4x4() noexcept = default;
		constexpr Matrix4x4(
			const float m11, const float m12, const float m13, const float m14,
			const float m21, const float m22, const float m23, const float m24,
			const float m31, const float m32, const float m33, const float m34,
			const float m41, const float m42, const float m43, const float m44) noexcept :
			m11{ m11 }, m12{ m12 }, m13{ m13 }, m14{ m14 },
			m21{ m21 }, m22{ m22 }, m23{ m23 }, m24{ m24 },
			m31{ m31 }, m32{ m32 }, m33{ m33 }, m34{ m34 },
			m41{ m41 }, m42{ m42 }, m43{ m43 }, m44{ m44 }
		{ }

		explicit(false) constexpr Matrix4x4(const WFN::float4x4& matrix) noexcept :
			m11{ matrix.m11 }, m12{ matrix.m12 }, m13{ matrix.m13 }, m14{ matrix.m14 },
			m21{ matrix.m21 }, m22{ matrix.m22 }, m23{ matrix.m23 }, m24{ matrix.m24 },
			m31{ matrix.m31 }, m32{ matrix.m32 }, m33{ matrix.m33 }, m34{ matrix.m34 },
			m41{ matrix.m41 }, m42{ matrix.m42 }, m43{ matrix.m43 }, m44{ matrix.m44 }
		{ }

		explicit(false) constexpr Matrix4x4(const D2D1_MATRIX_4X4_F& matrix) noexcept : 
			m11{ matrix._11 }, m12{ matrix._12 }, m13{ matrix._13 }, m14{ matrix._14 },
			m21{ matrix._21 }, m22{ matrix._22 }, m23{ matrix._23 }, m24{ matrix._24 },
			m31{ matrix._31 }, m32{ matrix._32 }, m33{ matrix._33 }, m34{ matrix._34 },
			m41{ matrix._41 }, m42{ matrix._42 }, m43{ matrix._43 }, m44{ matrix._44 }
		{ }

		explicit(false) constexpr Matrix4x4(const D2D1::Matrix4x4F& matrix) noexcept :
			m11{ matrix._11 }, m12{ matrix._12 }, m13{ matrix._13 }, m14{ matrix._14 },
			m21{ matrix._21 }, m22{ matrix._22 }, m23{ matrix._23 }, m24{ matrix._24 },
			m31{ matrix._31 }, m32{ matrix._32 }, m33{ matrix._33 }, m34{ matrix._34 },
			m41{ matrix._41 }, m42{ matrix._42 }, m43{ matrix._43 }, m44{ matrix._44 }
		{ }

		[[nodiscard]] constexpr auto Product(const Matrix4x4& other) const noexcept
		{
			return Product(*this, other);
		}
		[[nodiscard]] constexpr auto Determinant() const noexcept
		{
			const auto s0 = m11 * m22 - m12 * m21;
			const auto s1 = m11 * m23 - m13 * m21;
			const auto s2 = m11 * m24 - m14 * m21;
			const auto s3 = m12 * m23 - m13 * m22;
			const auto s4 = m12 * m24 - m14 * m22;
			const auto s5 = m13 * m24 - m14 * m23;

			const auto c5 = m33 * m44 - m34 * m43;
			const auto c4 = m32 * m44 - m34 * m42;
			const auto c3 = m32 * m43 - m33 * m42;
			const auto c2 = m31 * m44 - m34 * m41;
			const auto c1 = m31 * m43 - m33 * m41;
			const auto c0 = m31 * m42 - m32 * m41;

			return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
		}
		[[nodiscard]] constexpr auto Det() const noexcept
		{
			return Determinant();
		}
		[[nodiscard]] constexpr auto IsInvertible() const noexcept
		{
			return Abs(Determinant()) >= std::numeric_limits<float>::epsilon();
		}
		[[nodiscard]] constexpr auto IsIdentity() const noexcept
		{
			return
				m11 == 1.0F && m12 == 0.0F && m13 == 0.0F && m14 == 0.0F &&
				m21 == 0.0F && m22 == 1.0F && m23 == 0.0F && m24 == 0.0F &&
				m31 == 0.0F && m32 == 0.0F && m33 == 1.0F && m34 == 0.0F &&
				m41 == 0.0F && m42 == 0.0F && m43 == 0.0F && m44 == 1.0F;
		}
		[[nodiscard]] constexpr auto Transform(const Point3F& point) const noexcept
		{
			return Point3F{
				point.x * m11 + point.y * m21 + point.z * m31 + m41,
				point.x * m12 + point.y * m22 + point.z * m32 + m42,
				point.x * m13 + point.y * m23 + point.z * m33 + m43
			};
		}
		[[nodiscard]] constexpr auto Invert() noexcept -> bool
		{
			const auto prev = *this;

			const auto s0 = prev.m11 * prev.m22 - prev.m12 * prev.m21;
			const auto s1 = prev.m11 * prev.m23 - prev.m13 * prev.m21;
			const auto s2 = prev.m11 * prev.m24 - prev.m14 * prev.m21;
			const auto s3 = prev.m12 * prev.m23 - prev.m13 * prev.m22;
			const auto s4 = prev.m12 * prev.m24 - prev.m14 * prev.m22;
			const auto s5 = prev.m13 * prev.m24 - prev.m14 * prev.m23;

			const auto c5 = prev.m33 * prev.m44 - prev.m34 * prev.m43;
			const auto c4 = prev.m32 * prev.m44 - prev.m34 * prev.m42;
			const auto c3 = prev.m32 * prev.m43 - prev.m33 * prev.m42;
			const auto c2 = prev.m31 * prev.m44 - prev.m34 * prev.m41;
			const auto c1 = prev.m31 * prev.m43 - prev.m33 * prev.m41;
			const auto c0 = prev.m31 * prev.m42 - prev.m32 * prev.m41;

			const auto det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

			if (Abs(det) < std::numeric_limits<float>::epsilon())
			{
				return false;
			}

			const auto invDet = 1 / det;

			m11 = ( prev.m22 * c5 - prev.m23 * c4 + prev.m24 * c3) * invDet;
			m12 = (-prev.m12 * c5 + prev.m13 * c4 - prev.m14 * c3) * invDet;
			m13 = ( prev.m42 * s5 - prev.m43 * s4 + prev.m44 * s3) * invDet;
			m14 = (-prev.m32 * s5 + prev.m33 * s4 - prev.m34 * s3) * invDet;

			m21 = (-prev.m21 * c5 + prev.m23 * c2 - prev.m24 * c1) * invDet;
			m22 = ( prev.m11 * c5 - prev.m13 * c2 + prev.m14 * c1) * invDet;
			m23 = (-prev.m41 * s5 + prev.m43 * s2 - prev.m44 * s1) * invDet;
			m24 = ( prev.m31 * s5 - prev.m33 * s2 + prev.m34 * s1) * invDet;

			m31 = ( prev.m21 * c4 - prev.m22 * c2 + prev.m24 * c0) * invDet;
			m32 = (-prev.m11 * c4 + prev.m12 * c2 - prev.m14 * c0) * invDet;
			m33 = ( prev.m41 * s4 - prev.m42 * s2 + prev.m44 * s0) * invDet;
			m34 = (-prev.m31 * s4 + prev.m32 * s2 - prev.m34 * s0) * invDet;

			m41 = (-prev.m21 * c3 + prev.m22 * c1 - prev.m23 * c0) * invDet;
			m42 = ( prev.m11 * c3 - prev.m12 * c1 + prev.m13 * c0) * invDet;
			m43 = (-prev.m41 * s3 + prev.m42 * s1 - prev.m43 * s0) * invDet;
			m44 = ( prev.m31 * s3 - prev.m32 * s1 + prev.m33 * s0) * invDet;

			return true;
		}
		[[nodiscard]] constexpr auto Inverted() const noexcept -> std::optional<Matrix4x4>
		{
			auto matrix = *this;

			if (!matrix.Invert())
			{
				return std::nullopt;
			}

			return matrix;
		}
		[[nodiscard]] constexpr auto Inverse() const noexcept -> std::optional<Matrix4x4>
		{
			return Inverted();
		}

		[[nodiscard]] constexpr auto Transpose() noexcept
		{
			std::swap(m12, m21);
			std::swap(m13, m31);
			std::swap(m14, m41);
			std::swap(m23, m32);
			std::swap(m24, m42);
			std::swap(m34, m43);
		}
		[[nodiscard]] constexpr auto Transposed() const noexcept -> Matrix4x4
		{
			auto matrix = *this;
			matrix.Transpose();
		
			return matrix;
		}

		[[nodiscard]] constexpr auto At(const std::size_t x, const std::size_t y) const noexcept -> std::optional<float>
		{
			if (x > 3 || y > 3)
			{
				return std::nullopt;
			}

			return reinterpret_cast<const float(&)[16]>(*this)[x * 4 + y];
		}

		explicit(false) constexpr operator WFN::float4x4() const noexcept
		{
			return WFN::float4x4{
				m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44
			};
		}

		explicit(false) constexpr operator D2D1_MATRIX_4X4_F() const noexcept
		{
			return D2D1_MATRIX_4X4_F{
				._11 = m11, ._12 = m12, ._13 = m13, ._14 = m14,
				._21 = m21, ._22 = m22, ._23 = m23, ._24 = m24,
				._31 = m31, ._32 = m32, ._33 = m33, ._34 = m34,
				._41 = m41, ._42 = m42, ._43 = m43, ._44 = m44
			};
		}

		explicit(false) constexpr operator D2D1::Matrix4x4F() const noexcept
		{
			return std::bit_cast<D2D1::Matrix4x4F>(*this);
		}

		[[nodiscard]] constexpr auto operator[](const std::size_t x, const std::size_t y) const -> float
		{
			const auto result = At(x, y);
			if (!result)
			{
				throw std::invalid_argument{ "Matrix4x4 index out of range" };
			}

			return *result;
		}

		[[nodiscard]] friend constexpr auto operator*(const Matrix4x4& a, const Matrix4x4& b) noexcept -> Matrix4x4
		{
			return Product(a, b);
		}
		constexpr auto operator*=(const Matrix4x4& other) noexcept -> Matrix4x4&
		{
			*this = Product(*this, other);

			return *this;
		}

		[[nodiscard]] constexpr auto operator*(const float factor) const noexcept -> Matrix4x4
		{
			return Matrix4x4{
				m11 * factor, m12 * factor, m13 * factor, m14 * factor,
				m21 * factor, m22 * factor, m23 * factor, m24 * factor,
				m31 * factor, m32 * factor, m33 * factor, m34 * factor,
				m41 * factor, m42 * factor, m43 * factor, m44 * factor
			};
		}
		constexpr auto operator*=(const float factor) noexcept -> Matrix4x4&
		{
			m11 *= factor; m12 *= factor; m13 *= factor; m14 *= factor;
			m21 *= factor; m22 *= factor; m23 *= factor; m24 *= factor;
			m31 *= factor; m32 *= factor; m33 *= factor; m34 *= factor;
			m41 *= factor; m42 *= factor; m43 *= factor; m44 *= factor;
			return *this;
		}
		[[nodiscard]] friend constexpr auto operator*(const float factor, const Matrix4x4& matrix) noexcept
		{
			return matrix * factor;
		}

		[[nodiscard]] constexpr auto operator*(const Point3F& point) const noexcept -> Point3F
		{
			return Transform(point);
		}
		[[nodiscard]] friend constexpr auto operator*(const Point3F& point, const Matrix4x4& matrix) noexcept
		{
			return matrix * point;
		}

		[[nodiscard]] constexpr auto operator/(const float factor) const noexcept -> Matrix4x4
		{
			return Matrix4x4{
				m11 / factor, m12 / factor, m13 / factor, m14 / factor,
				m21 / factor, m22 / factor, m23 / factor, m24 / factor,
				m31 / factor, m32 / factor, m33 / factor, m34 / factor,
				m41 / factor, m42 / factor, m43 / factor, m44 / factor
			};
		}
		constexpr auto operator/=(const float factor) noexcept -> Matrix4x4&
		{
			m11 /= factor; 
			m12 /= factor; 
			m13 /= factor; 
			m14 /= factor;

			m21 /= factor; 
			m22 /= factor; 
			m23 /= factor; 
			m24 /= factor;

			m31 /= factor; 
			m32 /= factor; 
			m33 /= factor; 
			m34 /= factor;

			m41 /= factor; 
			m42 /= factor; 
			m43 /= factor; 
			m44 /= factor;

			return *this;
		}

		[[nodiscard]] constexpr auto operator==(const Matrix4x4& other) const noexcept -> bool = default;
	};
}


template <typename CharT>
struct std::formatter<PGUI::Matrix4x4, CharT>
{
	template <typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
		auto iter = ctx.begin();
		const auto end = ctx.end();
		if (iter == end || *iter == '}')
		{
			return iter;
		}
		throw std::format_error{ "No formatting args supported for Matrix4x4" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Matrix4x4& matrix, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(),
			"[[{:.3f}, {:.3f}, {:.3f}, {:.3f}], [{:.3f}, {:.3f}, {:.3f}, {:.3f}], [{:.3f}, {:.3f}, {:.3f}, {:.3f}], [{:.3f}, {:.3f}, {:.3f}, {:.3f}]]",
			matrix.m11, matrix.m12, matrix.m13, matrix.m14,
			matrix.m21, matrix.m22, matrix.m23, matrix.m24,
			matrix.m31, matrix.m32, matrix.m33, matrix.m34,
			matrix.m41, matrix.m42, matrix.m43, matrix.m44);
	}
};

module;
#include <d2d1_1.h>

#include <winrt/Windows.Foundation.Numerics.h>

export module PGUI.Shape:Matrix3x2;

import std;

import :Point2;
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

	struct Matrix3x2 final
	{
		float m11{ 1.0F }, m12{ 0.0F };
		float m21{ 0.0F }, m22{ 1.0F };
		float m31{ 0.0F }, m32{ 0.0F };

		[[nodiscard]] static constexpr auto Product(const Matrix3x2& a, const Matrix3x2& b) noexcept -> Matrix3x2
		{
			return Matrix3x2{
				a.m11 * b.m11 + a.m12 * b.m21,
				a.m11 * b.m12 + a.m12 * b.m22,
				a.m21 * b.m11 + a.m22 * b.m21,
				a.m21 * b.m12 + a.m22 * b.m22,
				a.m31 * b.m11 + a.m32 * b.m21 + b.m31,
				a.m31 * b.m12 + a.m32 * b.m22 + b.m32
			};
		}

		[[nodiscard]] static constexpr auto Identity() noexcept -> Matrix3x2
		{
			return Matrix3x2{ };
		}

		[[nodiscard]] static constexpr auto Translation(const Point2F& translation) noexcept
		{
			return Matrix3x2{
				1.0F, 0.0F,
				0.0F, 1.0F,
				translation.x, translation.y
			};
		}
		[[nodiscard]] static constexpr auto Translation(const float dx, const float dy) noexcept
		{
			return Translation(Point2F{ dx, dy });
		}

		[[nodiscard]] static constexpr auto Scale(
			const SizeF& scale, 
			const Point2F center = { }) noexcept
		{
			return Matrix3x2{
				scale.cx, 0.0F,
				0.0F, scale.cy,
				center.x - scale.cx * center.x,
				center.y - scale.cy * center.y
			};
		}
		[[nodiscard]] static constexpr auto Scale(
			const float cx, const float cy, 
			const Point2F center = { }) noexcept
		{
			return Scale(SizeF{ cx, cy }, center);
		}

		[[nodiscard]] static auto Rotate(
			const float angle,
			const Point2F center = { }) noexcept
		{
			return std::bit_cast<Matrix3x2>(WFN::make_float3x2_rotation(angle, center));
		}

		[[nodiscard]] static auto Shear(
			const float xAngle, const float yAngle, 
			const Point2F center = { }) noexcept
		{
			return std::bit_cast<Matrix3x2>(WFN::make_float3x2_skew(xAngle, yAngle, center));
		}
		[[nodiscard]] static auto Skew(
			const float xAngle, const float yAngle,
			const Point2F center = { }) noexcept
		{
			return Shear(xAngle, yAngle, center);
		}

		constexpr Matrix3x2() noexcept = default;
		constexpr Matrix3x2(
			const float m11, const float m12,
			const float m21, const float m22,
			const float m31, const float m32) noexcept :
			m11{ m11 }, m12{ m12 }, 
			m21{ m21 }, m22{ m22 }, 
			m31{ m31 }, m32{ m32 }
		{ }

		explicit(false) constexpr Matrix3x2(const WFN::float3x2& matrix) noexcept : 
			m11{ matrix.m11 }, m12{ matrix.m12 },
			m21{ matrix.m21 }, m22{ matrix.m22 },
			m31{ matrix.m31 }, m32{ matrix.m32 }
		{ }

		explicit(false) constexpr Matrix3x2(const D2D1_MATRIX_3X2_F& matrix) noexcept :
			m11{ matrix._11 }, m12{ matrix._12 },
			m21{ matrix._21 }, m22{ matrix._22 },
			m31{ matrix._31 }, m32{ matrix._32 }
		{ }
		explicit(false) constexpr Matrix3x2(const D2D1::Matrix3x2F& matrix) noexcept :
			m11{ matrix._11 }, m12{ matrix._12 },
			m21{ matrix._21 }, m22{ matrix._22 },
			m31{ matrix._31 }, m32{ matrix._32 }
		{ }

		[[nodiscard]] constexpr auto Product(const Matrix3x2& b) const noexcept
		{
			return Product(*this, b);
		}
		[[nodiscard]] constexpr auto Determinant() const noexcept
		{
			return m11 * m22 - m12 * m21;
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
			return m11 == 1.0F && m22 == 1.0F && m12 == 0.0F && m21 == 0.0F && m31 == 0.0F && m32 == 0.0F;
		}
		[[nodiscard]] constexpr auto Transform(const Point2F& p) const noexcept
		{
			return Point2F{
				p.x* m11 + p.y * m21 + m31,
				p.x* m12 + p.y * m22 + m32
			};
		}
		[[nodiscard]] constexpr auto Invert() noexcept -> bool
		{
			const auto det = m11* m22 - m12 * m21;

			if (Abs(Determinant()) >= std::numeric_limits<float>::epsilon())
			{
				return false;
			}

			const auto prev = *this;
			const auto invDet = 1 / det;

			m11 = prev.m22 * invDet;
			m12 = -prev.m12 * invDet;
			m21 = -prev.m21 * invDet;
			m22 = prev.m11 * invDet;
			m31 = (prev.m32 * prev.m21 - prev.m31 * prev.m22) * invDet;
			m32 = (prev.m31 * prev.m12 - prev.m32 * prev.m11) * invDet;

			return true;
		}

		[[nodiscard]] constexpr auto Inverted() const noexcept -> std::optional<Matrix3x2>
		{
			if (!IsInvertible())
			{
				return std::nullopt;
			}
			
			auto matrix = *this;
			[[maybe_unused]] const auto result = matrix.Invert();

			return matrix;
		}
		[[nodiscard]] constexpr auto Inverse() const noexcept -> std::optional<Matrix3x2>
		{
			return Inverted();
		}

		auto Transpose() noexcept -> void
		{
			std::swap(m12, m21);
		}
		[[nodiscard]] constexpr auto Transposed() const noexcept -> Matrix3x2
		{
			return Matrix3x2{
				m11, m21,
				m12, m22,
				m31, m32
			};
		}

		[[nodiscard]] constexpr auto At(const std::size_t x, const std::size_t y) const noexcept -> std::optional<float>
		{
			if (x > 2 || y > 1)
			{
				return std::nullopt;
			}

			return reinterpret_cast<const float(&)[6]>(*this)[x * 2 + y];
		}

		[[nodiscard]] explicit(false) constexpr operator WFN::float3x2() const noexcept
		{
			return WFN::float3x2{
				m11, m12,
				m21, m22,
				m31, m32
			};
		}
		[[nodiscard]] explicit(false) constexpr operator D2D1_MATRIX_3X2_F() const noexcept
		{
			return D2D1_MATRIX_3X2_F{
				._11 = m11, ._12 = m12,
				._21 = m21, ._22 = m22,
				._31 = m31, ._32 = m32
			};
		}
		[[nodiscard]] explicit(false) constexpr operator D2D1::Matrix3x2F() const noexcept
		{
			return std::bit_cast<D2D1::Matrix3x2F>(*this);
		}

		[[nodiscard]] constexpr auto operator[](const std::size_t x, const std::size_t y) const -> float
		{
			const auto result = At(x, y);

			if (!result)
			{
				throw std::invalid_argument{ "Matrix3x2 index out of range" };
			}

			return *result;
		}

		[[nodiscard]] friend constexpr auto operator*(const Matrix3x2& a, const Matrix3x2& b) noexcept -> Matrix3x2
		{
			return Product(a, b);
		}
		constexpr auto operator*=(const Matrix3x2& other) noexcept -> Matrix3x2&
		{
			*this = Product(*this, other);
		
			return *this;
		}

		[[nodiscard]] constexpr auto operator*(const float factor) const noexcept -> Matrix3x2
		{
			return Matrix3x2{
				m11 * factor, m12 * factor,
				m21 * factor, m22 * factor,
				m31 * factor, m32 * factor
			};
		}
		constexpr auto operator*=(const float factor) noexcept -> Matrix3x2&
		{
			m11 *= factor;
			m12 *= factor;
			
			m21 *= factor;
			m22 *= factor;
			
			m31 *= factor;
			m32 *= factor;
		
			return *this;
		}
		[[nodiscard]] friend constexpr auto operator*(const float factor, const Matrix3x2& matrix) noexcept
		{
			return matrix * factor;
		}

		[[nodiscard]] constexpr auto operator*(const Point2F& point) const noexcept
		{
			return Transform(point);
		}

		[[nodiscard]] constexpr auto operator/(const float factor) const noexcept -> Matrix3x2
		{
			return Matrix3x2{
				m11 / factor, m12 / factor,
				m21 / factor, m22 / factor,
				m31 / factor, m32 / factor
			};
		}
		constexpr auto operator/=(const float factor) noexcept -> Matrix3x2&
		{
			m11 /= factor;
			m12 /= factor;

			m21 /= factor;
			m22 /= factor;

			m31 /= factor;
			m32 /= factor;

			return *this;
		}

		[[nodiscard]] constexpr auto operator==(const Matrix3x2& other) const noexcept -> bool = default;
	};
}

template <typename CharT>
struct std::formatter<PGUI::Matrix3x2, CharT>
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
		throw std::format_error{ "No formatting args supported for Matrix3x2" };
	}

	template <typename FormatContext>
	auto format(const PGUI::Matrix3x2& matrix, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), 
			"[[{:.3f}, {:.3f}, {:.3f}], {:.3f}, {:.3f}, {:.3f}]]",
			matrix.m11, matrix.m12,
			matrix.m21, matrix.m22,
			matrix.m31, matrix.m32);
	}
};

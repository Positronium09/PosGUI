module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DGeometry;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.Exceptions;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	constexpr auto DEFAULT_FLATTENING_TOLERANCE = D2D1_DEFAULT_FLATTENING_TOLERANCE;

	struct PointAndUnitTangent
	{
		PointF point{ };
		PointF unitTangentVector{ };
	};

	template <typename Interface = ID2D1Geometry>
	class D2DGeometry : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, ID2D1Geometry>, "Interface must be derived from ID2D1Geometry");

		public:
		constexpr D2DGeometry() noexcept = default;

		explicit(false) constexpr D2DGeometry(const ComPtr<Interface>& geometry) noexcept :
			ComPtrHolder<Interface>{ geometry }
		{ }

		[[nodiscard]] auto GetBounds(const Matrix3x2& worldTransform = Matrix3x2{ })
		{
			D2D1_RECT_F bounds{ };
			auto hr = this->Get()->GetBounds(worldTransform, &bounds);
			ThrowFailed(hr);

			return bounds;
		}

		[[nodiscard]] auto GetWidenedBounds(
			float strokeWidth,
			ComPtr<ID2D1StrokeStyle> strokeStyle = nullptr,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE)
		{
			D2D1_RECT_F bounds{ };
			auto hr = this->Get()->GetWidenedBounds(
				strokeWidth, strokeStyle.Get(),
				worldTransform, flatteningTolerance, &bounds);
			ThrowFailed(hr);

			return bounds;
		}

		[[nodiscard]] auto ComputeArea(
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE)
		{
			auto area = 0.0F;
			auto hr = this->Get()->ComputeArea(
				worldTransform,
				flatteningTolerance, &area);
			ThrowFailed(hr);

			return area;
		}

		[[nodiscard]] auto FillContainsPoint(
			PointF point,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) -> bool
		{
			auto contains = FALSE;
			auto hr = this->Get()->FillContainsPoint(
				point, worldTransform,
				flatteningTolerance, &contains);
			ThrowFailed(hr);

			return contains;
		}

		[[nodiscard]] auto StrokeContainsPoint(
			PointF point, float strokeWidth,
			ComPtr<ID2D1StrokeStyle> strokeStyle = nullptr,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) -> bool
		{
			auto contains = FALSE;
			auto hr = this->Get()->StrokeContainsPoint(
				point, strokeWidth, strokeStyle.Get(),
				worldTransform, flatteningTolerance, &contains);
			ThrowFailed(hr);
			return contains;
		}

		[[nodiscard]] auto ComputeLength(
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE)
		{
			auto length = 0.0F;
			auto hr = this->Get()->ComputeLength(
				worldTransform,
				flatteningTolerance, &length);
			ThrowFailed(hr);

			return length;
		}

		[[nodiscard]] auto ComputePointAtLength(
			float length,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE)
		{
			D2D1_POINT_2F point;
			D2D1_POINT_2F unitTangentVector;

			auto hr = this->Get()->ComputePointAtLength(
				length, worldTransform, flatteningTolerance,
				&point, &unitTangentVector);
			ThrowFailed(hr);

			return PointAndUnitTangent{ .point = point, .unitTangentVector = unitTangentVector };
		}

		[[nodiscard]] auto CompareWithGeometry(
			D2DGeometry<> inputGeometry,
			const Matrix3x2& inputGeometryTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE)
		{
			D2D1_GEOMETRY_RELATION relation;
			auto hr = this->Get()->CompareWithGeometry(
				inputGeometry.Get(), inputGeometryTransform,
				flatteningTolerance, &relation);
			ThrowFailed(hr);

			return static_cast<GeometryRelation>(relation);
		}

		//TODO Tessellate
		//TODO Simplify
		//TODO Outline
		//TODO Widen
		//TODO CombineWithGeometry

		explicit(false) constexpr operator D2DGeometry<>() const noexcept
		{
			return D2DGeometry<>{ this->Get() };
		}
	};
}

module;
#include <d2d1_1.h>
#include <wrl.h>

export module PGUI.UI.D2D.D2DGeometry;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.ErrorHandling;
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

		[[nodiscard]] auto GetBounds(const Matrix3x2& worldTransform = Matrix3x2{ }) const noexcept -> Result<RectF>
		{
			D2D1_RECT_F bounds{ };

			if (auto hr = this->Get()->GetBounds(worldTransform, &bounds);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to get bounds");
				return Unexpected{ error };
			}

			return RectF{ bounds };
		}

		[[nodiscard]] auto GetWidenedBounds(
			float strokeWidth,
			const ComPtr<ID2D1StrokeStyle>& strokeStyle = nullptr,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<RectF>
		{
			D2D1_RECT_F bounds{ };

			if (auto hr = this->Get()->GetWidenedBounds(
				strokeWidth, strokeStyle.Get(),
				worldTransform, flatteningTolerance, &bounds);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to get widened bounds");
				return Unexpected{ error };
			}
			

			return RectF{ bounds };
		}

		[[nodiscard]] auto ComputeArea(
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<float>
		{
			auto area = 0.0F;
			if (auto hr = this->Get()->ComputeArea(
				worldTransform,
				flatteningTolerance, &area);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to compute area");
				return Unexpected{ error };
			}

			return area;
		}

		[[nodiscard]] auto FillContainsPoint(
			PointF point,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<bool>
		{
			auto contains = FALSE;
			if (auto hr = this->Get()->FillContainsPoint(
				point, worldTransform,
				flatteningTolerance, &contains);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to check if point is contained in fill");
				return Unexpected{ error };
			}

			return contains;
		}

		[[nodiscard]] auto StrokeContainsPoint(
			PointF point, float strokeWidth,
			const ComPtr<ID2D1StrokeStyle>& strokeStyle = nullptr,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<bool>
		{
			auto contains = FALSE;
			if (auto hr = this->Get()->StrokeContainsPoint(
				point, strokeWidth, strokeStyle.Get(),
				worldTransform, flatteningTolerance, &contains);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to check if point is contained in stroke");
				return Unexpected{ error };
			}

			return contains;
		}

		[[nodiscard]] auto ComputeLength(
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<float>
		{
			auto length = 0.0F;
			if (auto hr = this->Get()->ComputeLength(
				worldTransform,
				flatteningTolerance, &length);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to compute length");
				return Unexpected{ error };
			}

			return length;
		}

		[[nodiscard]] auto ComputePointAtLength(
			float length,
			const Matrix3x2& worldTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<PointAndUnitTangent>
		{
			D2D1_POINT_2F point;
			D2D1_POINT_2F unitTangentVector;

			if (auto hr = this->Get()->ComputePointAtLength(
				length, worldTransform, flatteningTolerance,
				&point, &unitTangentVector);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to compute point at length");
				return Unexpected{ error };
			}

			return PointAndUnitTangent{ .point = point, .unitTangentVector = unitTangentVector };
		}

		[[nodiscard]] auto CompareWithGeometry(
			D2DGeometry<> inputGeometry,
			const Matrix3x2& inputGeometryTransform = Matrix3x2{ },
			float flatteningTolerance = DEFAULT_FLATTENING_TOLERANCE) const noexcept -> Result<GeometryRelation>
		{
			D2D1_GEOMETRY_RELATION relation;
			if (auto hr = this->Get()->CompareWithGeometry(
				inputGeometry.GetRaw(), inputGeometryTransform,
				flatteningTolerance, &relation);
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Failed to compare with geometry");
				return Unexpected{ error };
			}

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

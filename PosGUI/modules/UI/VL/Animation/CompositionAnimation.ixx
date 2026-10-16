module;
#include <d2d1_1.h>
#include <Windows.h>

#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionAnimation;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.Utils;
import PGUI.UI.Color;
import PGUI.UI.VL.CompositionObject;
import PGUI.UI.VL.InitialValueExpressionCollection;

namespace WF = winrt::Windows::Foundation;
namespace WFN = WF::Numerics;
namespace WG = winrt::Windows::Graphics;
namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionAnimation> Projected = WUC::CompositionAnimation>
	class CompositionAnimationT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;

		[[nodiscard]] auto GetInitialValueExpressions() const noexcept -> Result<InitialValueExpressionCollection>
		{
			return CallProjected([this]
			{
				return InitialValueExpressionCollection{ this->Get().InitialValueExpressions() };
			});
		}

		[[nodiscard]] auto GetTarget() const noexcept -> Result<std::wstring>
		{
			return CallProjected([this]
			{
				return std::wstring{ this->Get().Target() };
			});
		}
		[[nodiscard]] auto SetTarget() const noexcept -> Result<void>
		{
			static_assert(false);
			return EmptyResult;
		}

		[[nodiscard]] auto ClearAllParameters() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().ClearAllParameters();
			});
		}

		[[nodiscard]] auto ClearParameter(const std::wstring_view key) const noexcept -> Result<void>
		{
			return CallProjected([this, &key]
			{
				this->Get().ClearParameter(key);
			});
		}

		[[nodiscard]] auto SetBooleanParameter(const std::wstring_view key, const bool value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, value]
			{
				this->Get().SetBooleanParameter(key, value);
			});
		}

		[[nodiscard]] auto SetColorParameter(const std::wstring_view key, const RGBA& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetColorParameter(key, value);
			});
		}

		[[nodiscard]] auto SetExpressionReferenceParameter(
			const std::wstring_view key, 
			const WrapperDerivedFrom<WUC::IAnimationObject> auto& source) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &source]
			{
				this->Get().SetExpressionReferenceParameter(key, source.Get());
			});
		}

		[[nodiscard]] auto SetMatrix3x2Parameter(const std::wstring_view key, const Matrix3x2& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetMatrix3x2Parameter(key, value);
			});
		}

		[[nodiscard]] auto SetMatrix4x4Parameter(const std::wstring_view key, const Matrix4x4& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetMatrix4x4Parameter(key, value);
			});
		}

		[[nodiscard]] auto SetQuaternionParameter(const std::wstring_view key, const Quaternion& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetQuaternionParameter(key, value);
			});
		}

		[[nodiscard]] auto SetReferenceParameter(const std::wstring_view key, const CompositionObject& compositionObject) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &compositionObject]
			{
				this->Get().SetReferenceParameter(key, compositionObject.Get());
			});
		}

		[[nodiscard]] auto SetScalarParameter(const std::wstring_view key, const float value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, value]
			{
				this->Get().SetScalarParameter(key, value);
			});
		}

		[[nodiscard]] auto SetVector2Parameter(const std::wstring_view key, const Point2F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetVector2Parameter(key, value);
			});
		}

		[[nodiscard]] auto SetVector3Parameter(const std::wstring_view key, const Point3F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetVector3Parameter(key, value);
			});
		}

		//TODO SetVector4Parameter(string key, Vector4 value) -> void

		template <typename T>
		[[nodiscard]] auto SetParameter(const std::wstring_view key, const T& value) const noexcept -> Result<void>
		{
			if constexpr (std::is_same_v<T, bool>)
			{
				return SetBooleanParameter(key, value);
			}
			else if constexpr (
				std::convertible_to<T, RGBA> && 
				!std::floating_point<T> && !std::integral<T>)
			{
				return SetColorParameter(key, value);
			}
			else if constexpr (std::convertible_to<T, Matrix3x2>)
			{
				return SetMatrix3x2Parameter(key, value);
			}
			else if constexpr (std::convertible_to<T, Matrix4x4>)
			{
				return SetMatrix4x4Parameter(key, value);
			}
			else if constexpr (std::convertible_to<T, Quaternion>)
			{
				return SetQuaternionParameter(key, value);
			}
			else if constexpr (WrapperDerivedFrom<T, WUC::CompositionObject>)
			{
				return SetReferenceParameter(key, value);
			}
			else if constexpr (WrapperDerivedFrom<T, WUC::IAnimationObject>)
			{
				return SetExpressionReferenceParameter(key, value);
			}
			else if constexpr (std::floating_point<T> || std::integral<T>)
			{
				return SetScalarParameter(key, static_cast<float>(value));
			}
			else if constexpr (IsInTypeList<T,
				Point2F, Point2I, Point2L, Point2U,
				WFN::float2, WF::Point, WG::PointInt32,
				POINT, POINTS, D2D1_POINT_2F, D2D1_POINT_2U>)
			{
				return SetVector2Parameter(key, value);
			}
			else if constexpr (IsInTypeList<T,
				Point3F, Point3I, Point3L, Point3U,
				WFN::float3>)
			{
				return SetVector3Parameter(key, value);
			}
			else
			{
				static_assert(false, "Unsupported parameter type for SetParameter");
				return EmptyResult;
			}
		}
	};
	using CompositionAnimation = CompositionAnimationT<>;
}

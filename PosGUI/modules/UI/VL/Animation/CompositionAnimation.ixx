module;
#include <winrt/Windows.UI.Composition.h>

export module PGUI.UI.VL.CompositionAnimation;

import std;

import PGUI.ErrorHandling;
import PGUI.ProjectionHolder;
import PGUI.Shape;
import PGUI.Wrapper;
import PGUI.UI.Color;
import PGUI.UI.VL.CompositionObject;

namespace WUC = winrt::Windows::UI::Composition;

export namespace PGUI::UI::VL
{
	template <HandleDerivedFrom<WUC::CompositionAnimation> Projected = WUC::CompositionAnimation>
	class CompositionAnimationT : public CompositionObjectT<Projected>
	{
		public:
		using Base = CompositionObjectT<Projected>;
		using Base::Base;

		//TODO InitialValueExpressions InitialValueExpressionCollection

		[[nodiscard]] auto GetTarget() const noexcept -> Result<std::wstring>
		{
			return CallProjected([this]
			{
				return std::wstring{ this->Get().Target() };
			});
		}
		[[nodiscard]] auto SetTarget(const std::wstring& target) const noexcept -> Result<void>
		{
			return CallProjected([this, &target]
			{
				this->Get().Target(target);
			});
		}

		[[nodiscard]] auto ClearAllParameters() const noexcept -> Result<void>
		{
			return CallProjected([this]
			{
				this->Get().ClearAllParameters();
			});
		}

		[[nodiscard]] auto ClearParameter(const std::wstring& key) const noexcept -> Result<void>
		{
			return CallProjected([this, &key]
			{
				this->Get().ClearParameter(key);
			});
		}

		[[nodiscard]] auto SetBooleanParameter(const std::wstring& key, const bool value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, value]
			{
				this->Get().SetBooleanParameter(key, value);
			});
		}

		[[nodiscard]] auto SetColorParameter(const std::wstring& key, const RGBA& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetColorParameter(key, value);
			});
		}

		//TODO SetExpressionReferenceParameter(string parameterName, IAnimationObject source) -> void

		[[nodiscard]] auto SetMatrix3x2Parameter(const std::wstring& key, const Matrix3x2& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetMatrix3x2Parameter(key, value);
			});
		}

		[[nodiscard]] auto SetMatrix4x4Parameter(const std::wstring& key, const Matrix4x4& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetMatrix4x4Parameter(key, value);
			});
		}

		[[nodiscard]] auto SetQuaternionParameter(const std::wstring& key, const Quaternion& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetQuaternionParameter(key, value);
			});
		}

		[[nodiscard]] auto SetReferenceParameter(const std::wstring& key, const CompositionObject& compositionObject) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &compositionObject]
			{
				this->Get().SetReferenceParameter(key, compositionObject.Get());
			});
		}

		[[nodiscard]] auto SetScalarParameter(const std::wstring& key, const float value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, value]
			{
				this->Get().SetScalarParameter(key, value);
			});
		}

		[[nodiscard]] auto SetVector2Parameter(const std::wstring& key, const Point2F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetVector2Parameter(key, value);
			});
		}

		[[nodiscard]] auto SetVector3Parameter(const std::wstring& key, const Point3F& value) const noexcept -> Result<void>
		{
			return CallProjected([this, &key, &value]
			{
				this->Get().SetVector3Parameter(key, value);
			});
		}

		//TODO SetVector4Parameter(string key, Vector4 value) -> void
	};
	using CompositionAnimation = CompositionAnimationT<>;
}

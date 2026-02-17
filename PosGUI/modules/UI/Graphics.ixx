module;
#include <d2d1_3.h>

export module PGUI.UI.Graphics;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.D2D.DeviceContext;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI
{
	class Graphics : public D2D::DeviceContext<ID2D1DeviceContext7>
	{
		public:
		explicit(false) Graphics(const ComPtr<ID2D1DeviceContext7>& deviceContext) noexcept;

		auto PushTransform(const D2D::Matrix3x2& transform) const -> void;

		auto PushTranslation(const PointF offset) const -> void
		{
			PushTransform(D2D::Matrix3x2::Translation(offset.x, offset.y));
		}

		auto PushScale(const SizeF scale, const PointF center = {}) const -> void
		{
			PushTransform(D2D::Matrix3x2::Scale(scale, center));
		}

		auto PushRotation(const float angle, const PointF center = {}) const -> void
		{
			PushTransform(D2D::Matrix3x2::Rotation(angle, center));
		}

		auto PushSkew(const float angleX, const float angleY, const PointF center = {}) const -> void
		{
			PushTransform(D2D::Matrix3x2::Skew(angleX, angleY, center));
		}

		auto PopTransform() const -> void;

		private:
		mutable std::vector<D2D::Matrix3x2> transformStack;
	};
}

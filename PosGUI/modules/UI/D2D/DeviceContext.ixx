module;
#include <Windows.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <bit>
#include <concepts>
#include <optional>
#include <type_traits>

export module PGUI.UI.D2D.DeviceContext;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.D2D.RenderTarget;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.Effect;
import PGUI.UI.D2D.D2DImage;

export namespace PGUI::UI::D2D
{
	template <typename Interface = ID2D1DeviceContext>
	class DeviceContext : public RenderTarget<Interface>
	{
		static_assert(std::derived_from<Interface, ID2D1DeviceContext>, "Interface must be derived from ID2D1DeviceContext");

		public:
		DeviceContext() noexcept = default;
		DeviceContext(ComPtr<Interface> deviceContext) noexcept :
			RenderTarget<Interface>{ deviceContext }
		{
		}
		~DeviceContext() noexcept = default;

		auto CreateEffect(const IID& effectID) const
		{
			Effect effect{ };
			auto hr = this->Get()->CreateEffect(effectID, effect.GetAddress()); ThrowFailed(hr);

			return effect;
		}

		void DrawImage(D2DImage<> image, 
			std::optional<PointF> targetOffset = std::nullopt,
			std::optional<RectF> imageRect = std::nullopt,
			InterpolationMode interpolationMode = InterpolationMode::Linear,
			CompositeMode compositeMode = CompositeMode::SourceOver) const noexcept
		{
			D2D1_POINT_2F* offset = nullptr;
			if (imageRect.has_value())
			{
				offset = std::bit_cast<D2D1_POINT_2F*>(&targetOffset.value());
			}

			D2D1_RECT_F* imgRect = nullptr;
			if (imageRect.has_value())
			{
				imgRect = std::bit_cast<D2D1_RECT_F*>(&imageRect.value());
			}

			this->Get()->DrawImage(image.GetRaw(), offset, imgRect,
				static_cast<D2D1_INTERPOLATION_MODE>(interpolationMode),
				static_cast<D2D1_COMPOSITE_MODE>(compositeMode));
		}
		void DrawImage(Effect effect,
			std::optional<PointF> targetOffset = std::nullopt,
			std::optional<RectF> imageRect = std::nullopt,
			InterpolationMode interpolationMode = InterpolationMode::Linear,
			CompositeMode compositeMode = CompositeMode::SourceOver) const noexcept
		{
			D2D1_POINT_2F* offset = nullptr;
			if (targetOffset.has_value())
			{
				offset = std::bit_cast<D2D1_POINT_2F*>(&targetOffset.value());
			}

			D2D1_RECT_F* imgRect = nullptr;
			if (imageRect.has_value())
			{
				imgRect = std::bit_cast<D2D1_RECT_F*>(&imageRect.value());
			}

			this->Get()->DrawImage(effect.GetRaw(), offset, imgRect,
				static_cast<D2D1_INTERPOLATION_MODE>(interpolationMode),
				static_cast<D2D1_COMPOSITE_MODE>(compositeMode));
		}
	};
}

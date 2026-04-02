module;
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <Windows.h>

export module PGUI.UI.D2D.DeviceContext;

import std;

import PGUI.Shape2D;
import PGUI.ComPtr;
import PGUI.Utils;
import PGUI.ErrorHandling;
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
		static_assert(std::derived_from<Interface, ID2D1DeviceContext>,
		              "Interface must be derived from ID2D1DeviceContext");

		public:
		DeviceContext() noexcept = default;

		explicit(false) DeviceContext(ComPtr<Interface> deviceContext) noexcept :
			RenderTarget<Interface>{ deviceContext }
		{ }

		~DeviceContext() noexcept = default;

		auto CreateEffect(const IID& effectID) const noexcept -> Result<Effect>
		{
			Effect effect{ };
			if (auto hr = this->Get()->CreateEffect(effectID, effect.Put()); 
				FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, std::format(L"Cannot create effect with IID {}", effectID));
				return Unexpected{ error };
			}

			return effect;
		}

		auto DrawImage(D2DImage<> image,
		               std::optional<PointF> targetOffset = std::nullopt,
		               std::optional<RectF> imageRect = std::nullopt,
		               const InterpolationMode interpolationMode = InterpolationMode::Linear,
		               const CompositeMode compositeMode = CompositeMode::SourceOver) const noexcept -> void
		{
			D2D1_POINT_2F* offset = nullptr;
			if (targetOffset.has_value())
			{
				offset = reinterpret_cast<D2D1_POINT_2F*>(&targetOffset.value());
			}

			D2D1_RECT_F* imgRect = nullptr;
			if (imageRect.has_value())
			{
				imgRect = reinterpret_cast<D2D1_RECT_F*>(&imageRect.value());
			}

			this->Get()->DrawImage(image.GetRaw(), offset, imgRect,
			                       ToUnderlying(interpolationMode),
			                       ToUnderlying(compositeMode));
		}

		auto DrawImage(Effect effect,
		               std::optional<PointF> targetOffset = std::nullopt,
		               std::optional<RectF> imageRect = std::nullopt,
		               const InterpolationMode interpolationMode = InterpolationMode::Linear,
		               const CompositeMode compositeMode = CompositeMode::SourceOver) const noexcept -> void
		{
			D2D1_POINT_2F* offset = nullptr;
			if (targetOffset.has_value())
			{
				offset = reinterpret_cast<D2D1_POINT_2F*>(&targetOffset.value());
			}

			D2D1_RECT_F* imgRect = nullptr;
			if (imageRect.has_value())
			{
				imgRect = reinterpret_cast<D2D1_RECT_F*>(&imageRect.value());
			}

			this->Get()->DrawImage(effect.GetRaw(), offset, imgRect,
			                       ToUnderlying(interpolationMode),
			                       ToUnderlying(compositeMode));
		}
	};
}

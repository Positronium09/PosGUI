module;
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>

export module PGUI.UI.D2D.RenderTarget;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.Brush;
import PGUI.UI.Color;
import PGUI.ErrorHandling;
import PGUI.UI.TextFormat;
import PGUI.UI.Font.FontEnums;
import PGUI.UI.TextLayout;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;
import PGUI.UI.D2D.D2DBitmap;
import PGUI.UI.D2D.D2DGeometry;
import PGUI.UI.D2D.D2DLayer;
import PGUI.UI.Imaging.BitmapSource;

export namespace PGUI::UI::D2D
{
	template <typename Interface = ID2D1RenderTarget>
	class RenderTarget : public ComPtrHolder<Interface>
	{
		static_assert(std::derived_from<Interface, ID2D1RenderTarget>,
		              "Interface must be derived from ID2D1RenderTarget");

		public:
		RenderTarget() noexcept = default;

		explicit(false) RenderTarget(ComPtr<Interface> renderTarget) noexcept :
			ComPtrHolder<Interface>{ renderTarget }
		{ }

		~RenderTarget() noexcept = default;

		auto BeginDraw() noexcept -> void
		{
			this->Get()->BeginDraw();
		}

		auto EndDraw() noexcept
		{
			return this->Get()->EndDraw();
		}

		auto Clear(const Brush& brush) noexcept -> void
		{
			const auto size = GetSize();
			const auto rect = RectF{ { }, size };

			FillRectangle(rect, brush);
		}

		auto Clear(RGBA color) noexcept -> void
		{
			this->Get()->Clear(color);
		}

		[[nodiscard]] auto CreateBrush(const BrushParameters& parameters)
		{
			return Brush{ this->template GetAs<ID2D1RenderTarget>(), parameters };
		}

		[[nodiscard]] auto CreateBitmap(SizeU size, BitmapProperties properties) noexcept -> Result<D2DBitmap>
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmap(size, properties, &bitmap);
			if (FAILED(hr))
			{
				Error error{ hr };
				error
					.AddDetail(L"Size", std::format(L"{}", size));
				Logger::Error(error, L"Cannot create bitmap");
				return Unexpected{ error };
			}

			hr = bitmap.As(&bitmap1);
			if (FAILED(hr))
			{
				Error error{ hr };
				error
					.AddDetail(L"Size", std::format(L"{}", size));
				Logger::Error(error, L"Cannot get ID2D1Bitmap1 interface");
				return Unexpected{ error };
			}

			return D2DBitmap{ bitmap1 };
		}

		[[nodiscard]] auto CreateBitmap(
			SizeU size,
			std::span<std::byte> sourceData,
			UINT32 pitch, BitmapProperties properties) noexcept -> Result<D2DBitmap>
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmap(size, sourceData.data(), pitch, properties, &bitmap);
			if (FAILED(hr))
			{
				Error error{ hr };
				error
					.AddDetail(L"Size", std::format(L"{}", size))
;
				Logger::Error(error, L"Cannot create bitmap");
				return Unexpected{ error };
			}

			hr = bitmap.As(&bitmap1);
			if (FAILED(hr))
			{
				Error error{ hr };
				error
					.AddDetail(L"Size", std::format(L"{}", size))
;
				Logger::Error(error, L"Cannot get ID2D1Bitmap1 interface");
				return Unexpected{ error };
			}
			return D2DBitmap{ bitmap1 };
		}

		[[nodiscard]] auto CreateBitmapFromSource(
			const Imaging::BitmapSource<>& source,
			std::optional<BitmapProperties> properties) noexcept -> Result<D2DBitmap>
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmapFromWicBitmap(
				source.GetRawAs<IWICBitmapSource>(),
				properties.has_value() ? &properties : nullptr, &bitmap);
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot create bitmap from source {}");
				return Unexpected{ error };
			}

			hr = bitmap.As(&bitmap1);
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot get ID2D1Bitmap1 interface {}");
				return Unexpected{ error };
			}
			return D2DBitmap{ bitmap1 };
		}

		[[nodiscard]] auto CreateBitmapFromSource(const Imaging::BitmapSource<>& source) noexcept -> Result<D2DBitmap>
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmapFromWicBitmap(source.GetRawAs<IWICBitmapSource>(), &bitmap);
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot create bitmap from source {}");
				return Unexpected{ error };
			}

			hr = bitmap.As(&bitmap1);
			if (FAILED(hr))
			{
				Error error{ hr };
				Logger::Error(error, L"Cannot get ID2D1Bitmap1 interface {}");
				return Unexpected{ error };
			}
			return D2DBitmap{ bitmap1 };
		}

		[[nodiscard]] auto CreateLayer(SizeF size = SizeF{ }) noexcept -> Result<D2DLayer>
		{
			ComPtr<ID2D1Layer> layer;

			if (auto hr = this->Get()->CreateLayer(size, &layer); 
				FAILED(hr))
			{
				Error error{ hr };
				error
					.AddDetail(L"Size", std::format(L"{}", size));
				Logger::Error(error, L"Cannot create layer {}");
				return Unexpected{ error };
			}
			return D2DLayer{ layer };
		}

		auto DrawLine(PointF p1, PointF p2, Brush brush, float strokeWidth = 1.0F) noexcept -> void
		{
			this->Get()->DrawLine(p1, p2, brush, strokeWidth);
		}

		auto DrawLine(LineSegmentF lineSegment, Brush brush, float strokeWidth = 1.0F) noexcept -> void
		{
			this->Get()->DrawLine(lineSegment.start, lineSegment.end, brush, strokeWidth);
		}

		auto DrawRectangle(RectF rect, Brush brush, float strokeWidth = 1.0F) noexcept -> void
		{
			this->Get()->DrawRectangle(rect, brush, strokeWidth);
		}

		auto FillRectangle(RectF rect, Brush brush) noexcept -> void
		{
			this->Get()->FillRectangle(rect, brush);
		}

		auto DrawRoundedRectangle(RoundedRect rect, Brush brush, float strokeWidth = 1.0F) noexcept -> void
		{
			this->Get()->DrawRoundedRectangle(rect, brush, strokeWidth);
		}

		auto FillRoundedRectangle(RoundedRect rect, Brush brush) noexcept -> void
		{
			this->Get()->FillRoundedRectangle(rect, brush);
		}

		auto DrawEllipse(Ellipse ellipse, Brush brush, float strokeWidth = 1.0F) noexcept -> void
		{
			this->Get()->DrawEllipse(ellipse, brush, strokeWidth);
		}

		auto FillEllipse(Ellipse ellipse, Brush brush) noexcept -> void
		{
			this->Get()->FillEllipse(ellipse, brush);
		}

		auto DrawGeometry(D2DGeometry<> geometry, Brush brush, float strokeWidth = 1.0F) noexcept -> void
		{
			this->Get()->DrawGeometry(geometry.GetRaw(), brush, strokeWidth);
		}

		auto FillGeometry(D2DGeometry<> geometry, Brush brush /* Write BitmapBrush */) -> void
		{
			this->Get()->FillGeometry(geometry.GetRaw(), brush, nullptr);
		}

		auto DrawText(std::wstring_view text, const TextFormat& format, RectF textRect, Brush brush) noexcept -> void
		{
			this->Get()->DrawText(text.data(), static_cast<UINT32>(text.size()),
			                      format.GetRawAs<IDWriteTextFormat>(), textRect, brush);
		}

		auto DrawText(const TextLayout& layout, PointF origin, Brush brush,
		              DrawTextOptions drawTextOptions = DrawTextOptions::EnableColorFont) noexcept -> void
		{
			this->Get()->DrawTextLayout(origin, layout.GetRawAs<IDWriteTextLayout>(), brush,
			                            static_cast<D2D1_DRAW_TEXT_OPTIONS>(drawTextOptions));
		}

		auto DrawBitmap(D2DBitmap bitmap,
		                std::optional<RectF> destinationRect = std::nullopt,
		                std::optional<RectF> sourceRect = std::nullopt,
		                BitmapInterpolationMode interpolationMode = BitmapInterpolationMode::Linear,
		                float opacity = 1.0F) noexcept -> void
		{
			D2D1_RECT_F* destRect = nullptr;
			if (destinationRect.has_value())
			{
				destRect = std::bit_cast<D2D1_RECT_F*>(&destinationRect.value());
			}

			D2D1_RECT_F* srcRect = nullptr;
			if (sourceRect.has_value())
			{
				srcRect = std::bit_cast<D2D1_RECT_F*>(&sourceRect.value());
			}

			this->Get()->DrawBitmap(bitmap.GetRaw(), destRect, opacity,
			                        static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(interpolationMode), srcRect);
		}

		auto FillOpacityMask(D2DBitmap bitmap, Brush brush,
		                     RectF destinationRect, RectF sourceRect) noexcept -> void
		{
			this->Get()->FillOpacityMask(bitmap.GetRaw(), brush,
			                             D2D1_OPACITY_MASK_CONTENT_GRAPHICS |
			                             D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL |
			                             D2D1_OPACITY_MASK_CONTENT_TEXT_GDI_COMPATIBLE,
			                             destinationRect, sourceRect);
		}

		auto SetTransform(Matrix3x2 transform) noexcept -> void
		{
			this->Get()->SetTransform(transform);
		}

		[[nodiscard]] auto GetTransform() const noexcept -> Matrix3x2
		{
			D2D1_MATRIX_3X2_F transform{ };
			this->Get()->GetTransform(&transform);

			return D2D1MatrixToMatrix3x2(transform);
		}

		auto ResetTransform() noexcept
		{
			auto transform = GetTransform();
			SetTransform(Matrix3x2::Identity());

			return transform;
		}

		auto SetAntialiasingMode(AntiAliasingMode mode) noexcept -> void
		{
			this->Get()->SetAntialiasMode(static_cast<D2D1_ANTIALIAS_MODE>(mode));
		}

		[[nodiscard]] auto GetAntialiasingMode() const noexcept
		{
			return static_cast<AntiAliasingMode>(this->Get()->GetAntialiasMode());
		}

		auto SetTextAntialiasingMode(TextAntialiasingMode mode) noexcept -> void
		{
			this->Get()->SetTextAntialiasMode(static_cast<D2D1_TEXT_ANTIALIAS_MODE>(mode));
		}

		auto GetTextAntialiasingMode() const noexcept
		{
			return static_cast<TextAntialiasingMode>(this->Get()->GetTextAntialiasMode());
		}

		auto SetTextRenderingParams(const ComPtr<IDWriteRenderingParams>& params) noexcept -> void
		{
			this->Get()->SetTextRenderingParams(params.Get());
		}

		[[nodiscard]] auto GetTextRenderingParams() const noexcept
		{
			ComPtr<IDWriteRenderingParams> params;
			this->Get()->GetTextRenderingParams(&params);

			return params;
		}

		auto SetTags(D2D1_TAG tag1, D2D1_TAG tag2) noexcept -> void
		{
			this->Get()->SetTags(tag1, tag2);
		}

		[[nodiscard]] auto GetTags() const noexcept
		{
			D2D1_TAG tag1{ };
			D2D1_TAG tag2{ };
			this->Get()->GetTags(&tag1, &tag2);

			return std::pair{ tag1, tag2 };
		}

		auto PushLayer(D2DLayer layer, const LayerParameters& layerParameters) -> void
		{
			this->Get()->PushLayer(layerParameters, layer.GetRaw());
		}

		auto PushLayer(const LayerParameters& layerParameters) -> void
		{
			this->Get()->PushLayer(layerParameters, nullptr);
		}

		auto PopLayer() noexcept -> void
		{
			this->Get()->PopLayer();
		}

		auto Flush(std::optional<std::pair<D2D1_TAG, D2D1_TAG>> tags) noexcept -> void
		{
			if (tags.has_value())
			{
				this->Get()->Flush(&tags->first, &tags->second);
			}
			else
			{
				this->Get()->Flush();
			}
		}

		auto PushAxisAlignedClip(const RectF clipRect, const AntiAliasingMode antialiasMode) noexcept -> void
		{
			this->Get()->PushAxisAlignedClip(clipRect, static_cast<D2D1_ANTIALIAS_MODE>(antialiasMode));
		}

		auto PopAxisAlignedClip() noexcept -> void
		{
			this->Get()->PopAxisAlignedClip();
		}

		auto SetDpi(const float dpiX, const float dpiY) noexcept -> void
		{
			this->Get()->SetDpi(dpiX, dpiY);
		}

		auto SetDpi(const float dpi) noexcept -> void
		{
			SetDpi(dpi, dpi);
		}

		auto SetDpi(const std::pair<float, float> dpi) noexcept -> void
		{
			SetDpi(dpi.first, dpi.second);
		}

		[[nodiscard]] auto GetDpi() const noexcept
		{
			auto dpiX = 0.0F;
			auto dpiY = 0.0F;
			this->Get()->GetDpi(&dpiX, &dpiY);

			return std::pair{ dpiX, dpiY };
		}

		[[nodiscard]] auto GetSize() const noexcept -> SizeF
		{
			return this->Get()->GetSize();
		}

		[[nodiscard]] auto GetPixelSize() const noexcept -> SizeU
		{
			return this->Get()->GetPixelSize();
		}

		[[nodiscard]] auto GetMaximumBitmapSize() const noexcept
		{
			return this->Get()->GetMaximumBitmapSize();
		}

		//TODO Add wrapper for D2D1_RENDER_TARGET_PROPERTIES
		[[nodiscard]] auto IsSupported(D2D1_RENDER_TARGET_PROPERTIES properties) const noexcept -> bool
		{
			return this->Get()->IsSupported(&properties);
		}

		//TODO SaveDrawingState
		//TODO RestoreDrawingState

		//TODO FillMesh
		//TODO DrawGlyphRun

		//TODO CreateLayer
		//TODO CreateMesh

		operator RenderTarget<>() const noexcept
		{
			return RenderTarget<>{ this->Get() };
		}
	};
}

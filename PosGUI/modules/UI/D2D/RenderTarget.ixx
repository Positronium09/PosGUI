module;
#include <Windows.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>

export module PGUI.UI.D2D.RenderTarget;

import std;

import PGUI.ComPtr;
import PGUI.Shape2D;
import PGUI.UI.Brush;
import PGUI.UI.Color;
import PGUI.EnumFlag;
import PGUI.Exceptions;
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
		static_assert(std::derived_from<Interface, ID2D1RenderTarget>, "Interface must be derived from ID2D1RenderTarget");

		public:
		RenderTarget() noexcept = default;
		RenderTarget(ComPtr<Interface> renderTarget) noexcept : 
			ComPtrHolder<Interface>{ renderTarget }
		{
		}
		~RenderTarget() noexcept = default;

		void BeginDraw() noexcept
		{
			this->Get()->BeginDraw();
		}
		auto EndDraw() noexcept
		{
			return this->Get()->EndDraw();
		}

		void Clear(Brush brush) noexcept
		{
			auto size = GetSize();
			auto rect = RectF{ { }, size };

			FillRectangle(rect, brush);
		}
		void Clear(RGBA color) noexcept
		{
			this->Get()->Clear(color);
		}

		[[nodiscard]] auto CreateBrush(BrushParameters parameters)
		{
			return Brush{ this->GetAs<ID2D1RenderTarget>(), parameters };
		}

		[[nodiscard]] auto CreateBitmap(SizeU size, BitmapProperties properties)
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmap(size, properties, &bitmap); ThrowFailed(hr);

			hr = bitmap.As(&bitmap1); ThrowFailed(hr);
			return D2DBitmap{ bitmap1 };
		}
		[[nodiscard]] auto CreateBitmap(SizeU size, std::span<std::byte> sourceData, UINT32 pitch, BitmapProperties properties)
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmap(size, sourceData.data(), pitch, properties, &bitmap); ThrowFailed(hr);
		
			hr = bitmap.As(&bitmap1); ThrowFailed(hr);
			return D2DBitmap{ bitmap1 };
		}
		[[nodiscard]] auto CreateBitmapFromSource(Imaging::BitmapSource<> source, 
			std::optional<BitmapProperties> properties)
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmapFromWicBitmap(source.GetRawAs<IWICBitmapSource>(), 
				properties.has_value() ? &properties : nullptr, &bitmap); ThrowFailed(hr);

			hr = bitmap.As(&bitmap1); ThrowFailed(hr);
			return D2DBitmap{ bitmap1 };
		}
		[[nodiscard]] auto CreateBitmapFromSource(Imaging::BitmapSource<> source)
		{
			ComPtr<ID2D1Bitmap> bitmap;
			ComPtr<ID2D1Bitmap1> bitmap1;
			auto hr = this->Get()->CreateBitmapFromWicBitmap(source.GetRawAs<IWICBitmapSource>(), &bitmap); ThrowFailed(hr);

			hr = bitmap.As(&bitmap1); ThrowFailed(hr);
			return D2DBitmap{ bitmap1 };
		}

		[[nodiscard]] auto CreateLayer(SizeF size = SizeF{ })
		{
			ComPtr<ID2D1Layer> layer;

			auto hr = this->Get()->CreateLayer(size, &layer); ThrowFailed(hr);
			return D2DLayer{ layer };
		}

		void DrawLine(PointF p1, PointF p2, Brush brush, float strokeWidth = 1.0F) noexcept
		{
			this->Get()->DrawLine(p1, p2, brush, strokeWidth);
		}
		void DrawLine(LineSegmentF lineSegment, Brush brush, float strokeWidth = 1.0F) noexcept
		{
			this->Get()->DrawLine(lineSegment.start, lineSegment.end, brush, strokeWidth);
		}
		void DrawRectangle(RectF rect, Brush brush, float strokeWidth = 1.0F) noexcept
		{
			this->Get()->DrawRectangle(rect, brush, strokeWidth);
		}
		void FillRectangle(RectF rect, Brush brush) noexcept
		{
			this->Get()->FillRectangle(rect, brush);
		}
		void DrawRoundedRectangle(RoundedRect rect, Brush brush, float strokeWidth = 1.0F) noexcept
		{
			this->Get()->DrawRoundedRectangle(rect, brush, strokeWidth);
		}
		void FillRoundedRectangle(RoundedRect rect, Brush brush) noexcept
		{
			this->Get()->FillRoundedRectangle(rect, brush);
		}
		void DrawEllipse(Ellipse ellipse, Brush brush, float strokeWidth = 1.0F) noexcept
		{
			this->Get()->DrawEllipse(ellipse, brush, strokeWidth);
		}
		void FillEllipse(Ellipse ellipse, Brush brush) noexcept
		{
			this->Get()->FillEllipse(ellipse, brush);
		}
		
		void DrawGeometry(D2DGeometry<> geometry, Brush brush, float strokeWidth = 1.0F) noexcept
		{
			this->Get()->DrawGeometry(geometry.GetRaw(), brush, strokeWidth);
		}
		void FillGeometry(D2DGeometry<> geometry, Brush brush /* Write BitmapBrush */)
		{
			this->Get()->FillGeometry(geometry.GetRaw(), brush, nullptr);
		}

		void DrawText(std::wstring_view text, TextFormat format, RectF textRect, Brush brush) noexcept
		{
			this->Get()->DrawText(text.data(), static_cast<UINT32>(text.size()), 
				format.GetRawAs<IDWriteTextFormat>(), textRect, brush);
		}
		void DrawText(TextLayout layout, PointF origin, Brush brush,
			DrawTextOptions drawTextOptions = DrawTextOptions::EnableColorFont) noexcept
		{
			this->Get()->DrawTextLayout(origin, layout.GetRawAs<IDWriteTextLayout>(), brush,
				static_cast<D2D1_DRAW_TEXT_OPTIONS>(drawTextOptions));
		}

		void DrawBitmap(D2DBitmap bitmap, 
			std::optional<RectF> destinationRect = std::nullopt, 
			std::optional<RectF> sourceRect = std::nullopt,
			BitmapInterpolationMode interpolationMode = BitmapInterpolationMode::Linear,
			float opacity = 1.0F) noexcept
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

		void FillOpacityMask(D2DBitmap bitmap, Brush brush, 
			RectF destinationRect, RectF sourceRect) noexcept
		{
			this->Get()->FillOpacityMask(bitmap.GetRaw(), brush, 
				D2D1_OPACITY_MASK_CONTENT_GRAPHICS | 
				D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL | D2D1_OPACITY_MASK_CONTENT_TEXT_GDI_COMPATIBLE,
				destinationRect, sourceRect);
		}

		void SetTransform(Matrix3x2 transform) noexcept
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

		void SetAntialiasingMode(AntiAliasingMode mode) noexcept
		{
			this->Get()->SetAntialiasMode(static_cast<D2D1_ANTIALIAS_MODE>(mode));
		}
		[[nodiscard]] auto GetAntialiasingMode() const noexcept
		{
			return static_cast<AntiAliasingMode>(this->Get()->GetAntialiasMode());
		}

		void SetTextAntialiasingMode(TextAntialiasingMode mode) noexcept
		{
			this->Get()->SetTextAntialiasMode(static_cast<D2D1_TEXT_ANTIALIAS_MODE>(mode));
		}
		auto GetTextAntialiasingMode() const noexcept
		{
			return static_cast<TextAntialiasingMode>(this->Get()->GetTextAntialiasMode());
		}

		void SetTextRenderingParams(ComPtr<IDWriteRenderingParams> params) noexcept
		{
			this->Get()->SetTextRenderingParams(params.Get());
		}
		[[nodiscard]] auto GetTextRenderingParams() const noexcept
		{
			ComPtr<IDWriteRenderingParams> params;
			this->Get()->GetTextRenderingParams(&params);

			return params;
		}

		void SetTags(D2D1_TAG tag1, D2D1_TAG tag2) noexcept
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

		void PushLayer(D2DLayer layer, const LayerParameters& layerParameters)
		{
			this->Get()->PushLayer(layerParameters, layer.GetRaw());
		}
		void PushLayer(const LayerParameters& layerParameters)
		{
			this->Get()->PushLayer(layerParameters, nullptr);
		}
		void PopLayer() noexcept
		{
			this->Get()->PopLayer();
		}

		void Flush(std::optional<std::pair<D2D1_TAG, D2D1_TAG>> tags) noexcept
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

		void PushAxisAlignedClip(RectF clipRect, AntiAliasingMode antialiasMode) noexcept
		{
			this->Get()->PushAxisAlignedClip(clipRect, static_cast<D2D1_ANTIALIAS_MODE>(antialiasMode));
		}
		void PopAxisAlignedClip() noexcept
		{
			this->Get()->PopAxisAlignedClip();
		}

		void SetDpi(float dpiX, float dpiY) noexcept
		{
			this->Get()->SetDpi(dpiX, dpiY);
		}
		[[nodiscard]] auto GetDpi() const noexcept
		{
			float dpiX = 0.0F;
			float dpiY = 0.0F;
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

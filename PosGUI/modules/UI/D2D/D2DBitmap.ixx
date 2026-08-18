module;
#include <d2d1_3.h>

export module PGUI.UI.D2D.D2DBitmap;

import std;

import PGUI.ComPtr;
import PGUI.Shape;
import PGUI.Utils;
import PGUI.ErrorHandling;
import PGUI.UI.D2D.D2DImage;
import PGUI.UI.D2D.D2DEnums;
import PGUI.UI.D2D.D2DStructs;

export namespace PGUI::UI::D2D
{
	enum class MapOptions
	{
		None = D2D1_MAP_OPTIONS_NONE,
		Read = D2D1_MAP_OPTIONS_READ,
		Write = D2D1_MAP_OPTIONS_WRITE,
		Discard = D2D1_MAP_OPTIONS_DISCARD
	};
	consteval auto MakeEnumFlag(MapOptions) noexcept -> void { }

	class D2DBitmap : public D2DImage<ID2D1Bitmap1>
	{
		public:
		D2DBitmap() noexcept = default;

		explicit(false) D2DBitmap(const ComPtr<ID2D1Bitmap1>& bitmap) noexcept;

		[[nodiscard]] auto GetSize() const noexcept -> SizeF
		{
			return Get()->GetSize();
		}

		[[nodiscard]] auto GetPixelSize() const noexcept -> SizeU
		{
			return Get()->GetPixelSize();
		}

		[[nodiscard]] auto GetDpi() const noexcept -> std::pair<float, float>;

		[[nodiscard]] auto GetPixelFormat() const noexcept
		{
			return Get()->GetPixelFormat();
		}

		[[nodiscard]] auto GetBitmapOptions() const noexcept
		{
			return FromUnderlying<BitmapOptions>(Get()->GetOptions());
		}

		[[nodiscard]] auto GetSurface() const noexcept -> Result<ComPtr<IDXGISurface>>;

		[[nodiscard]] auto Map(MapOptions options) const noexcept -> Result<MappedRect>;

		[[nodiscard]] auto Unmap() const noexcept -> Result<void>;

		[[nodiscard]] auto CopyFromBitmap(
			D2DBitmap bitmap,
			std::optional<PointU> destPoint = std::nullopt,
			std::optional<RectU> srcRect = std::nullopt) const noexcept -> Result<void>;

		[[nodiscard]] auto CopyFromMemory(
			const void* source, UINT32 pitch,
			std::optional<RectU> destRect = std::nullopt) const noexcept -> Result<void>;

		//TODO GetColorContext
	};
}

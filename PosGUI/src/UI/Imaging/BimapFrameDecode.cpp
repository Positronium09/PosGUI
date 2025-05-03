module;
#include <vector>
#include <ranges>
#include <wrl.h>
#include <wincodec.h>

module PGUI.UI.Imaging.BitmapFrameDecode;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.MetadataReader;

namespace PGUI::UI::Imaging
{
	BitmapFrameDecode::BitmapFrameDecode(ComPtr<IWICBitmapFrameDecode> frame) noexcept : 
		BitmapSource{ frame }
	{
	}
	auto BitmapFrameDecode::GetMetadataReader() const -> MetadataReader
	{
		ComPtr<IWICMetadataQueryReader> reader;
		auto hr = Get()->GetMetadataQueryReader(&reader); ThrowFailed(hr);

		return reader;
	}
	auto BitmapFrameDecode::GetThumbnail() const -> BitmapSource<>
	{
		ComPtr<IWICBitmapSource> thumbnail;
		auto hr = Get()->GetThumbnail(&thumbnail); ThrowFailed(hr);

		return thumbnail;
	}
	auto BitmapFrameDecode::GetColorContexts(UINT count) const -> std::vector<ComPtr<IWICColorContext>>
	{
		std::vector<IWICColorContext*> contexts(count);
		auto hr = Get()->GetColorContexts(count, contexts.data(), nullptr); ThrowFailed(hr);

		return contexts | std::ranges::views::transform([](auto context)
		{
			return ComPtr<IWICColorContext>{ context };
		}) | std::ranges::to<std::vector<ComPtr<IWICColorContext>>>();
	}
}

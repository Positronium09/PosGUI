module;
#include <ranges>
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.BitmapFrameDecode;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.MetadataReader;

namespace PGUI::UI::Imaging
{
	BitmapFrameDecode::BitmapFrameDecode(const ComPtr<IWICBitmapFrameDecode>& frame) noexcept :
		BitmapSource{ frame }
	{ }

	auto BitmapFrameDecode::GetMetadataReader() const -> MetadataReader
	{
		ComPtr<IWICMetadataQueryReader> reader;
		const auto hr = Get()->GetMetadataQueryReader(&reader);
		ThrowFailed(hr);

		return reader;
	}

	auto BitmapFrameDecode::GetThumbnail() const -> BitmapSource<>
	{
		ComPtr<IWICBitmapSource> thumbnail;
		const auto hr = Get()->GetThumbnail(&thumbnail);
		ThrowFailed(hr);

		return thumbnail;
	}

	auto BitmapFrameDecode::GetColorContexts(const UINT count) const -> std::vector<ComPtr<IWICColorContext>>
	{
		std::vector<IWICColorContext*> contexts(count);
		const auto hr = Get()->GetColorContexts(count, contexts.data(), nullptr);
		ThrowFailed(hr);

		return contexts | std::ranges::views::transform([](auto context)
		{
			return ComPtr<IWICColorContext>{ context };
		}) | std::ranges::to<std::vector<ComPtr<IWICColorContext>>>();
	}
}

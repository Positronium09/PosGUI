module;
#include <ranges>
#include <wincodec.h>
#include <wrl.h>

module PGUI.UI.Imaging.BitmapFrameDecode;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;
import PGUI.UI.Imaging.BitmapSource;
import PGUI.UI.Imaging.MetadataReader;

namespace PGUI::UI::Imaging
{
	BitmapFrameDecode::BitmapFrameDecode(const ComPtr<IWICBitmapFrameDecode>& frame) noexcept :
		BitmapSource{ frame }
	{ }

	auto BitmapFrameDecode::GetMetadataReader() const noexcept -> Result<MetadataReader>
	{
		ComPtr<IWICMetadataQueryReader> reader;

		if (const auto hr = Get()->GetMetadataQueryReader(&reader);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get metadata query reader");
			return Unexpected{ error };
		}

		return reader;
	}

	auto BitmapFrameDecode::GetThumbnail() const noexcept -> Result<BitmapSource<>>
	{
		ComPtr<IWICBitmapSource> thumbnail;

		if (const auto hr = Get()->GetThumbnail(&thumbnail);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get thumbnail");
			return Unexpected{ error };
		}

		return thumbnail;
	}

	auto BitmapFrameDecode::GetColorContexts(
		const UINT count) const noexcept -> Result<std::vector<ComPtr<IWICColorContext>>>
	{
		std::vector<IWICColorContext*> contexts(count);
		if (const auto hr = Get()->GetColorContexts(count, contexts.data(), nullptr);
			FAILED(hr))
		{
			Error error{ hr };
			error.AddTag(ErrorTags::Imaging);
			Logger::Error(error, L"Failed to get color contexts");
			return Unexpected{ error };
		}

		return contexts | std::ranges::views::transform([](auto context)
		{
			return ComPtr<IWICColorContext>{ context };
		}) | std::ranges::to<std::vector<ComPtr<IWICColorContext>>>();
	}
}

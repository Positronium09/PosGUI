module;
#include <wincodec.h>

export module PGUI.Factories:WICFactory;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::Factories::WICFactory::Details
{
	ComPtr<IWICImagingFactory2> factory{ nullptr };
	std::once_flag initFlag;
}

export namespace PGUI::Factories::WICFactory
{
	[[nodiscard]] auto GetFactory() -> const ComPtr<IWICImagingFactory2>&
	{
		std::call_once(Details::initFlag, []
		{
			if (const auto hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				GetIID<IWICImagingFactory2>(),
				Details::factory.put_void());
				FAILED(hr))
			{
				const Error error{ hr };
				Logger::Critical(error, L"Failed to create WIC factory");
				throw Exception{ error };
			}
		});

		return Details::factory;
	}
}

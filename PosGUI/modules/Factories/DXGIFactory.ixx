module;
#include <dxgi1_6.h>
#include <Windows.h>

export module PGUI.Factories:DXGIFactory;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::Factories::DXGIFactory::Details
{
	ComPtr<IDXGIFactory7> factory{ nullptr };
	std::once_flag initFlag;
}

export namespace PGUI::Factories::DXGIFactory
{
	[[nodiscard]] auto GetFactory() -> const ComPtr<IDXGIFactory7>&
	{
		std::call_once(Details::initFlag, []
		{
			UINT flags = 0;

			#ifdef _DEBUG
			flags = DXGI_CREATE_FACTORY_DEBUG;
			#endif

			if (const auto hr = CreateDXGIFactory2(
				flags,
				GetIID<IDXGIFactory7>(),
				Details::factory.put_void());
				FAILED(hr))
			{
				const Error error{ hr };
				Logger::Critical(error, L"Failed to create DXGI factory");
				throw Exception{ error };
			}
		});

		return Details::factory;
	}
}

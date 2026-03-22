module;
#include <dwrite_3.h>

export module PGUI.Factories:DWriteFactory;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::Factories::DWriteFactory::Details
{
	ComPtr<IDWriteFactory8> factory{ nullptr };
	std::once_flag initFlag;
}

export namespace PGUI::Factories::DWriteFactory
{
	[[nodiscard]] auto GetFactory() -> const ComPtr<IDWriteFactory8>&
	{
		std::call_once(Details::initFlag, []
		{
			if (const auto hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				GetIID<IDWriteFactory8>(),
				Details::factory.put_unknown());
				FAILED(hr))
			{
				const Error error{ hr };
				Logger::Critical(error, L"Failed to create DirectWrite factory");
				throw Exception{ error };
			}
		});

		return Details::factory;
	}
}

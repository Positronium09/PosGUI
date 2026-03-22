module;
#include <d2d1.h>
#include <d2d1_3.h>

export module PGUI.Factories:D2DFactory;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

namespace PGUI::Factories::D2DFactory::Details
{
	ComPtr<ID2D1Factory8> factory{ nullptr };
	std::once_flag initFlag;
}

export namespace PGUI::Factories::D2DFactory
{
	[[nodiscard]] auto GetFactory() -> const ComPtr<ID2D1Factory8>&
	{
		std::call_once(Details::initFlag, []
		{
			constexpr D2D1_FACTORY_OPTIONS options{
				#ifdef _DEBUG
				.debugLevel = D2D1_DEBUG_LEVEL_ERROR
				#endif
			};

			if (const auto hr = D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				options, Details::factory.put());
				FAILED(hr))
			{
				const Error error{ hr };
				Logger::Critical(error, L"Failed to create Direct2D factory");
				throw Exception{ error };
			}
		});

		return Details::factory;
	}
}

module;
#include <d2d1.h>
#include <d2d1_3.h>
#include <wrl.h>

export module PGUI.Factories:D2DFactory;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::Factories
{
	class D2DFactory
	{
		public:
		D2DFactory() = delete;

		D2DFactory(const D2DFactory&) = delete;

		auto operator=(const D2DFactory&) -> D2DFactory & = delete;

		D2DFactory(D2DFactory&&) = delete;

		auto operator=(D2DFactory&&) -> D2DFactory & = delete;

		~D2DFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!factory)
			{
				D2D1_FACTORY_OPTIONS options{ };

				#ifdef _DEBUG
				options.debugLevel = D2D1_DEBUG_LEVEL_ERROR;
				#endif

				if (const auto hr = D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED,
					options, factory.GetAddressOf());
					FAILED(hr))
				{
					const Error error{ hr };
					Logger::Critical(error, L"Failed to create Direct2D factory");
					throw Exception{ error };
				}
			}

			return factory;
		}

		private:
		static inline ComPtr<ID2D1Factory8> factory = nullptr;
	};
}

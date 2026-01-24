module;
#include <wincodec.h>

export module PGUI.Factories:WICFactory;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::Factories
{
	class WICFactory
	{
		public:
		WICFactory() = delete;

		WICFactory(const WICFactory&) = delete;

		auto operator=(const WICFactory&) -> WICFactory & = delete;

		WICFactory(WICFactory&&) = delete;

		auto operator=(WICFactory&&) -> WICFactory & = delete;

		~WICFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!factory)
			{
				if (const auto hr = CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					GetIID(factory),
					factory.put_void());
					FAILED(hr))
				{
					const Error error{ hr };
					Logger::Critical(error, L"Failed to create WIC factory");
					throw Exception{ error };
				}
			}

			return factory;
		}

		private:
		static inline ComPtr<IWICImagingFactory2> factory = nullptr;
	};
}

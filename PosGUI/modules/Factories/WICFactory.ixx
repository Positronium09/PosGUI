module;
#include <wincodec.h>
#include <wrl.h>

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

		auto operator=(const WICFactory&) -> WICFactory& = delete;

		WICFactory(WICFactory&&) = delete;

		auto operator=(WICFactory&&) -> WICFactory& = delete;

		~WICFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!factory)
			{
				const auto hr = CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					__uuidof(IWICImagingFactory2),
					std::bit_cast<void**>(factory.GetAddressOf())
				);
				ThrowFailed(hr);
			}

			return factory;
		}

		private:
		static inline ComPtr<IWICImagingFactory2> factory = nullptr;
	};
}

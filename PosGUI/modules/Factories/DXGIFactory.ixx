module;
#include <dxgi1_6.h>
#include <Windows.h>

export module PGUI.Factories:DXGIFactory;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::Factories
{
	class DXGIFactory
	{
		public:
		DXGIFactory() = delete;

		DXGIFactory(const DXGIFactory&) = delete;

		auto operator=(const DXGIFactory&) -> DXGIFactory & = delete;

		DXGIFactory(DXGIFactory&&) = delete;

		auto operator=(DXGIFactory&&) -> DXGIFactory & = delete;

		~DXGIFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!dxgiFactory)
			{
				UINT flags = 0;

				#ifdef _DEBUG
				flags = DXGI_CREATE_FACTORY_DEBUG;
				#endif

				if (const auto hr = CreateDXGIFactory2(
					flags,
					GetIID(dxgiFactory),
					dxgiFactory.put_void());
					FAILED(hr))
				{
					const Error error{ hr };
					Logger::Critical(error, L"Failed to create DXGI factory");
					throw Exception{ error };
				}
			}

			return dxgiFactory;
		}

		private:
		static inline ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	};
}

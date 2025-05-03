module;
#include <bit>
#include <wrl.h>
#include <dxgi1_6.h>
#include <Windows.h>

export module PGUI.Factories:DXGIFactory;

import PGUI.ComPtr;
import PGUI.Exceptions;

export namespace PGUI::Factories
{
	class DXGIFactory
	{
		public:
		DXGIFactory() = delete;
		DXGIFactory(const DXGIFactory&) = delete;
		auto operator=(const DXGIFactory&)->DXGIFactory & = delete;
		DXGIFactory(DXGIFactory&&) = delete;
		auto operator=(DXGIFactory&&)->DXGIFactory & = delete;
		~DXGIFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!dxgiFactory)
			{
				UINT flags = 0;

				#ifdef _DEBUG
				flags = DXGI_CREATE_FACTORY_DEBUG;
				#endif

				auto hr = CreateDXGIFactory2(flags,
					__uuidof(IDXGIFactory7), std::bit_cast<void**>((dxgiFactory.GetAddressOf()))); ThrowFailed(hr);
			}

			return dxgiFactory;
		}

		private:
		static inline ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	};
}

module;
#include <dwrite_3.h>
#include <Windows.h>
#include <wrl.h>

export module PGUI.Factories:DWriteFactory;

import PGUI.ComPtr;
import PGUI.Exceptions;

export namespace PGUI::Factories
{
	class DWriteFactory
	{
		public:
		DWriteFactory() = delete;
		DWriteFactory(const DWriteFactory&) = delete;
		auto operator=(const DWriteFactory&) -> DWriteFactory & = delete;
		DWriteFactory(DWriteFactory&&) = delete;
		auto operator=(DWriteFactory&&)->DWriteFactory & = delete;
		~DWriteFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!directWriteFactory)
			{
				auto hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory8), (IUnknown**)directWriteFactory.GetAddressOf()); ThrowFailed(hr);
			}
			return directWriteFactory;
		}

		private:
		static inline ComPtr<IDWriteFactory8> directWriteFactory = nullptr;
	};
}

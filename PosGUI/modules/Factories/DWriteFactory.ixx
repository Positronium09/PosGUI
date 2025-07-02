module;
#include <dwrite_3.h>
#include <wrl.h>

export module PGUI.Factories:DWriteFactory;

import std;

import PGUI.ComPtr;
import PGUI.Exceptions;

export namespace PGUI::Factories
{
	class DWriteFactory
	{
		public:
		DWriteFactory() = delete;

		DWriteFactory(const DWriteFactory&) = delete;

		auto operator=(const DWriteFactory&) -> DWriteFactory& = delete;

		DWriteFactory(DWriteFactory&&) = delete;

		auto operator=(DWriteFactory&&) -> DWriteFactory& = delete;

		~DWriteFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!directWriteFactory)
			{
				const auto hr =
					DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
					                    __uuidof(IDWriteFactory8),
					                    std::bit_cast<IUnknown**>(directWriteFactory.GetAddressOf()));
				ThrowFailed(hr);
			}
			return directWriteFactory;
		}

		private:
		static inline ComPtr<IDWriteFactory8> directWriteFactory = nullptr;
	};
}

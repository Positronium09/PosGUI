module;
#include <dwrite_3.h>

export module PGUI.Factories:DWriteFactory;

import std;

import PGUI.ComPtr;
import PGUI.ErrorHandling;

export namespace PGUI::Factories
{
	class DWriteFactory
	{
		public:
		DWriteFactory() = delete;

		DWriteFactory(const DWriteFactory&) = delete;

		auto operator=(const DWriteFactory&) -> DWriteFactory & = delete;

		DWriteFactory(DWriteFactory&&) = delete;

		auto operator=(DWriteFactory&&) -> DWriteFactory & = delete;

		~DWriteFactory() = default;

		[[nodiscard]] static auto GetFactory()
		{
			if (!directWriteFactory)
			{
				if (const auto hr = DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					GetIID(directWriteFactory),
					directWriteFactory.put_unknown());
					FAILED(hr))
				{
					const Error error{ hr };
					Logger::Critical(error, L"Failed to create DirectWrite factory");
					throw Exception{ error };
				}
			}
			return directWriteFactory;
		}

		private:
		static inline ComPtr<IDWriteFactory8> directWriteFactory = nullptr;
	};
}
